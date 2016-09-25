#include "MpegPesDemux.h"
#include "MpegPes.h"
#include <stdio.h>
#include <stdlib.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include "MpegPes.h"
#include <string2.h>
#include <assert.h>
#include	<List.h>
#include <ByteStream.h>
#ifdef _WIN32
#include <Windows.h>
#else
#endif

typedef struct tagMPEG_PES_PACKET_READER_ITEM
{
	BYTE maskPesStreamId;
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader;
}MPEG_PES_PACKET_READER_ITEM, *P_MPEG_PES_PACKET_READER_ITEM;

struct tagMPEG_PES_DEMUXER{
	MPEG_PES_HEADER headerMpegPes;
	BOOL bNeedSearchStartPrefix, bNeedReceivePesPacketHeader;
	BYTE abLocalBuf[MAX_MPEG_PES_PACKET_SIZE];	//本地数据缓冲区
	PBYTE apPesPayloadStart[4096];
	INT iValidPesPayloadStartCnt;
	UINT uiLocalBufSize;	//本地缓冲区有效数据量
	P_LIST pMpegPesPacketReaderList;
#ifdef RECORD_MPEG_PES_PACKET_DATA
	FILE * fpRecord;
#endif	//RECORD_MPEG_PES_PACKET_DATA
};

static INT MpegPesDemuxer_ReceiveSample(PVOID pCookie, P_MPEG_TS_PACKET_INFO pMpegTsPacketInfo);
static INT MpegPesDemuxer_Destroy(PVOID pCookie);
static INT MpegPesDemuxer_RegisterPacketReader(PVOID pCookie, BYTE maskPesStreamId,
											   P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
static INT MpegPesDemuxer_UnregisterPacketReader(PVOID pCookie,
	const P_MPEG_PES_PACKET_READER pMpegPesPacketReader);
static INT MpegPesDemuxer_ResetState(PVOID pCookie);

static INT AddPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, PBYTE pPesPacketStart);
static INT DelPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, const PBYTE pPesPacketStart);
static INT AdjustPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, INT iOffset);
static INT RemoveAllPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer);
static INT FindPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, const PBYTE pPesPacketStart, BOOL * pbFound);

static MPEG_TS_PACKET_READER_OPS g_MpegTsPacketReaderOpsForPesDemux = {
	MpegPesDemuxer_ReceiveSample,
	MpegPesDemuxer_Destroy,
	MpegPesDemuxer_RegisterPacketReader,
	MpegPesDemuxer_UnregisterPacketReader,
	MpegPesDemuxer_ResetState
};

P_MPEG_TS_PACKET_READER MpegPesDemux_CreateTsPacketReader()
{
	P_MPEG_TS_PACKET_READER pMpegTsPacketReader = NULL;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = NULL;

	do{
		pMpegPesDemuxer = malloc(sizeof(*pMpegPesDemuxer));
		if(NULL == pMpegPesDemuxer)
		{
			break;
		}

		memset(pMpegPesDemuxer, 0x00, sizeof(*pMpegPesDemuxer));
		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
		pMpegPesDemuxer->uiLocalBufSize = 0;
		pMpegPesDemuxer->pMpegPesPacketReaderList = List_new();
		pMpegPesDemuxer->iValidPesPayloadStartCnt = 0;
#ifdef RECORD_MPEG_PES_PACKET_DATA
		pMpegPesDemuxer->fpRecord = NULL;
#endif	//RECORD_MPEG_PES_PACKET_DATA
		pMpegTsPacketReader = MpegTsPacketReader_CreateWithCookie(pMpegPesDemuxer, &g_MpegTsPacketReaderOpsForPesDemux);

#ifdef RECORD_MPEG_PES_PACKET_DATA
		pMpegPesDemuxer->fpRecord = fopen("Record.ts", "wb");
		if(NULL == pMpegPesDemuxer->fpRecord)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
#endif	//RECORD_MPEG_PES_PACKET_DATA
	}while(FALSE);

	return pMpegTsPacketReader;
}

