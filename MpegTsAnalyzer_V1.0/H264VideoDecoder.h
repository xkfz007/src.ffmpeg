#ifndef _H264_VIDEO_DECODER_H_
#define _H264_VIDEO_DECODER_H_

#include "MpegPesPacketReader.h"

//#define RECORD_MPEG_ES_PACKET_DATA

#define NAL_RBSP_PACKET_BUF_SIZE		(32*1024)
#define NAL_UNIT_BUF_SIZE				(256*1024)

typedef struct tagH264_VIDEO_DECODER H264_VIDEO_DECODER;
typedef H264_VIDEO_DECODER * P_H264_VIDEO_DECODER;

typedef struct {
	BYTE abNalRbspBuf[NAL_RBSP_PACKET_BUF_SIZE];
	INT iNalRbspSize;
}NAL_RBSP_PACKET, *P_NAL_RBSP_PACKET;

typedef struct{
	INT iNalUnitType;
	P_NAL_RBSP_PACKET pNalRbspPacket;
}NAL_UINT_PACKET, *P_NAL_UINT_PACKET;

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

P_MPEG_PES_PACKET_READER H264VideoDecoder_CreateMpegPesPackertReader(); 
INT InitNalRbspPacket(P_NAL_RBSP_PACKET pNalRbspPacket);
INT NalRbspPacket_getData(P_NAL_RBSP_PACKET pNalRbspPacket, PBYTE pBuf, INT iBytesToGet);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_H264_VIDEO_DECODER_H_