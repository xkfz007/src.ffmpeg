#ifndef _CONFIG_H_
#define _CONFIG_H_
#define HAVE_MALLOC_H 1
#define HAVE_IO_H 1
#define CONFIG_MEMORY_POISONING 0
#define CONFIG_FTRAPV 0
#define ARCH_AARCH64 0
#define ARCH_ARM 0
#define ARCH_X86 0
#define ARCH_PPC 0
#define ARCH_MIPS 0
#define CONFIG_XVMC 0
#define CONFIG_ERROR_RESILIENCE 1
#define CONFIG_MPEGVIDEO 1
#define HAVE_THREADS 1
#define CONFIG_GRAY 0
#define CONFIG_SVQ3_DECODER 0
#define CONFIG_SMALL 0
#define CONFIG_H264_D3D11VA_HWACCEL 0
#define CONFIG_H264_DXVA2_HWACCEL 0
#define CONFIG_H264_MMAL_HWACCEL 0
#define CONFIG_H264_QSV_HWACCEL 0
#define CONFIG_H264_VAAPI_HWACCEL 0
#define CONFIG_H264_VDA_HWACCEL 0
#define CONFIG_H264_VDA_OLD_HWACCEL 0
#define CONFIG_H264_VDPAU_HWACCEL 0

#define HAVE_PTHREADS 0
#define HAVE_OS2THREADS 0
#define HAVE_W32THREADS 1
#define HAVE_ATOMICS_NATIVE 1
#define HAVE_DOS_PATHS 1

#define CONFIG_ME_CMP 1
#define CONFIG_FRAME_THREAD_ENCODER 1
#define CONFIG_MDCT 0
#define CONFIG_FAANDCT 1
#define CONFIG_ICONV 0

#define HAVE_ATOMICS_GCC 0
#define HAVE_ATOMICS_SUNCC 0
#define HAVE_ATOMICS_WIN32 1

#define FFMPEG_CONFIGURATION "--disable-ffserver --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --disable-network --disable-d3d11va --disable-dxva2 --disable-vaapi --disable-vda --disable-vdpau --disable-bzlib --disable-iconv --disable-lzma --disable-securetransport --disable-xlib --disable-asm --disable-optimizations --disable-stripping"
#define FFMPEG_LICENSE "LGPL version 2.1 or later"

#define av_restrict restrict

#ifdef WIN32
#include "compat\msvcrt\snprintf.h"
#define strtoll _strtoi64
#endif
#endif