INT AddPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, PBYTE pPesPacketStart)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegPesDemuxer || NULL == pPesPacketStart)
		{
			break;
		}

		if(4 == sizeof(pMpegPesDemuxer->apPesPayloadStart))
		{
			PRINT_FILE_LINENO_BUG_STR;
		}

		pMpegPesDemuxer->iValidPesPayloadStartCnt++;
		if(sizeof(pMpegPesDemuxer->apPesPayloadStart) / sizeof(pMpegPesDemuxer->apPesPayloadStart[0]) < 
			pMpegPesDemuxer->iValidPesPayloadStartCnt)	//超出缓冲区
		{
			memmove(pMpegPesDemuxer->apPesPayloadStart, &(pMpegPesDemuxer->apPesPayloadStart[1]), 
				sizeof(pMpegPesDemuxer->apPesPayloadStart) - sizeof(pMpegPesDemuxer->apPesPayloadStart[0]));
			pMpegPesDemuxer->iValidPesPayloadStartCnt--;
		}

		pMpegPesDemuxer->apPesPayloadStart[pMpegPesDemuxer->iValidPesPayloadStartCnt - 1] = pPesPacketStart;
	} while (FALSE);

	return iOutRet;
}

INT DelPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, const PBYTE pPesPacketStart)
{
	INT iOutRet = ERROR_NOT_FOUND, iId = 0;

	do 
	{
		if(NULL == pMpegPesDemuxer || NULL == pPesPacketStart)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		for(iId=0; iId<pMpegPesDemuxer->iValidPesPayloadStartCnt; iId++)
		{
			if(pPesPacketStart == pMpegPesDemuxer->apPesPayloadStart[iId])
			{
				if(0 < (pMpegPesDemuxer->iValidPesPayloadStartCnt - iId - 1))
				{
					memmove(&(pMpegPesDemuxer->apPesPayloadStart[iId]), &(pMpegPesDemuxer->apPesPayloadStart[iId+1]),
						sizeof(pMpegPesDemuxer->apPesPayloadStart[0]) * (pMpegPesDemuxer->iValidPesPayloadStartCnt - iId - 1));
				}
				(pMpegPesDemuxer->iValidPesPayloadStartCnt)--;
				iOutRet = ERROR_SUCCESS;
				break;
			}
		}
	} while (FALSE);

	return iOutRet;
}

