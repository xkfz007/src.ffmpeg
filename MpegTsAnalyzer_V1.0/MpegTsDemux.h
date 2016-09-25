#ifndef	_MPEG_TS_DEMUX_H_
#define	_MPEG_TS_DEMUX_H_

#include <BaseTypeDef.h>
#include "MpegTs.h"
#include "MpegTsPacketReader.h"

#define MPEG_TS_SYNC_CONTINUITY_COUNT		5	

typedef struct _MPEG_TS_DEMUXER MPEG_TS_DEMUXER;
typedef MPEG_TS_DEMUXER * P_MPEG_TS_DEMUXER;

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

INT ParseMpegTsHeader(P_MPEG_TS_PACKET_HEADER pMpegTsPacketHeader, PBYTE pHeaderData, INT iHeaderSize);

P_MPEG_TS_DEMUXER MpegTsDemuxer_Create();
INT MpegTsDemuxer_ResetState(P_MPEG_TS_DEMUXER pMpegTsDemuxer);
INT MpegTsDemuxer_ReceiveSample(P_MPEG_TS_DEMUXER pMpegTsDemuxer, const PBYTE pBuf, const INT iSize);
INT MpegTsDemuxer_RegisterPacketReader(P_MPEG_TS_DEMUXER pMpegTsDemuxer, unsigned short int usPid,
									   P_MPEG_TS_PACKET_READER pMpegTsPacketReader);
INT MpegTsDemuxer_UnregisterPacketReader(P_MPEG_TS_DEMUXER pMpegTsDemuxer, 
	const P_MPEG_TS_PACKET_READER pMpegTsPacketReader);
INT MpegTsDemuxer_Destroy(P_MPEG_TS_DEMUXER pMpegTsDemuxer);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_MPEG_TS_DEMUX_H_