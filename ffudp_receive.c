/******************************************************************************
 * \File
 *   main.c
 * \Brief
 *   Recording udp-multicast data to a file
 * \Author
 *   Hank
 * \Created date
 *   2013-02-25
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 8
#endif

#define UDP_TX_BUF_SIZE 32768
#define UDP_MAX_PKT_SIZE 65536

#ifndef IN_MULTICAST
#define IN_MULTICAST(a) ((((uint32_t)(a)) & 0xf0000000) == 0xe0000000)
#endif

#define HAVE_AVX 1
#define HAVE_PTHREADS 1
#define ALIGN (HAVE_AVX ? 32 : 16)

#define AVIO_FLAG_READ  1       /**< read-only */
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.
#define closesocket close
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define ff_neterrno() AVERROR(errno)

typedef struct AVFifoBuffer {
  uint8_t *buffer;
  uint8_t *rptr, *wptr, *end;
  uint32_t rndx, wndx;
} AVFifoBuffer;


typedef struct {
  int udp_fd;
  int ttl;
  int buffer_size;
  int is_multicast;
  /*Added by Anderson, Fixed udp multicast*/
  int  b_localaddr_flag;
  char local_addr[20];
  /*end*/


  int local_port;
  int reuse_socket;
  struct sockaddr_storage dest_addr;
  int dest_addr_len;
  int is_connected;


  /* Circular Buffer variables for use in UDP receive code */
  int circular_buffer_size;
  AVFifoBuffer *fifo;
  int circular_buffer_error;
  #if HAVE_PTHREADS
  pthread_t circular_buffer_thread;
  #endif
} UDPContext;


typedef struct URLContext {
  int flags;
  int is_streamed;            /**< true if streamed (no seek possible), default = false */
  int max_packet_size;          /**< if non zero, the stream is packetized with this max packet size */
  void *priv_data;
  int is_connected;
} URLContext;


static int default_interrupt_cb(void);
int (*url_interrupt_cb)(void) = default_interrupt_cb;


int udp_set_url(struct sockaddr_storage *addr,
const char *hostname, int port);
int ff_is_multicast_address(struct sockaddr *addr);


size_t av_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = 0;
    while (++len < size && *src)
        *dst++ = *src++;
    if (len <= size)
        *dst = 0;
    return len + strlen(src) - 1;
}

void av_url_split(char *proto, int proto_size,
                  char *authorization, int authorization_size,
                  char *hostname, int hostname_size,
                  int *port_ptr, char *path, int path_size, const char *url)
{
    const char *p, *ls, *ls2, *at, *at2, *col, *brk;

    if (port_ptr)
        *port_ptr = -1;
    if (proto_size > 0)
        proto[0] = 0;
    if (authorization_size > 0)
        authorization[0] = 0;
    if (hostname_size > 0)
        hostname[0] = 0;
    if (path_size > 0)
        path[0] = 0;

    /* parse protocol */
    if ((p = strchr(url, ':'))) {
        av_strlcpy(proto, url, FFMIN(proto_size, p + 1 - url));
        p++; /* skip ':' */
        if (*p == '/')
            p++;
        if (*p == '/')
            p++;
    } else {
        /* no protocol means plain filename */
        av_strlcpy(path, url, path_size);
        return;
    }

    /* separate path from hostname */
    ls = strchr(p, '/');
    ls2 = strchr(p, '?');
    if (!ls)
        ls = ls2;
    else if (ls && ls2)
        ls = FFMIN(ls, ls2);
    if (ls)
        av_strlcpy(path, ls, path_size);
    else
        ls = &p[strlen(p)];  // XXX

    /* the rest is hostname, use that to parse auth/port */
    if (ls != p) {
        /* authorization (user[:pass]@hostname) */
        at2 = p;
        while ((at = strchr(p, '@')) && at < ls) {
            av_strlcpy(authorization, at2,
                       FFMIN(authorization_size, at + 1 - at2));
            p = at + 1; /* skip '@' */
        }

        if (*p == '[' && (brk = strchr(p, ']')) && brk < ls) {
            /* [host]:port */
            av_strlcpy(hostname, p + 1,
                       FFMIN(hostname_size, brk - p));
            if (brk[1] == ':' && port_ptr)
                *port_ptr = atoi(brk + 2);
        } else if ((col = strchr(p, ':')) && col < ls) {
            av_strlcpy(hostname, p,
                       FFMIN(col + 1 - p, hostname_size));
            if (port_ptr)
                *port_ptr = atoi(col + 1);
        } else
            av_strlcpy(hostname, p,
                       FFMIN(ls + 1 - p, hostname_size));
    }
}
/**         
 * If no filename is given to av_open_input_file because you want to
 * get the local port first, then you must call this function to set
 * the remote server address.
 *      
 * url syntax: udp://host:port[?option=val...]
 * option: 'ttl=n'       : set the ttl value (for multicast only)                                                                                 
 *         'localport=n' : set the local port
 *         'pkt_size=n'  : set max packet size
 *         'reuse=1'     : enable reusing the socket
 *      
 * @param h media file context
 * @param uri of the remote server
 * @return zero if no error.
 */ 