INT AdjustPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, INT iOffset)
{
	INT iOutRet = ERROR_SUCCESS, iId = 0;

	do 
	{
		if(NULL == pMpegPesDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(0 == iOffset)
		{
			break;
		}

		for(iId=0; iId<pMpegPesDemuxer->iValidPesPayloadStartCnt; iId++)
		{
			pMpegPesDemuxer->apPesPayloadStart[iId] += iOffset;
		}
	} while (FALSE);

	return iOutRet;
}

INT RemoveAllPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{	
		if(NULL == pMpegPesDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pMpegPesDemuxer->iValidPesPayloadStartCnt = 0;
	} while (FALSE);

	return iOutRet;
}

INT FindPesPacketStartPtr(P_MPEG_PES_DEMUXER pMpegPesDemuxer, const PBYTE pPesPacketStart, BOOL * pbFound)
{
	INT iOutRet = ERROR_SUCCESS, iId = 0;

	do 
	{
		if(NULL == pMpegPesDemuxer || NULL == pPesPacketStart || NULL == pbFound)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		*pbFound = FALSE;

		for(iId=0; iId<pMpegPesDemuxer->iValidPesPayloadStartCnt; iId++)
		{
			if(pPesPacketStart == pMpegPesDemuxer->apPesPayloadStart[iId])
			{
				*pbFound = TRUE;
				break;
			}
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegPesDemuxer_ReceiveSample(PVOID pCookie, P_MPEG_TS_PACKET_INFO pMpegTsPacketInfo)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	UINT uiDataScanOff = 0;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = (P_MPEG_PES_DEMUXER)pCookie;
	PBYTE pRet = NULL;
	MPEG_PES_PACKET_INFO infoMpegPesPacket;
	BOOL bHavePts = FALSE, bHaveDts = FALSE;

	if(NULL == pMpegPesDemuxer || NULL == pMpegTsPacketInfo)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		PRINT_FILE_LINENO_BUG_STR;
		goto RECEIVE_SAMPLE_END;
	}

	//printf("Pid=%d, PayloadSize=%d\n", pMpegTsPacketInfo->usPid, pMpegTsPacketInfo->iPayloadSize);

#ifdef RECORD_MPEG_PES_PACKET_DATA
	if(pMpegPesDemuxer->fpRecord)
	{
		iRet = fwrite(pMpegTsPacketInfo->pPayloadAddr, 1, pMpegTsPacketInfo->iPayloadSize,
			pMpegPesDemuxer->fpRecord);
		if(pMpegTsPacketInfo->iPayloadSize != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
	}
#endif	//RECORD_MPEG_PES_PACKET_DATA
		
	if(0 >= pMpegTsPacketInfo->iPayloadSize)
	{
		goto RECEIVE_SAMPLE_END;
	}

	if(pMpegPesDemuxer->bNeedSearchStartPrefix)	//需要PES同步头
	{
		if(pMpegPesDemuxer->bNeedReceivePesPacketHeader)	//需要首先接收PES包头
		{
			if(FALSE == pMpegTsPacketInfo->bPayloadUnitStartIndicator)	//非PES起始包
			{
				goto RECEIVE_SAMPLE_END;	//丢弃
			}
			else	//是PES起始包
			{
				pMpegPesDemuxer->bNeedReceivePesPacketHeader = FALSE;
			}
		}
	}

	//注入一个TS包
	if(sizeof(pMpegPesDemuxer->abLocalBuf) < pMpegPesDemuxer->uiLocalBufSize + pMpegTsPacketInfo->iPayloadSize)
	{
		//缓冲区溢出，丢弃。
//#ifdef	_DEBUG
		PRINT_FILE_LINENO_BUG_STR;
//#endif	//_DEBUG
		pMpegPesDemuxer->uiLocalBufSize = 0;
		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
		iRet = RemoveAllPesPacketStartPtr(pMpegPesDemuxer);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		goto RECEIVE_SAMPLE_END;
	}
	memmove(pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize, pMpegTsPacketInfo->pPayloadAddr,
		pMpegTsPacketInfo->iPayloadSize);
	if(pMpegTsPacketInfo->bPayloadUnitStartIndicator)
	{
		//记录PES包起始指针
		iRet = AddPesPacketStartPtr(pMpegPesDemuxer, pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}
	pMpegPesDemuxer->uiLocalBufSize += pMpegTsPacketInfo->iPayloadSize;

CONTINUE_PARSE_DATA:
	if(MAX_MPEG_PES_PACKET_SIZE - (4*1024) > pMpegPesDemuxer->uiLocalBufSize)	//本地缓冲区经注入数据后还是不够数
	{
		goto RECEIVE_SAMPLE_END;
	}

	if(0 != memcmp(pMpegPesDemuxer->abLocalBuf, MPEG_PES_PACKET_START_PREFIX, 3))
	{
		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
	}

	//够数了，开始分析
	if(pMpegPesDemuxer->bNeedSearchStartPrefix)	//找PES起始头
	{
		pRet = MemSearchMem(pMpegPesDemuxer->abLocalBuf, pMpegPesDemuxer->uiLocalBufSize, MPEG_PES_PACKET_START_PREFIX, 3);
		if(NULL == pRet)	//未找到PES起始头
		{
			pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
			pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
			pMpegPesDemuxer->uiLocalBufSize = 0;

			iRet = RemoveAllPesPacketStartPtr(pMpegPesDemuxer);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}

			goto RECEIVE_SAMPLE_END;
		}

		pMpegPesDemuxer->bNeedSearchStartPrefix = FALSE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = FALSE;

		//找到PES起始头，但有效数据太少。 
		if(MAX_MPEG_PES_PACKET_SIZE > (pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize - pRet))
		{
			memmove(pMpegPesDemuxer->abLocalBuf, pRet, pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize-pRet);
			pMpegPesDemuxer->uiLocalBufSize = pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize - pRet;

			iRet = AdjustPesPacketStartPtr(pMpegPesDemuxer, (pMpegPesDemuxer->abLocalBuf - pRet));
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}

			goto RECEIVE_SAMPLE_END;
		}

		if(pRet > pMpegPesDemuxer->abLocalBuf)	//已找到PES起始头，有效数据也够，但需要移动数据位置。
		{
			memmove(pMpegPesDemuxer->abLocalBuf, pRet, pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize - pRet);
			pMpegPesDemuxer->uiLocalBufSize = pMpegPesDemuxer->abLocalBuf+pMpegPesDemuxer->uiLocalBufSize - pRet;			

			iRet = AdjustPesPacketStartPtr(pMpegPesDemuxer, (pMpegPesDemuxer->abLocalBuf - pRet));
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
		}
	}

	//现在本地缓冲区中就是一个完整的PES包了
	//包头
	uiDataScanOff = 3;
	pMpegPesDemuxer->headerMpegPes.idStream = pMpegPesDemuxer->abLocalBuf[uiDataScanOff];
	uiDataScanOff += 1;
	/*
	What you underlined is the packet length of a video Packetized Elementary Stream. A length of 0000 indicates the packet 
	has no specific length (ie it ends at another system header or the end of the file). This is only allowed in video 
	PES within Transport Stream.
	*/
	pMpegPesDemuxer->headerMpegPes.uiPesPacketLength = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] << 8) |
		pMpegPesDemuxer->abLocalBuf[uiDataScanOff+1];
	if(0 == pMpegPesDemuxer->headerMpegPes.uiPesPacketLength)	//长度为0，需从下一个包头来计算长度。
	{
		PBYTE pDataStart = pMpegPesDemuxer->abLocalBuf + 5, pDataFinded = NULL;
		BOOL bFound = FALSE;

CONTINUE_FIND_NEXT_PES_START:
		//查找下一个包头
		pDataFinded = MemSearchMem(pDataStart, pMpegPesDemuxer->abLocalBuf + pMpegPesDemuxer->uiLocalBufSize - pDataStart,
			MPEG_PES_PACKET_START_PREFIX, 3);
		if(NULL == pDataFinded)	//未找到下一个PES包头
		{
			goto RECEIVE_SAMPLE_END;
		}
		iRet = FindPesPacketStartPtr(pMpegPesDemuxer, pDataFinded, &bFound);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		if(FALSE == bFound)
		{
			pDataStart = pDataFinded + 1;
			goto CONTINUE_FIND_NEXT_PES_START;
		}
		//计算包长
		pMpegPesDemuxer->headerMpegPes.uiPesPacketLength = pDataFinded - (pMpegPesDemuxer->abLocalBuf + 6);
	}
	uiDataScanOff += 2;

	if(pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0xC0 == 0x80)
	{
		PRINT_FILE_LINENO;
		printf("FIXME! mpeg2 PES packet\r\n");
	}
	else if(pMpegPesDemuxer->abLocalBuf[uiDataScanOff] == 0xFF)
	{
		PRINT_FILE_LINENO;
		printf("FIXME! MPEG-1 stuffing\r\n");
	}
	else if(pMpegPesDemuxer->abLocalBuf[uiDataScanOff] == 0x40)
	{
		PRINT_FILE_LINENO;
		printf("FIXME! MPEG-1 skipping\r\n");
	}

	//检查PES包的有效性
	if((uiDataScanOff + pMpegPesDemuxer->headerMpegPes.uiPesPacketLength) > pMpegPesDemuxer->uiLocalBufSize)	//溢出
	{
		//可能错误的包头，需要重新搜索包头
		memmove(pMpegPesDemuxer->abLocalBuf, pMpegPesDemuxer->abLocalBuf+1, pMpegPesDemuxer->uiLocalBufSize - 1);
		pMpegPesDemuxer->uiLocalBufSize -= 1;
		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;

		iRet = DelPesPacketStartPtr(pMpegPesDemuxer, pMpegPesDemuxer->abLocalBuf);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}

		goto CONTINUE_PARSE_DATA;
	}
	//基本流特有信息
	pMpegPesDemuxer->headerMpegPes.pes_scrambling_control = pMpegPesDemuxer->abLocalBuf[uiDataScanOff] >> 6;
	pMpegPesDemuxer->headerMpegPes.pes_priority = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x20) >> 5;
	pMpegPesDemuxer->headerMpegPes.data_position_indicator = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x10) >> 4;
	pMpegPesDemuxer->headerMpegPes.copyright = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x08) >> 3;
	pMpegPesDemuxer->headerMpegPes.original_or_copy = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x04) >> 2;
	uiDataScanOff += 1;
	pMpegPesDemuxer->headerMpegPes.PTS_or_DTS_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0xC0) >> 6;
	pMpegPesDemuxer->headerMpegPes.ESCR_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x80) >> 7;
	pMpegPesDemuxer->headerMpegPes.base_stream_rate_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x40) >> 6;
	pMpegPesDemuxer->headerMpegPes.DSM_trick_mode_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x20) >> 5;
	pMpegPesDemuxer->headerMpegPes.additional_info_copy = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x10) >> 4;
	pMpegPesDemuxer->headerMpegPes.PES_CRC_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x08) >> 3;
	pMpegPesDemuxer->headerMpegPes.PES_extension_flag = (pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x04) >> 2;
	uiDataScanOff += 1;
	//PES包头长 field
	pMpegPesDemuxer->headerMpegPes.ucPesPacketHeaderLen = pMpegPesDemuxer->abLocalBuf[uiDataScanOff];
	uiDataScanOff += 1;
	//extract data from PES packet header area
	pMpegPesDemuxer->headerMpegPes.uiPts = 0;
	bHavePts = pMpegPesDemuxer->headerMpegPes.PTS_or_DTS_flag & 0x02;
	bHaveDts = pMpegPesDemuxer->headerMpegPes.PTS_or_DTS_flag & 0x01;
	if(bHavePts)	//have PTS
	{
		UINT32 uiNext4Bytes = 0;
		pMpegPesDemuxer->headerMpegPes.uiPts |= 
			(((UINT64)((pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x08) >> 3)) << 32);	//high bit
		pMpegPesDemuxer->headerMpegPes.uiPts |= 
			(((UINT32)(pMpegPesDemuxer->abLocalBuf[uiDataScanOff] & 0x06)) << 29);
		uiDataScanOff += 1;
		uiNext4Bytes = 0;
		uiNext4Bytes = get4BytesFromBigEndianSrc(&(pMpegPesDemuxer->abLocalBuf[uiDataScanOff]));
		pMpegPesDemuxer->headerMpegPes.uiPts |= (uiNext4Bytes & 0xFFFE0000) >> 2;
		pMpegPesDemuxer->headerMpegPes.uiPts |= (uiNext4Bytes & 0x0000FFFE) >> 1;
		uiDataScanOff += 4;
	}
	if(bHaveDts)
	{
		pMpegPesDemuxer->headerMpegPes.uiDts = ((UINT64)(pMpegPesDemuxer->abLocalBuf[uiDataScanOff + 0] & 0x0E) << 29) |
			((pMpegPesDemuxer->abLocalBuf[uiDataScanOff + 1]) << 22) |
			((pMpegPesDemuxer->abLocalBuf[uiDataScanOff + 2] & 0xFE) << 14) |
			((pMpegPesDemuxer->abLocalBuf[uiDataScanOff + 3]) << 7) |
			((pMpegPesDemuxer->abLocalBuf[uiDataScanOff + 4]) << 1);
		uiDataScanOff += 5;
	}
	//print some info
	/*
	printf("Pes:PTS:%d, DTS:%d\n", (pMpegPesDemuxer->headerMpegPes.PTS_or_DTS_flag & 0x02)?1:0, 
		pMpegPesDemuxer->headerMpegPes.PTS_or_DTS_flag & 0x01);
	*/

	if(bHavePts)	//have PTS
	{
		char szPesStreamInfo[128];
		if((MPEG_PES_STREAM_ID_VIDEO_MASK & pMpegPesDemuxer->headerMpegPes.idStream) == MPEG_PES_STREAM_ID_VIDEO_MASK)
		{
			_snprintf(szPesStreamInfo, sizeof(szPesStreamInfo)/sizeof(szPesStreamInfo[0]), "Video<%d>", 
				pMpegPesDemuxer->headerMpegPes.idStream & 0x0F);
		}
		else if((MPEG_PES_STREAM_ID_AUDIO_MASK & pMpegPesDemuxer->headerMpegPes.idStream) == MPEG_PES_STREAM_ID_AUDIO_MASK)
		{
			_snprintf(szPesStreamInfo, sizeof(szPesStreamInfo)/sizeof(szPesStreamInfo[0]), "Audio<%d>", 
				pMpegPesDemuxer->headerMpegPes.idStream & 0x1F);
		}
		else
		{
			_snprintf(szPesStreamInfo, sizeof(szPesStreamInfo)/sizeof(szPesStreamInfo[0]), "Unknown");
		}
		szPesStreamInfo[sizeof(szPesStreamInfo)/sizeof(szPesStreamInfo[0]) - 1] = '\0';
		//PRINT_FILE_LINENO;
		printf("PES(%s): PTS=%llu ms", szPesStreamInfo, pMpegPesDemuxer->headerMpegPes.uiPts / 90U);
	}

	if(bHaveDts)	//have DTS
	{
		printf(", DTS=%llu ms", pMpegPesDemuxer->headerMpegPes.uiDts / 90U);
	}

	printf("\r\n");

	//PES包头信息 add offset
	uiDataScanOff += pMpegPesDemuxer->headerMpegPes.ucPesPacketHeaderLen;
	//构造PES包信息
	memset(&infoMpegPesPacket, 0x00, sizeof(infoMpegPesPacket));
	infoMpegPesPacket.idPesStream = pMpegPesDemuxer->headerMpegPes.idStream;
	infoMpegPesPacket.pPayloadAddr = pMpegPesDemuxer->abLocalBuf + uiDataScanOff;
	infoMpegPesPacket.iPayloadSize = pMpegPesDemuxer->headerMpegPes.uiPesPacketLength - (uiDataScanOff - 6);

	//printf("Pes payload size=%d\n", infoMpegPesPacket.iPayloadSize);

	//发送PES数据包到Reader
	if(pMpegPesDemuxer->pMpegPesPacketReaderList)
	{
		P_LIST pMpegPesPacketReaderList = NULL;
		POSITION pPos = NULL;
		P_MPEG_PES_PACKET_READER pMpegPesPacketReader = NULL;
		P_MPEG_PES_PACKET_READER_ITEM pMpegPesPacketReaderItem = NULL;

		pMpegPesPacketReaderList = pMpegPesDemuxer->pMpegPesPacketReaderList;
		pPos = List_GetHeadPosition(pMpegPesPacketReaderList);
		while(pPos)
		{
			pMpegPesPacketReaderItem = List_GetNext(pMpegPesPacketReaderList, &pPos);

			if(pMpegPesPacketReaderItem && 
				(infoMpegPesPacket.idPesStream & pMpegPesPacketReaderItem->maskPesStreamId) == 
				pMpegPesPacketReaderItem->maskPesStreamId)
			{
				pMpegPesPacketReader = pMpegPesPacketReaderItem->pMpegPesPacketReader;
				iRet = MpegPesPacketReader_ReceiveSample(pMpegPesPacketReader, &infoMpegPesPacket);
			}
		}
	}	

	uiDataScanOff += infoMpegPesPacket.iPayloadSize;
	//废弃已分析的PES包
	if(uiDataScanOff == pMpegPesDemuxer->uiLocalBufSize)	//整包
	{
		iRet = RemoveAllPesPacketStartPtr(pMpegPesDemuxer);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}

		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
		pMpegPesDemuxer->uiLocalBufSize = 0;
	}
	else if(uiDataScanOff < pMpegPesDemuxer->uiLocalBufSize)	//余下一个PES包的数据
	{
		iRet = DelPesPacketStartPtr(pMpegPesDemuxer, pMpegPesDemuxer->abLocalBuf);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		memmove(pMpegPesDemuxer->abLocalBuf, pMpegPesDemuxer->abLocalBuf+uiDataScanOff, pMpegPesDemuxer->uiLocalBufSize -
			uiDataScanOff);
		pMpegPesDemuxer->uiLocalBufSize = pMpegPesDemuxer->uiLocalBufSize - uiDataScanOff;

		iRet = AdjustPesPacketStartPtr(pMpegPesDemuxer, -((INT)uiDataScanOff));
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}

		goto CONTINUE_PARSE_DATA;
	}
	else
	{
		PRINT_FILE_LINENO_BUG_STR;
	}

