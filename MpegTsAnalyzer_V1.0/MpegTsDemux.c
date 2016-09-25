#include "MpegTsDemux.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <BaseErrDef.h>
#include "MpegTs.h"
#include <ErrPrintHelper.h>
#ifdef	_WIN32
#include <assert.h>
#endif	//_WIN32
#include "MpegTsPacketReader.h"
#include <string2.h>
#include <memory.h>
#include	<List.h>
#ifdef _WIN32
  #include <Windows.h>
#else
#endif

typedef struct tagMPEG_TS_PACKET_READER_ITEM
{
	unsigned short int usPid;
	P_MPEG_TS_PACKET_READER pMpegTsPacketReader;
}MPEG_TS_PACKET_READER_ITEM, *P_MPEG_TS_PACKET_READER_ITEM;

struct _MPEG_TS_DEMUXER{
	BOOL bNeedSearchSync;
	BYTE abBufData[4*1024];
	INT iBufSize;
	P_LIST pMpegTsPacketReaderList;
};

INT ParseMpegTsHeader(P_MPEG_TS_PACKET_HEADER pMpegTsPacketHeader, PBYTE pHeaderData, INT iHeaderSize)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pMpegTsPacketHeader || NULL == pHeaderData || 4 > iHeaderSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pMpegTsPacketHeader->sync_byte = pHeaderData[0];
		pMpegTsPacketHeader->transport_error_indicator = pHeaderData[1] >> 7;
		pMpegTsPacketHeader->payload_unit_start_indicator = (pHeaderData[1] >> 6) & 0x01;
		pMpegTsPacketHeader->transport_priority = (pHeaderData[1] >> 5) & 0x01;
		pMpegTsPacketHeader->pid = ((pHeaderData[1] & 0x1F) << 8) | pHeaderData[2];
		pMpegTsPacketHeader->transport_scrambling_control = pHeaderData[3] >> 6;
		pMpegTsPacketHeader->adaption_field_control = (pHeaderData[3] & 0x20) >> 5;
		pMpegTsPacketHeader->payload_field_control = (pHeaderData[3] & 0x10) >> 4;
		pMpegTsPacketHeader->continuity_counter = pHeaderData[3] & 0x0F;
	}while(FALSE);

	return iOutRet;
}

P_MPEG_TS_DEMUXER MpegTsDemuxer_Create()
{
	P_MPEG_TS_DEMUXER pMpegTsDemuxer = NULL;
	do 
	{
		pMpegTsDemuxer = malloc(sizeof(MPEG_TS_DEMUXER));
		if(NULL == pMpegTsDemuxer)
		{			
			break;
		}
		//init
		memset(pMpegTsDemuxer, 0x00, sizeof(*pMpegTsDemuxer));
		pMpegTsDemuxer->bNeedSearchSync = TRUE;
		pMpegTsDemuxer->iBufSize = 0;
		pMpegTsDemuxer->pMpegTsPacketReaderList = List_new();
	} while(FALSE);

	return pMpegTsDemuxer;
}

INT MpegTsDemuxer_ResetState(P_MPEG_TS_DEMUXER pMpegTsDemuxer)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pMpegTsDemuxer->bNeedSearchSync = TRUE;
		pMpegTsDemuxer->iBufSize = 0;
	} while (FALSE);

	return iOutRet;
}

INT MpegTsDemuxer_ReceiveSample(P_MPEG_TS_DEMUXER pMpegTsDemuxer, const PBYTE pBuf, const INT iSize)
{
	INT iOutRet = ERROR_SUCCESS, iMaximumBytesForAnalyze = 0, iSrcDataOffset = 0, iBytesNeedCopy = 0,
		iAvailableBytesLeft = 0, iRet = 0;
	PBYTE pByte = NULL;
	MPEG_TS_PACKET_HEADER headerMpegTsPacket;
	MPEG_TS_PACKET_INFO infoMpegTsPacket;
	PBYTE pMpegTsPacketHeaderDataPtr = NULL;
	
	if(NULL == pMpegTsDemuxer || NULL == pBuf || 0 >= iSize)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto RECEIVE_SAMPLE_END;
	}

