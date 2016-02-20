/*
 * Register all the formats and protocols
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "avformat.h"
#include "rtp.h"
#include "rdt.h"
#include "url.h"
#include "version.h"

#define REGISTER_MUXER(X, x)                                            \
    {                                                                   \
        extern AVOutputFormat ff_##x##_muxer;                           \
        if (CONFIG_##X##_MUXER)                                         \
            av_register_output_format(&ff_##x##_muxer);                 \
    }

#define REGISTER_DEMUXER(X, x)                                          \
    {                                                                   \
        extern AVInputFormat ff_##x##_demuxer;                          \
        if (CONFIG_##X##_DEMUXER)                                       \
            av_register_input_format(&ff_##x##_demuxer);                \
    }

#define REGISTER_MUXDEMUX(X, x) REGISTER_MUXER(X, x); REGISTER_DEMUXER(X, x)

#define REGISTER_PROTOCOL(X, x)                                         \
    {                                                                   \
        extern URLProtocol ff_##x##_protocol;                           \
        if (CONFIG_##X##_PROTOCOL)                                      \
            ffurl_register_protocol(&ff_##x##_protocol);                \
    }

void av_register_all(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;

    avcodec_register_all();

    /* (de)muxers */
    REGISTER_DEMUXER (CONCAT,           concat);
    REGISTER_MUXDEMUX(FLV,              flv);
    REGISTER_DEMUXER (LIVE_FLV,         live_flv);
    REGISTER_MUXDEMUX(H264,             h264);
    REGISTER_MUXDEMUX(HEVC,             hevc);
    REGISTER_MUXDEMUX(HLS,              hls);
    REGISTER_MUXDEMUX(MATROSKA,         matroska);
    //REGISTER_MUXER   (MATROSKA_AUDIO,   matroska_audio);
    REGISTER_MUXER   (MP4,              mp4);
    REGISTER_MUXDEMUX(MPEGTS,           mpegts);
    REGISTER_MUXER   (NULL,             null);
    REGISTER_MUXDEMUX(RAWVIDEO,         rawvideo);
#ifdef NDEBUG
    REGISTER_MUXDEMUX(RTP,              rtp);
    REGISTER_MUXER   (RTP_MPEGTS,       rtp_mpegts);
    REGISTER_MUXDEMUX(RTSP,             rtsp);
#endif
	//REGISTER_DEMUXER (SDP,              sdp);
    REGISTER_MUXER   (SEGMENT,          segment);
    REGISTER_MUXER   (SEGMENT,          stream_segment);
    REGISTER_MUXER   (TEE,              tee);
    REGISTER_DEMUXER (TTY,              tty);
    //REGISTER_MUXDEMUX(YUV4MPEGPIPE,     yuv4mpegpipe);


    /* protocols */
#ifdef NDEBUG
    REGISTER_PROTOCOL(BLURAY,           bluray);
    REGISTER_PROTOCOL(CACHE,            cache);
    REGISTER_PROTOCOL(CONCAT,           concat);
    REGISTER_PROTOCOL(CRYPTO,           crypto);
    REGISTER_PROTOCOL(DATA,             data);
    REGISTER_PROTOCOL(FFRTMPCRYPT,      ffrtmpcrypt);
    REGISTER_PROTOCOL(FFRTMPHTTP,       ffrtmphttp);
#endif
    REGISTER_PROTOCOL(FILE,             file);
#ifdef NDEBUG
    REGISTER_PROTOCOL(FTP,              ftp);
    REGISTER_PROTOCOL(GOPHER,           gopher);
    REGISTER_PROTOCOL(HLS,              hls);
    REGISTER_PROTOCOL(HTTP,             http);
    REGISTER_PROTOCOL(HTTPPROXY,        httpproxy);
    REGISTER_PROTOCOL(HTTPS,            https);
    REGISTER_PROTOCOL(ICECAST,          icecast);
    REGISTER_PROTOCOL(MMSH,             mmsh);
    REGISTER_PROTOCOL(MMST,             mmst);
    REGISTER_PROTOCOL(MD5,              md5);
    REGISTER_PROTOCOL(PIPE,             pipe);
    REGISTER_PROTOCOL(RTMP,             rtmp);
    REGISTER_PROTOCOL(RTMPE,            rtmpe);
    REGISTER_PROTOCOL(RTMPS,            rtmps);
    REGISTER_PROTOCOL(RTMPT,            rtmpt);
    REGISTER_PROTOCOL(RTMPTE,           rtmpte);
    REGISTER_PROTOCOL(RTMPTS,           rtmpts);
    REGISTER_PROTOCOL(RTP,              rtp);
    REGISTER_PROTOCOL(SCTP,             sctp);
    REGISTER_PROTOCOL(SRTP,             srtp);
    REGISTER_PROTOCOL(SUBFILE,          subfile);
    REGISTER_PROTOCOL(TCP,              tcp);
    REGISTER_PROTOCOL(TLS_SECURETRANSPORT, tls_securetransport);
    REGISTER_PROTOCOL(TLS_GNUTLS,       tls_gnutls);
    REGISTER_PROTOCOL(TLS_OPENSSL,      tls_openssl);
    REGISTER_PROTOCOL(UDP,              udp);
    REGISTER_PROTOCOL(UDPLITE,          udplite);
    REGISTER_PROTOCOL(UNIX,             unix);

    /* external libraries */
    REGISTER_DEMUXER (LIBGME,           libgme);
    REGISTER_DEMUXER (LIBMODPLUG,       libmodplug);
    REGISTER_MUXDEMUX(LIBNUT,           libnut);
    REGISTER_DEMUXER (LIBQUVI,          libquvi);
    REGISTER_PROTOCOL(LIBRTMP,          librtmp);
    REGISTER_PROTOCOL(LIBRTMPE,         librtmpe);
    REGISTER_PROTOCOL(LIBRTMPS,         librtmps);
    REGISTER_PROTOCOL(LIBRTMPT,         librtmpt);
    REGISTER_PROTOCOL(LIBRTMPTE,        librtmpte);
    REGISTER_PROTOCOL(LIBSSH,           libssh);
    REGISTER_PROTOCOL(LIBSMBCLIENT,     libsmbclient);
#endif
}