int ff_udp_set_remote_url(URLContext *h, const char *uri)
{   
  UDPContext *s = h->priv_data;
  char hostname[256], buf[10];
  int port;
  const char *p;


  av_url_split(NULL, 0, NULL, 0, hostname, sizeof(hostname), &port, NULL, 0, uri);
  //strcpy(hostname, "225.1.1.31");
  //port = 1234;


  /* set the destination address */
  s->dest_addr_len = udp_set_url(&s->dest_addr, hostname, port);
  if (s->dest_addr_len < 0) {
  return AVERROR(EIO);
  }   
  s->is_multicast = ff_is_multicast_address((struct sockaddr*) &s->dest_addr);


  return 0; 
}


struct addrinfo* udp_resolve_host(const char *hostname, int port, int type, int family, int flags)
{
  struct addrinfo hints, *res = 0;
  int error;
  char sport[16];
  const char *node = 0, *service = "0";


  if (port > 0) 
  {
    snprintf(sport, sizeof(sport), "%d", port);
    service = sport;
  }
  if ((hostname) && (hostname[0] != '\0') && (hostname[0] != '?')) 
  {
    node = hostname;
  }


  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = type;
  hints.ai_family   = family;
  hints.ai_flags = flags;
  if ((error = getaddrinfo(node, service, &hints, &res))) 
  {
    res = NULL;
    //av_log(NULL, AV_LOG_ERROR, "udp_resolve_host: %s\n", gai_strerror(error));
    fprintf(stderr, "udp_resolve_host: %s\n", strerror(error));
  }

  return res;
}   


int udp_set_url(struct sockaddr_storage *addr, const char *hostname, int port)
{   
  struct addrinfo *res0;
  int addr_len;


  res0 = udp_resolve_host(hostname, port, SOCK_DGRAM, AF_UNSPEC, 0);
  if (res0 == 0) 
    return AVERROR(EIO);


  memcpy(addr, res0->ai_addr, res0->ai_addrlen);
  addr_len = res0->ai_addrlen;
  freeaddrinfo(res0);


  return addr_len;
}


int ff_is_multicast_address(struct sockaddr *addr)
{                   
  if (addr->sa_family == AF_INET) {
    return IN_MULTICAST(ntohl(((struct sockaddr_in *)addr)->sin_addr.s_addr));
  }
  return 0;
}
    
int udp_socket_create(UDPContext *s, struct sockaddr_storage *addr, int *addr_len)
{ 
  int udp_fd = -1;
  struct addrinfo *res0 = NULL, *res = NULL; 
  int family = AF_UNSPEC;


  if (((struct sockaddr *) &s->dest_addr)->sa_family)
    family = ((struct sockaddr *) &s->dest_addr)->sa_family;


  res0 = udp_resolve_host(0, s->local_port, SOCK_DGRAM, family, AI_PASSIVE);
  if (res0 == 0)
    goto fail;


  for (res = res0; res; res=res->ai_next) 
  {
    udp_fd = socket(res->ai_family, SOCK_DGRAM, 0);
    if (udp_fd > 0) 
      break;
    //av_log(NULL, AV_LOG_ERROR, "socket: %s\n", strerror(errno));
    fprintf(stderr, "socket: %s\n", strerror(errno));
  }


  if (udp_fd < 0)
    goto fail;


  memcpy(addr, res->ai_addr, res->ai_addrlen);
  *addr_len = res->ai_addrlen;
  freeaddrinfo(res0);


  return udp_fd; 


fail:
  if (udp_fd >= 0)
   closesocket(udp_fd);                                                                                                                      
  if(res0)
   freeaddrinfo(res0);
  return -1;
}     


