#ifndef	_MPEG_TS_PACKET_READER_H_
#define _MPEG_TS_PACKET_READER_H_

#include <BaseTypeDef.h>
#include "MpegTs.h"
#include "MpegPesPacketReader.h"

typedef struct _MPEG_TS_PACKET_READER MPEG_TS_PACKET_READER;
typedef MPEG_TS_PACKET_READER * P_MPEG_TS_PACKET_READER;

#ifdef __cplusplus
extern "C"
{
#endif	//__cplusplus

#define MPEG_PES_STREAM_ID_VIDEO_MASK				0xE0
#define MPEG_PES_STREAM_ID_AUDIO_MASK				0xC0

typedef INT (*PFN_MpegTsPacketReader_ReceiveSample)(PVOID pCookie, P_MPEG_TS_PACKET_INFO pMpegTsPacketInfo);
typedef INT (*PFN_MpegTsPacketReader_Destroy)(PVOID pCookie);
typedef INT (*PFN_MpegTsPacketReader_RegisterPacketReader)(PVOID pCookie, BYTE maskPesStreamId,
														   P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
typedef INT (*PFN_MpegTsPacketReader_UnregisterPacketReader)(PVOID pCookie, 
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
typedef INT (*PFN_MpegTsPacketReader_ResetState)(PVOID pCookie);

//仅仅是接口，并非基类。
typedef struct tagMPEG_TS_PACKET_READER_OPS{
	PFN_MpegTsPacketReader_ReceiveSample pfnReceiveSample;
	PFN_MpegTsPacketReader_Destroy pfnDestroy;
	PFN_MpegTsPacketReader_RegisterPacketReader pfnRegisterPacketReader;
	PFN_MpegTsPacketReader_UnregisterPacketReader pfnUnregisterPacketReader;
	PFN_MpegTsPacketReader_ResetState pfnResetState;
}MPEG_TS_PACKET_READER_OPS, *P_MPEG_TS_PACKET_READER_OPS;

P_MPEG_TS_PACKET_READER 
MpegTsPacketReader_CreateWithCookie(PVOID pCookie, 
									P_MPEG_TS_PACKET_READER_OPS pMpegTsPacketReaderOps);
INT MpegTsPacketReader_ReceiveSample(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, 
									 P_MPEG_TS_PACKET_INFO pMpegTsPacketInfo);
INT MpegTsPacketReader_RegisterPacketReader(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, BYTE maskPesStreamId,
									   P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
INT MpegTsPacketReader_UnregisterPacketReader(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, 
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
INT MpegTsPacketReader_ResetState(P_MPEG_TS_PACKET_READER pMpegTsPacketReader);
INT MpegTsPacketReader_Destroy(P_MPEG_TS_PACKET_READER pMpegTsPacketReader);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_MPEG_TS_PACKET_READER_H_