RECEIVE_SAMPLE_END:

	return iOutRet;
}

INT MpegPesDemuxer_Destroy(PVOID pCookie)
{
	INT iOutRet = ERROR_SUCCESS;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = (P_MPEG_PES_DEMUXER)pCookie;
	P_LIST pMpegPesPacketReaderList = NULL;
	P_MPEG_PES_PACKET_READER_ITEM pMpegPesPacketReaderItem = NULL;
	POSITION pPos = NULL;

	do{
		if(NULL == pMpegPesDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		
		if(pMpegPesDemuxer->pMpegPesPacketReaderList)
		{
			//destroy MpegTsPacketReader list
			pMpegPesPacketReaderList = pMpegPesDemuxer->pMpegPesPacketReaderList;
			pPos = List_GetHeadPosition(pMpegPesPacketReaderList);
			while(pPos)
			{
				pMpegPesPacketReaderItem = List_GetNext(pMpegPesPacketReaderList, &pPos);
				if(pMpegPesPacketReaderItem)
				{
					free(pMpegPesPacketReaderItem);
					pMpegPesPacketReaderItem = NULL;
				}
			}
			List_RemoveAll(pMpegPesPacketReaderList);
			List_delete(pMpegPesDemuxer->pMpegPesPacketReaderList);
			pMpegPesDemuxer->pMpegPesPacketReaderList = NULL;
		}

#ifdef RECORD_MPEG_PES_PACKET_DATA
		if(pMpegPesDemuxer->fpRecord)
		{
			iRet = fclose(pMpegPesDemuxer->fpRecord);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pMpegPesDemuxer->fpRecord = NULL;
		}
#endif	//RECORD_MPEG_PES_PACKET_DATA

		free(pMpegPesDemuxer);
		pMpegPesDemuxer = NULL;
	}while(FALSE);

	return iOutRet;
}

INT MpegPesDemuxer_RegisterPacketReader(PVOID pCookie, BYTE maskPesStreamId,
										P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;
	
	P_LIST pMpegPesPacketReaderList = NULL;
	P_MPEG_PES_PACKET_READER_ITEM pMpegPesPacketReaderItem = NULL;
	POSITION pPos = NULL;
	BOOL bFound = FALSE;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = NULL;

	pMpegPesDemuxer = pCookie;

	do 
	{
		if(NULL == pMpegPesDemuxer || NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(NULL == pMpegPesDemuxer->pMpegPesPacketReaderList)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		//first, check whether already registered
		bFound = FALSE;
		pMpegPesPacketReaderList = pMpegPesDemuxer->pMpegPesPacketReaderList;
		pPos = List_GetHeadPosition(pMpegPesPacketReaderList);
		while(pPos)
		{
			pMpegPesPacketReaderItem = List_GetNext(pMpegPesPacketReaderList, &pPos);
			if(maskPesStreamId == pMpegPesPacketReaderItem->maskPesStreamId && 
				pMpegPesPacketReader == pMpegPesPacketReaderItem->pMpegPesPacketReader)
			{
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			break;
		}

		pMpegPesPacketReaderItem = malloc(sizeof(*pMpegPesPacketReaderItem));
		if(NULL == pMpegPesPacketReaderItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		memset(pMpegPesPacketReaderItem, 0x00, sizeof(*pMpegPesPacketReaderItem));
		pMpegPesPacketReaderItem->maskPesStreamId = maskPesStreamId;
		pMpegPesPacketReaderItem->pMpegPesPacketReader = pMpegPesPacketReader;

		pPos = List_AddTail(pMpegPesPacketReaderList, pMpegPesPacketReaderItem);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegPesDemuxer_UnregisterPacketReader(PVOID pCookie,
	const P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;
	P_LIST pMpegPesPacketReaderList = NULL;
	P_MPEG_PES_PACKET_READER_ITEM pMpegPesPacketReaderItem = NULL;
	POSITION pPos = NULL, pCurPos = NULL;
	BOOL bFound = FALSE;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = pCookie;

	do 
	{
		if(NULL == pMpegPesDemuxer || NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		if(pMpegPesDemuxer->pMpegPesPacketReaderList)
		{
			pMpegPesPacketReaderList = pMpegPesDemuxer->pMpegPesPacketReaderList;
			pPos = List_GetHeadPosition(pMpegPesPacketReaderList);
			while(pPos)
			{
				pCurPos = pPos;
				pMpegPesPacketReaderItem = List_GetNext(pMpegPesPacketReaderList, &pPos);
				if(pMpegPesPacketReader == pMpegPesPacketReaderItem->pMpegPesPacketReader)
				{	
					List_RemoveAt(pMpegPesPacketReaderList, pCurPos);
					free(pMpegPesPacketReaderItem);
					pMpegPesPacketReaderItem = NULL;
					bFound = TRUE;
				}
			}
		}

		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	} while (FALSE);
	
	return iOutRet;
}

INT MpegPesDemuxer_ResetState(PVOID pCookie)
{
	INT iOutRet = ERROR_SUCCESS;
	P_MPEG_PES_DEMUXER pMpegPesDemuxer = (P_MPEG_PES_DEMUXER)pCookie;

	do 
	{
		if(NULL == pMpegPesDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pMpegPesDemuxer->bNeedReceivePesPacketHeader = TRUE;
		pMpegPesDemuxer->bNeedSearchStartPrefix = TRUE;
		pMpegPesDemuxer->uiLocalBufSize = 0;
		pMpegPesDemuxer->iValidPesPayloadStartCnt = 0;
		memset(&(pMpegPesDemuxer->headerMpegPes), 0x00, sizeof(pMpegPesDemuxer->headerMpegPes));
	} while (FALSE);

	return iOutRet;
}