int udp_port(struct sockaddr_storage *addr, int addr_len) 
{   
  char sbuf[sizeof(int)*3+1];


  if (getnameinfo((struct sockaddr *)addr, addr_len, NULL, 0,  sbuf, sizeof(sbuf), NI_NUMERICSERV) != 0) {
    //av_log(NULL, AV_LOG_ERROR, "getnameinfo: %s\n", strerror(errno));
    fprintf(stderr, "getnameinfo: %s\n", strerror(errno));
    return -1;
  }


  return strtol(sbuf, NULL, 10);
}




int udp_join_multicast_group(int sockfd, struct sockaddr *addr, int b_localaddr, char *pc_local_addr)
{
  #ifdef IP_ADD_MEMBERSHIP 
  if (addr->sa_family == AF_INET) {
    struct ip_mreq mreq;


    mreq.imr_multiaddr.s_addr = ((struct sockaddr_in *)addr)->sin_addr.s_addr;
    if ( 0 == b_localaddr )
    {   
      mreq.imr_interface.s_addr= INADDR_ANY; 
    } 
    else
    { 
      mreq.imr_interface.s_addr= inet_addr(pc_local_addr);
    }


    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&mreq, sizeof(mreq)) < 0) {
      //av_log(NULL, AV_LOG_ERROR, "setsockopt(IP_ADD_MEMBERSHIP): %s\n", strerror(errno));
      fprintf(stderr, "setsockopt(IP_ADD_MEMBERSHIP): %s\n", strerror(errno));
      return -1;
    }
  }   
  #endif
  return 0;                                                                                                                                     
}


void av_free(void *ptr)
{
  free(ptr);
}


void av_freep(void *arg)
{   
  void **ptr= (void**)arg;
  av_free(*ptr);
  *ptr = NULL; 
}


void av_fifo_free(AVFifoBuffer *f)
{   
  if(f){
    av_freep(&f->buffer);
    av_free(f);
  }
}


#define MAX_MALLOC_SIZE INT_MAX
void *av_malloc(size_t size)
{
  void *ptr = NULL;


  /* let's disallow possible ambiguous cases */
  if (size > (MAX_MALLOC_SIZE-32))
    return NULL;


  if (size) //OSX on SDK 10.6 has a broken posix_memalign implementation
    if (posix_memalign(&ptr,ALIGN,size))
      ptr = NULL;


  if (!ptr && !size)
    ptr= av_malloc(1);


  return ptr;                                                                                                                                   
} 




void *av_mallocz(size_t size)
{   
  void *ptr = av_malloc(size);
  if (ptr)
    memset(ptr, 0, size);
  return ptr;
}


void av_fifo_reset(AVFifoBuffer *f)
{       
  f->wptr = f->rptr = f->buffer;
  f->wndx = f->rndx = 0;
}


int av_fifo_size(AVFifoBuffer *f)
{       
   return (uint32_t)(f->wndx - f->rndx);
}           
                
int av_fifo_space(AVFifoBuffer *f)
{           
   return f->end - f->buffer - av_fifo_size(f);
}




AVFifoBuffer *av_fifo_alloc(unsigned int size)
{       
  AVFifoBuffer *f= av_mallocz(sizeof(AVFifoBuffer));
  if(!f)
    return NULL;
  f->buffer = av_malloc(size);
  f->end = f->buffer + size;
  av_fifo_reset(f); 
  if (!f->buffer)
    av_freep(&f);
  return f;
} 


static int default_interrupt_cb(void)
{
   return 0;
}