PARSE_NEXT_DATA_BLOCK:
	//计算为分析需要的最小字节数
	if(pMpegTsDemuxer->bNeedSearchSync)
	{
		iMaximumBytesForAnalyze = MPEG_TS_PACKET_SIZE * (MPEG_TS_SYNC_CONTINUITY_COUNT+1);
	}
	else
	{
		iMaximumBytesForAnalyze = MPEG_TS_PACKET_SIZE;
	}

	//调节缓冲区数据量
	if(0 < pMpegTsDemuxer->iBufSize)	//预读缓冲区有数据
	{
		if(iMaximumBytesForAnalyze > pMpegTsDemuxer->iBufSize)	//需要往缓冲区注入数据
		{
			if(0 >= iSize - iSrcDataOffset)	//无更多数据可以分析
			{
				goto RECEIVE_SAMPLE_END;
			}

			iBytesNeedCopy = iMaximumBytesForAnalyze - pMpegTsDemuxer->iBufSize;	//期望的拷贝数
			if(iBytesNeedCopy > iSize - iSrcDataOffset)
			{
				iBytesNeedCopy = iSize - iSrcDataOffset;
			}
			if(sizeof(pMpegTsDemuxer->abBufData) < pMpegTsDemuxer->iBufSize+iBytesNeedCopy)	//缓冲区不够
			{
				iOutRet = ERROR_NOT_ENOUGH_BUFFER;
				goto RECEIVE_SAMPLE_END;
			}
			memcpy(pMpegTsDemuxer->abBufData+pMpegTsDemuxer->iBufSize, pBuf+iSrcDataOffset, iBytesNeedCopy);
			pMpegTsDemuxer->iBufSize += iBytesNeedCopy;
			iSrcDataOffset += iBytesNeedCopy;
		}
	}
	else	//预读缓冲区无数据
	{
		if(iMaximumBytesForAnalyze > iSize - iSrcDataOffset)	//无足够的可分析数据,先缓存起来。
		{
			iBytesNeedCopy = iSize - iSrcDataOffset;	//期望的拷贝数
			if(sizeof(pMpegTsDemuxer->abBufData) < pMpegTsDemuxer->iBufSize+iBytesNeedCopy)	//缓冲区不够
			{
				iOutRet = ERROR_NOT_ENOUGH_BUFFER;
#ifdef	_WIN32
				assert(FALSE);
#endif
				goto RECEIVE_SAMPLE_END;
			}
			if(0 == iBytesNeedCopy)	//无数据可拷贝
			{
				goto RECEIVE_SAMPLE_END;
			}
			memcpy(pMpegTsDemuxer->abBufData+pMpegTsDemuxer->iBufSize, pBuf+iSrcDataOffset, iBytesNeedCopy);
			pMpegTsDemuxer->iBufSize += iBytesNeedCopy;
			iSrcDataOffset += iBytesNeedCopy;
			goto RECEIVE_SAMPLE_END;
		}
	}

	//开始分析
	if(pMpegTsDemuxer->bNeedSearchSync)	//检查同步字节
	{
		//搜索同步byte
		if(0 < pMpegTsDemuxer->iBufSize)	//先在缓冲区中搜
		{
			INT iSyncCnt = 0;

			pByte = memchr(pMpegTsDemuxer->abBufData, MPEG_TS_SYNC_BYTE_VAL, pMpegTsDemuxer->iBufSize);
			if(NULL == pByte)	//没有搜索到同步字
			{
				pMpegTsDemuxer->iBufSize = 0;	//丢弃无效数据
				goto PARSE_NEXT_DATA_BLOCK;
			}

			//检查同步位置是否正确
			iSyncCnt = 1;
			while(MPEG_TS_SYNC_CONTINUITY_COUNT > iSyncCnt)
			{
				if(pMpegTsDemuxer->iBufSize - 1 < MPEG_TS_PACKET_SIZE * iSyncCnt)	//数据不够啦
				{
					//计算剩余有效字节数
					iAvailableBytesLeft = pMpegTsDemuxer->iBufSize - (pByte - pMpegTsDemuxer->abBufData);
					memmove(pMpegTsDemuxer->abBufData, pByte, iAvailableBytesLeft);
					pMpegTsDemuxer->iBufSize = iAvailableBytesLeft;
					pMpegTsDemuxer->bNeedSearchSync = TRUE;
					goto PARSE_NEXT_DATA_BLOCK;
				}

				if(MPEG_TS_SYNC_BYTE_VAL != *(pByte + MPEG_TS_PACKET_SIZE * iSyncCnt))	//假定的同步位不正确
				{
					//计算剩余有效字节数
					iAvailableBytesLeft = pMpegTsDemuxer->iBufSize - (pByte - pMpegTsDemuxer->abBufData) - 1;
					memmove(pMpegTsDemuxer->abBufData, pByte + 1, iAvailableBytesLeft);
					pMpegTsDemuxer->iBufSize = iAvailableBytesLeft;
					pMpegTsDemuxer->bNeedSearchSync = TRUE;
					goto PARSE_NEXT_DATA_BLOCK;
				}

				iSyncCnt++;
			}

			//搜索到同步字
			//计算剩余有效字节数
			iAvailableBytesLeft = pMpegTsDemuxer->iBufSize - (pByte - pMpegTsDemuxer->abBufData);
			memmove(pMpegTsDemuxer->abBufData, pByte, iAvailableBytesLeft);
			pMpegTsDemuxer->iBufSize = iAvailableBytesLeft;
			pMpegTsDemuxer->bNeedSearchSync = FALSE;
		}
		else	//缓冲区无数据，需在传入缓冲区中搜索。
		{
			INT iSyncCnt = 0;

			pByte = memchr(pBuf+iSrcDataOffset, MPEG_TS_SYNC_BYTE_VAL, iSize-iSrcDataOffset);
			if(NULL == pByte)	//没有搜索到同步字
			{
				iSrcDataOffset = iSize;	//丢弃无效数据
				goto PARSE_NEXT_DATA_BLOCK;
			}

			//检查同步位置是否正确
			iSyncCnt = 1;
			while(MPEG_TS_SYNC_CONTINUITY_COUNT > iSyncCnt)
			{
				if(iSize - iSrcDataOffset - 1 < MPEG_TS_PACKET_SIZE * iSyncCnt)	//数据不够啦
				{
					//计算剩余有效字节数
					iSrcDataOffset = pByte - pBuf;
					pMpegTsDemuxer->bNeedSearchSync = TRUE;
					goto PARSE_NEXT_DATA_BLOCK;
				}

				if(MPEG_TS_SYNC_BYTE_VAL != *(pByte + MPEG_TS_PACKET_SIZE * iSyncCnt))	//假定的同步位不正确
				{
					//计算剩余有效字节数
					iSrcDataOffset = pByte - pBuf + 1;
					pMpegTsDemuxer->bNeedSearchSync = TRUE;
					goto PARSE_NEXT_DATA_BLOCK;
				}

				iSyncCnt++;
			}

			//搜索到同步字
			//计算剩余有效字节数
			iSrcDataOffset = pByte - pBuf;
			pMpegTsDemuxer->bNeedSearchSync = FALSE;
		}
	}
	else
	{
		//同步字复核
		if(0 < pMpegTsDemuxer->iBufSize)
		{
			if(MPEG_TS_SYNC_BYTE_VAL != pMpegTsDemuxer->abBufData[0])
			{
				pMpegTsDemuxer->bNeedSearchSync = TRUE;
				goto PARSE_NEXT_DATA_BLOCK;
			}
		}
		else
		{
			if(MPEG_TS_SYNC_BYTE_VAL != pBuf[iSrcDataOffset])
			{
				pMpegTsDemuxer->bNeedSearchSync = TRUE;
				goto PARSE_NEXT_DATA_BLOCK;
			}
		}
	}

	//数据已同步，可以分析包了。
	memset(&headerMpegTsPacket, 0x00, sizeof(headerMpegTsPacket));
	memset(&infoMpegTsPacket, 0x00, sizeof(infoMpegTsPacket));
	if(0 < pMpegTsDemuxer->iBufSize)
	{
		pMpegTsPacketHeaderDataPtr = &(pMpegTsDemuxer->abBufData[0]);
	}
	else
	{
		pMpegTsPacketHeaderDataPtr = pBuf+iSrcDataOffset;
	}

	iRet = ParseMpegTsHeader(&headerMpegTsPacket, pMpegTsPacketHeaderDataPtr, 4);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_BUG_STR;
	}

	infoMpegTsPacket.usPid = headerMpegTsPacket.pid;
	infoMpegTsPacket.pPayloadAddr = NULL;
	infoMpegTsPacket.iPayloadSize = 0;
	infoMpegTsPacket.bPayloadUnitStartIndicator = headerMpegTsPacket.payload_unit_start_indicator;
	infoMpegTsPacket.bPcrBaseAvailable = FALSE;

	if(headerMpegTsPacket.payload_field_control)	//有负载数据
	{
		if(headerMpegTsPacket.payload_unit_start_indicator)	//有负载单元起始指示符
		{
			if(headerMpegTsPacket.adaption_field_control)	//有自适应控制字段
			{
				UINT8 uiAdaptionAreaLen = pMpegTsPacketHeaderDataPtr[4];
				//get the PCR				
				if((7U <= uiAdaptionAreaLen) && (pMpegTsPacketHeaderDataPtr[5] & 0x10))
				{
					infoMpegTsPacket.uiPcrBase = (((UINT64)(pMpegTsPacketHeaderDataPtr[6])) << 25) |
						(((UINT64)pMpegTsPacketHeaderDataPtr[7]) << 17) | (((UINT64)pMpegTsPacketHeaderDataPtr[8]) << 9) |
						(((UINT64)pMpegTsPacketHeaderDataPtr[9]) << 1) | (((UINT64)pMpegTsPacketHeaderDataPtr[10]) >> 7);
					infoMpegTsPacket.bPcrBaseAvailable = TRUE;
					printf("PCR=%llu ms\r\n", infoMpegTsPacket.uiPcrBase/90U);
				}

				infoMpegTsPacket.pPayloadAddr = pMpegTsPacketHeaderDataPtr + (5U + pMpegTsPacketHeaderDataPtr[4]);
				infoMpegTsPacket.iPayloadSize = MPEG_TS_PACKET_SIZE - (5U + pMpegTsPacketHeaderDataPtr[4]);
				//检查负载参数合法性
				if(0 >= infoMpegTsPacket.iPayloadSize)
				{
					infoMpegTsPacket.pPayloadAddr = NULL;
					infoMpegTsPacket.iPayloadSize = 0;
#ifdef _WIN32
				assert(FALSE);
#endif
				}	
			}
			else	//无自适应控制字段
			{				
				infoMpegTsPacket.pPayloadAddr = pMpegTsPacketHeaderDataPtr + 4;
				infoMpegTsPacket.iPayloadSize = MPEG_TS_PACKET_SIZE - 4;
			}
		}
		else	//无负载单元起始指示符
		{
			if(headerMpegTsPacket.adaption_field_control)	//有自适应控制字段
			{
				UINT8 uiAdaptionAreaLen = pMpegTsPacketHeaderDataPtr[4];
				//get the PCR				
				if((7U <= uiAdaptionAreaLen) && (pMpegTsPacketHeaderDataPtr[5] & 0x10))
				{
					infoMpegTsPacket.uiPcrBase = (((UINT64)(pMpegTsPacketHeaderDataPtr[6])) << 25) |
						(((UINT64)pMpegTsPacketHeaderDataPtr[7]) << 17) | (((UINT64)pMpegTsPacketHeaderDataPtr[8]) << 9) |
						(((UINT64)pMpegTsPacketHeaderDataPtr[9]) << 1) | (((UINT64)pMpegTsPacketHeaderDataPtr[10]) >> 7);
					infoMpegTsPacket.bPcrBaseAvailable = TRUE;
				}

				infoMpegTsPacket.pPayloadAddr = pMpegTsPacketHeaderDataPtr + (5U + pMpegTsPacketHeaderDataPtr[4]);
				infoMpegTsPacket.iPayloadSize = MPEG_TS_PACKET_SIZE - (5U + pMpegTsPacketHeaderDataPtr[4]);
				//检查负载参数合法性
				if(0 >= infoMpegTsPacket.iPayloadSize)
				{
					infoMpegTsPacket.pPayloadAddr = NULL;
					infoMpegTsPacket.iPayloadSize = 0;
#ifdef _WIN32
					assert(FALSE);
#endif
				}
			}
			else	//无自适应控制字段
			{
				infoMpegTsPacket.pPayloadAddr = pMpegTsPacketHeaderDataPtr + MPEG_TS_BASE_PACKET_HEADER_SIZE;
				infoMpegTsPacket.iPayloadSize = MPEG_TS_PACKET_SIZE - MPEG_TS_BASE_PACKET_HEADER_SIZE;
			}
		}
	}

	//PRINT_FILE_LINENO;
	/*
	printf("transport_error_indicator=%d, payload_unit_start_indicator=%d, transport_priority=%d,"
		"pid=%d, adaption_field_control=%d, payload_field_control=%d, continuity_counter=%d\n",
		headerMpegTsPacket.transport_error_indicator, headerMpegTsPacket.payload_unit_start_indicator,
		headerMpegTsPacket.transport_priority, headerMpegTsPacket.pid, headerMpegTsPacket.adaption_field_control,
		headerMpegTsPacket.payload_field_control, headerMpegTsPacket.continuity_counter);
	printf("payload_start_indicator=%d,"
		"pid=%d, adaption_field=%d, payload_field=%d, continuity_counter=%d\n",
		headerMpegTsPacket.payload_unit_start_indicator,
		headerMpegTsPacket.pid, headerMpegTsPacket.adaption_field_control,
		headerMpegTsPacket.payload_field_control, headerMpegTsPacket.continuity_counter);
	*/
	//printf("pid=%d, PayloadSize=%d\n", infoMpegTsPacket.usPid, infoMpegTsPacket.iPayloadSize);

	/*
	if(0 == memcmp(infoMpegTsPacket.pPayloadAddr, "\x00\x00\x01\xE0", 4))
	{
		PRINT_FILE_LINENO;
		printf("\n");
	}
	*/

	//发送数据包到Reader
	if(pMpegTsDemuxer->pMpegTsPacketReaderList)	//have readers
	{
		P_LIST pMpegTsPacketReaderList = NULL;
		P_MPEG_TS_PACKET_READER_ITEM pMpegTsPacketReaderItem = NULL;
		POSITION pPos = NULL;

		pMpegTsPacketReaderList = pMpegTsDemuxer->pMpegTsPacketReaderList;
		pPos = List_GetHeadPosition(pMpegTsPacketReaderList);
		while(pPos)
		{
			pMpegTsPacketReaderItem = List_GetNext(pMpegTsPacketReaderList, &pPos);

			if(pMpegTsPacketReaderItem && infoMpegTsPacket.usPid == pMpegTsPacketReaderItem->usPid)
			{
				iRet = MpegTsPacketReader_ReceiveSample(pMpegTsPacketReaderItem->pMpegTsPacketReader,
					&infoMpegTsPacket);
			}
		}
	}

	//行进到下一个TS数据包
	if(0 < pMpegTsDemuxer->iBufSize)
	{
		pMpegTsDemuxer->iBufSize -= MPEG_TS_PACKET_SIZE;
		if(0 < pMpegTsDemuxer->iBufSize)
		{
			memmove(pMpegTsDemuxer->abBufData, pMpegTsDemuxer->abBufData+MPEG_TS_PACKET_SIZE, pMpegTsDemuxer->iBufSize);
		}
	}
	else
	{
		iSrcDataOffset += MPEG_TS_PACKET_SIZE;
	}

	goto PARSE_NEXT_DATA_BLOCK;

