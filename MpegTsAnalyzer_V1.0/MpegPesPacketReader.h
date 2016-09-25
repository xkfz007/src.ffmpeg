#ifndef _MPEG_PES_PACKET_READER_H_
#define _MPEG_PES_PACKET_READER_H_

#include <BaseTypeDef.h>
#include "MpegPes.h"

#define MPEG_PES_PACKET_READER_IOCTL_CODE_BASE			4700
#define GET_H264_NAL_SPS_IS_VALID						(MPEG_PES_PACKET_READER_IOCTL_CODE_BASE + 1)
#define GET_H264_FRAME_FIELD_INFO						(MPEG_PES_PACKET_READER_IOCTL_CODE_BASE + 2)

typedef struct _MPEG_PES_PACKET_READER MPEG_PES_PACKET_READER;
typedef MPEG_PES_PACKET_READER * P_MPEG_PES_PACKET_READER;

typedef struct
{
	BOOL bProgressive;
	BOOL bH264Mbaff;
}H264_FRAME_FIELD_INFO, *P_H264_FRAME_FIELD_INFO;

#ifdef __cplusplus
extern "C"
{
#endif	//__cplusplus

typedef INT (*PFN_MpegPesPacketReader_ReceiveSample)(PVOID pCookie, P_MPEG_PES_PACKET_INFO pMpegPesPacketInfo);
typedef INT (*PFN_MpegPesPacketReader_ResetState)(PVOID pCookie);
typedef INT (*PFN_MpegPesPacketReader_Destroy)(PVOID pCookie);
typedef INT (*PFN_MpegPesPacketReader_IoControl)(PVOID pCookie, UINT uiIoCtrlCode, PBYTE pControlData, INT iDataSize);

//仅仅是接口，并非基类。
typedef struct tagMPEG_PES_PACKET_READER_OPS{
	PFN_MpegPesPacketReader_ReceiveSample pfnReceiveSample;
	PFN_MpegPesPacketReader_ResetState pfnResetState;
	PFN_MpegPesPacketReader_Destroy pfnDestroy;
	PFN_MpegPesPacketReader_IoControl pfnIoControl;
}MPEG_PES_PACKET_READER_OPS, *P_MPEG_PES_PACKET_READER_OPS;

P_MPEG_PES_PACKET_READER 
MpegPesPacketReader_CreateWithCookie(PVOID pCookie, P_MPEG_PES_PACKET_READER_OPS pMpegPesPacketReaderOps);
INT MpegPesPacketReader_ReceiveSample(P_MPEG_PES_PACKET_READER pMpegPesPacketReader, 
	P_MPEG_PES_PACKET_INFO pMpegPesPacketInfo);
INT MpegPesPacketReader_ResetState(P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
INT MpegPesPacketReader_Destroy(P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
INT MpegPesPacketReader_IoControl(P_MPEG_PES_PACKET_READER pMpegPesPacketReader, UINT uiIoCtrlCode, PBYTE pControlData, 
								  INT iDataSize);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_MPEG_PES_PACKET_READER_H_