void *circular_buffer_task( void *_URLContext)
{
  URLContext *h = _URLContext;
  UDPContext *s = h->priv_data;
  fd_set rfds;
  struct timeval tv;


  for(;;) {
    int left;
    int ret;
    int len;


    if (url_interrupt_cb()) {
      s->circular_buffer_error = EINTR;
      return NULL;
    }


    FD_ZERO(&rfds);
    FD_SET(s->udp_fd, &rfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    ret = select(s->udp_fd + 1, &rfds, NULL, NULL, &tv);
    if (ret < 0) {
      if (ff_neterrno() == AVERROR(EINTR))
        continue;
      s->circular_buffer_error = EIO;
      return NULL;
    }


    if (!(ret > 0 && FD_ISSET(s->udp_fd, &rfds)))
      continue;


    /* How much do we have left to the end of the buffer */
    /* Whats the minimum we can read so that we dont comletely fill the buffer */
    left = av_fifo_space(s->fifo);
    left = FFMIN(left, s->fifo->end - s->fifo->wptr);


    /* No Space left, error, what do we do now */
    if( !left) { 
      //av_log(h, AV_LOG_ERROR, "circular_buffer: OVERRUN\n");
      fprintf(stderr, "circular_buffer: OVERRUN\n");
      s->circular_buffer_error = EIO; 
      return NULL;
    }


    len = recv(s->udp_fd, s->fifo->wptr, left, 0);
    if (len < 0) {
      if (ff_neterrno() != AVERROR(EAGAIN) && ff_neterrno() != AVERROR(EINTR)) {
        s->circular_buffer_error = EIO;
      return NULL;
      }
    }
    s->fifo->wptr += len;
    if (s->fifo->wptr >= s->fifo->end)
    s->fifo->wptr = s->fifo->buffer;
    s->fifo->wndx += len;
  }
  return NULL;
}




/** Discard data from the FIFO. */
void av_fifo_drain(AVFifoBuffer *f, int size)
{   
  f->rptr += size;
  if (f->rptr >= f->end)
  f->rptr -= f->end - f->buffer;
  f->rndx += size;
} 


int av_fifo_generic_read(AVFifoBuffer *f, void *dest, int buf_size, void (*func)(void*, void*, int))
{               
  // Read memory barrier needed for SMP here in theory
  do {        
    int len = FFMIN(f->end - f->rptr, buf_size);


    if(func) 
    {
      func(dest, f->rptr, len);
    }
    else
    {   
      memcpy(dest, f->rptr, len);
      dest = (uint8_t*)dest + len;
    } 
    // memory barrier needed for SMP here in theory
    av_fifo_drain(f, len);
    buf_size -= len;
  } while (buf_size > 0);


  return 0;
}


int udp_read(URLContext *h, uint8_t *buf, int size)
{
  UDPContext *s = h->priv_data;
  int ret;
  int avail;
  fd_set rfds;
  struct timeval tv;


  if (s->fifo) {
    do {
      avail = av_fifo_size(s->fifo);
      if (avail) 
      { // >=size) {
        // Maximum amount available
        size = FFMIN( avail, size);
        av_fifo_generic_read(s->fifo, buf, size, NULL);
        return size;
      }
      else 
      {
        FD_ZERO(&rfds);
        FD_SET(s->udp_fd, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        ret = select(s->udp_fd + 1, &rfds, NULL, NULL, &tv);
        if (ret<0)
          return ret;
      }
    } while( 1);
  }


  return 0;
}


int udp_leave_multicast_group(int sockfd, struct sockaddr *addr, int b_localaddr, char *pc_local_addr)
{
  if (addr->sa_family == AF_INET) {
    struct ip_mreq mreq;


    mreq.imr_multiaddr.s_addr = ((struct sockaddr_in *)addr)->sin_addr.s_addr;


    if( 0 == b_localaddr )
      mreq.imr_interface.s_addr= INADDR_ANY;
    else
      mreq.imr_interface.s_addr= inet_addr(pc_local_addr);


    if (setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const void *)&mreq, sizeof(mreq)) < 0) {
      //av_log(NULL, AV_LOG_ERROR, "setsockopt(IP_DROP_MEMBERSHIP): %s\n", strerror(errno));
      fprintf(stderr, "setsockopt(IP_DROP_MEMBERSHIP): %s\n", strerror(errno));
      return -1;
    }
  }
                                                                                                         
  return 0;                                                                                                        
}     




int udp_close(URLContext *h)
{
  UDPContext *s = h->priv_data;


  /*Modified by Anderson, fixed udp multicast*/
  if (s->is_multicast && (h->flags & AVIO_FLAG_READ))
  udp_leave_multicast_group(s->udp_fd, (struct sockaddr *)&s->dest_addr, s->b_localaddr_flag, s->local_addr);
  /*end*/


  closesocket(s->udp_fd);
  av_fifo_free(s->fifo);
  av_free(s);
  return 0;
}