RECEIVE_SAMPLE_END:

	return iOutRet;
}

INT MpegTsDemuxer_RegisterPacketReader(P_MPEG_TS_DEMUXER pMpegTsDemuxer, unsigned short int usPid,
									   P_MPEG_TS_PACKET_READER pMpegTsPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;
	P_LIST pMpegTsPacketReaderList = NULL;
	P_MPEG_TS_PACKET_READER_ITEM pMpegTsPacketReaderItem = NULL;
	POSITION pPos = NULL;
	BOOL bFound = FALSE;

	do 
	{
		if(NULL == pMpegTsDemuxer || NULL == pMpegTsPacketReader || PID_TS_NULL < usPid)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(NULL == pMpegTsDemuxer->pMpegTsPacketReaderList)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		//first, check whether already registered
		bFound = FALSE;
		pMpegTsPacketReaderList = pMpegTsDemuxer->pMpegTsPacketReaderList;
		pPos = List_GetHeadPosition(pMpegTsPacketReaderList);
		while(pPos)
		{
			pMpegTsPacketReaderItem = List_GetNext(pMpegTsPacketReaderList, &pPos);
			if(usPid == pMpegTsPacketReaderItem->usPid && 
				pMpegTsPacketReader == pMpegTsPacketReaderItem->pMpegTsPacketReader)
			{
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			break;
		}

		pMpegTsPacketReaderItem = malloc(sizeof(*pMpegTsPacketReaderItem));
		if(NULL == pMpegTsPacketReaderItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		memset(pMpegTsPacketReaderItem, 0x00, sizeof(*pMpegTsPacketReaderItem));
		pMpegTsPacketReaderItem->usPid = usPid;
		pMpegTsPacketReaderItem->pMpegTsPacketReader = pMpegTsPacketReader;

		pPos = List_AddTail(pMpegTsPacketReaderList, pMpegTsPacketReaderItem);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegTsDemuxer_UnregisterPacketReader(P_MPEG_TS_DEMUXER pMpegTsDemuxer, 
	const P_MPEG_TS_PACKET_READER pMpegTsPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;
	P_LIST pMpegTsPacketReaderList = NULL;
	P_MPEG_TS_PACKET_READER_ITEM pMpegTsPacketReaderItem = NULL;
	POSITION pPos = NULL, pCurPos = NULL;
	BOOL bFound = FALSE;

	do 
	{
		if(NULL == pMpegTsDemuxer || NULL == pMpegTsPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		if(pMpegTsDemuxer->pMpegTsPacketReaderList)
		{
			pMpegTsPacketReaderList = pMpegTsDemuxer->pMpegTsPacketReaderList;
			pPos = List_GetHeadPosition(pMpegTsPacketReaderList);
			while(pPos)
			{
				pCurPos = pPos;
				pMpegTsPacketReaderItem = List_GetNext(pMpegTsPacketReaderList, &pPos);
				if(pMpegTsPacketReader == pMpegTsPacketReaderItem->pMpegTsPacketReader)
				{	
					List_RemoveAt(pMpegTsPacketReaderList, pCurPos);
					free(pMpegTsPacketReaderItem);
					pMpegTsPacketReaderItem = NULL;
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

INT MpegTsDemuxer_Destroy(P_MPEG_TS_DEMUXER pMpegTsDemuxer)
{
	INT iOutRet = ERROR_SUCCESS;
	P_LIST pMpegTsPacketReaderList = NULL;
	P_MPEG_TS_PACKET_READER_ITEM pMpegTsPacketReaderItem = NULL;
	POSITION pPos = NULL;

	do 
	{
		if(NULL == pMpegTsDemuxer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegTsDemuxer->pMpegTsPacketReaderList)
		{
			//destroy MpegTsPacketReader list
			pMpegTsPacketReaderList = pMpegTsDemuxer->pMpegTsPacketReaderList;
			pPos = List_GetHeadPosition(pMpegTsPacketReaderList);
			while(pPos)
			{
				pMpegTsPacketReaderItem = List_GetNext(pMpegTsPacketReaderList, &pPos);
				if(pMpegTsPacketReaderItem)
				{
					free(pMpegTsPacketReaderItem);
					pMpegTsPacketReaderItem = NULL;
				}
			}
			List_RemoveAll(pMpegTsPacketReaderList);
			List_delete(pMpegTsDemuxer->pMpegTsPacketReaderList);
			pMpegTsDemuxer->pMpegTsPacketReaderList = NULL;
		}

		free(pMpegTsDemuxer);
		pMpegTsDemuxer = NULL;
	} while(FALSE);

	return iOutRet;
}

