#ifndef	_MPEG_PES_DEMUX_H_
#define _MPEG_PES_DEMUX_H_

#include "MpegTsPacketReader.h"

//#define RECORD_MPEG_PES_PACKET_DATA

#define MAX_MPEG_PES_PACKET_SIZE			(512*1024)

typedef struct tagMPEG_PES_DEMUXER MPEG_PES_DEMUXER;
typedef MPEG_PES_DEMUXER * P_MPEG_PES_DEMUXER;

#ifdef __cplusplus
extern "C"
{
#endif	//__cplusplus

P_MPEG_TS_PACKET_READER MpegPesDemux_CreateTsPacketReader();

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//_MPEG_PES_DEMUX_H_