int main(int argc, char* argv[])
{
  URLContext *h, h_url;
  char hostname[1024];
  int len;
  int is_output = 0;
  int port, udp_fd = -1, tmp, bind_ret = -1;
  struct sockaddr_storage my_addr;
  UDPContext *s = NULL;


  char read_buf[4096];


  h = &h_url;
  h->flags = 1;
  h->is_streamed = 1;
  h->max_packet_size = 1472;


  s = av_mallocz(sizeof(UDPContext));
  if (!s)
   return AVERROR(ENOMEM); 


  h->priv_data = s;
  s->ttl = 16;
  s->buffer_size = UDP_MAX_PKT_SIZE; 
  s->circular_buffer_size = 7*188*4096;


  s->b_localaddr_flag = 1;
  //s->local_addr = "192.168.1.73";
  strcpy(s->local_addr, "192.168.1.73");


  //av_url_split(NULL, 0, NULL, 0, hostname, sizeof(hostname), &port, NULL, 0, uri);
  strcpy(hostname, "225.1.1.31");
  port = 1234;


  if(ff_udp_set_remote_url(h, "udp://225.1.1.31:1234") < 0)
   goto fail;


  if (s->is_multicast || !s->local_port)
  {
    s->local_port = port; //1234
  }


  udp_fd = udp_socket_create(s, &my_addr, &len);
  if (udp_fd < 0)
   goto fail;


  /* Follow the requested reuse option, unless it's multicast in which
   * case enable reuse unless explicitely disabled.
   */
  //if (s->reuse_socket || (s->is_multicast && !reuse_specified)) 
  if (s->reuse_socket || (s->is_multicast /*&& !reuse_specified*/)) 
  {
    s->reuse_socket = 1;
    if (setsockopt (udp_fd, SOL_SOCKET, SO_REUSEADDR, &(s->reuse_socket), sizeof(s->reuse_socket)) != 0)
     goto fail;
  }


  /* the bind is needed to give a port to the socket now */
  /* if multicast, try the multicast address bind first */
  if (s->is_multicast && (h->flags & AVIO_FLAG_READ)) {
    bind_ret = bind(udp_fd,(struct sockaddr *)&s->dest_addr, len);
  }


  len = sizeof(my_addr);
  getsockname(udp_fd, (struct sockaddr *)&my_addr, &len);
  s->local_port = udp_port(&my_addr, len);


  if (s->is_multicast) {
    if (udp_join_multicast_group(udp_fd, (struct sockaddr *)&s->dest_addr, s->b_localaddr_flag, s->local_addr) < 0)
      goto fail;
  }


  /* set udp recv buffer size to the largest possible udp packet size to
   * avoid losing data on OSes that set this too low by default. */
  tmp = s->buffer_size;
  if (setsockopt(udp_fd, SOL_SOCKET, SO_RCVBUF, &tmp, sizeof(tmp)) < 0) {
  //av_log(h, AV_LOG_WARNING, "setsockopt(SO_RECVBUF): %s\n", strerror(errno));
  fprintf(stderr, "setsockopt(SO_RECVBUF): %s\n", strerror(errno));
  }
  /* make the socket non-blocking */
  //ff_socket_nonblock(udp_fd, 1);
  fcntl(udp_fd, F_SETFL, fcntl(udp_fd, F_GETFL) | O_NONBLOCK);


  s->udp_fd = udp_fd;


  #if HAVE_PTHREADS
  if (!is_output && s->circular_buffer_size) 
  {
    /* start the task going */
    s->fifo = av_fifo_alloc(s->circular_buffer_size);


    if (pthread_create(&s->circular_buffer_thread, NULL, circular_buffer_task, h)) {
      //av_log(h, AV_LOG_ERROR, "pthread_create failed\n");
      fprintf(stderr, "pthread_create failed\n");
      goto fail;
    }
  }
  #endif


  //sleep(2);


  udp_read(h, &read_buf, 2048);


  udp_close(h);
  return 0;
fail:
  if (udp_fd >= 0)
   closesocket(udp_fd);
  av_fifo_free(s->fifo);
  av_free(s);
  return AVERROR(EIO);
}

