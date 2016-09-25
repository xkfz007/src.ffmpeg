#include "H264VideoDecoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MpegPes.h"
#include "MpegPesPacketReader.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include "Nal.h"
#include "BitStream.h"

struct tagH264_VIDEO_DECODER{
	NAL_RBSP_PACKET packetNalRbsp;
	BYTE abNalUnitBuf[NAL_UNIT_BUF_SIZE];
	INT iNalUnitBufSize, iNalUnitBufStartOff;
	NAL_UNIT_SPS_DATA dataLastSequenceParaSet;
	BOOL bAlreadyGetSequenceParameterSet;
#ifdef RECORD_MPEG_ES_PACKET_DATA
	FILE * fpRecord;
#endif	//RECORD_MPEG_ES_PACKET_DATA
};

static INT H264VideoDecoder_ReceiveSample(PVOID pCookie, P_MPEG_PES_PACKET_INFO pMpegPesPacketInfo);
static INT H264VideoDecoder_ResetState(PVOID pCookie);
static INT H264VideoDecoder_Destroy(PVOID pCookie);
static INT H264VideoDecoder_IoControl(PVOID pCookie, UINT uiIoCtrlCode, PBYTE pControlData, INT iDataSize);

static INT ParseNalUnitPacket(P_H264_VIDEO_DECODER pH264VideoDecoder, P_NAL_UINT_PACKET pNalUnitPacket);

static MPEG_PES_PACKET_READER_OPS g_opsMpegPesPacketReader = {
	H264VideoDecoder_ReceiveSample,
	H264VideoDecoder_ResetState,
	H264VideoDecoder_Destroy,
	H264VideoDecoder_IoControl
};

INT InitNalRbspPacket(P_NAL_RBSP_PACKET pNalRbspPacket)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pNalRbspPacket)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

P_MPEG_PES_PACKET_READER H264VideoDecoder_CreateMpegPesPackertReader()
{
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader = NULL;
	P_H264_VIDEO_DECODER pH264VideoDecoder = NULL;

	do{
		pH264VideoDecoder = malloc(sizeof(*pH264VideoDecoder));
		if(NULL == pH264VideoDecoder)
		{
			break;
		}

		memset(pH264VideoDecoder, 0x00, sizeof(*pH264VideoDecoder));
		pH264VideoDecoder->iNalUnitBufSize = 0;
		pH264VideoDecoder->iNalUnitBufStartOff = 0;
		pH264VideoDecoder->bAlreadyGetSequenceParameterSet = FALSE;
#ifdef RECORD_MPEG_ES_PACKET_DATA
		pH264VideoDecoder->fpRecord = NULL;
#endif	//RECORD_MPEG_ES_PACKET_DATA
		pMpegPesPacketReader = MpegPesPacketReader_CreateWithCookie(pH264VideoDecoder, &g_opsMpegPesPacketReader);

#ifdef RECORD_MPEG_ES_PACKET_DATA
		pH264VideoDecoder->fpRecord = fopen("Record.ts", "wb");
		if(NULL == pH264VideoDecoder->fpRecord)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
#endif	//RECORD_MPEG_ES_PACKET_DATA
	}while(FALSE);

	return pMpegPesPacketReader;
}

INT H264VideoDecoder_ReceiveSample(PVOID pCookie, P_MPEG_PES_PACKET_INFO pMpegPesPacketInfo)
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0, iSrcDataOffset = 0, iNalStart = -1, iNalEnd = -1, iNalPacketLen = -1,
		iNalUnitType = 0, iNalDataOff = 0, iNalPacketLenForRbsp = -1;
	P_H264_VIDEO_DECODER pH264VideoDecoder = pCookie;
	PBYTE pNalUnitData = NULL;
	NAL_UINT_PACKET packetNalUnit;

	if(NULL == pH264VideoDecoder || NULL == pMpegPesPacketInfo)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto RECEIVE_SAMPLE_END;
	}

	//printf("idPesStream=0x%02x, sizePesPacket=%d\n", pMpegPesPacketInfo->idPesStream, pMpegPesPacketInfo->iPayloadSize);
#ifdef RECORD_MPEG_ES_PACKET_DATA
	if(pH264VideoDecoder->fpRecord)
	{
		iRet = fwrite(pMpegPesPacketInfo->pPayloadAddr, 1, pMpegPesPacketInfo->iPayloadSize,
			pH264VideoDecoder->fpRecord);
		if(pMpegPesPacketInfo->iPayloadSize != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
	}
#endif	//RECORD_MPEG_ES_PACKET_DATA

	if(0 >= pMpegPesPacketInfo->iPayloadSize)	//无负载数据
	{
		goto RECEIVE_SAMPLE_END;
	}

	//NAL_UNIT Buf不够
	if(NAL_UNIT_BUF_SIZE < pH264VideoDecoder->iNalUnitBufSize + pMpegPesPacketInfo->iPayloadSize)	
	{
		memmove(pH264VideoDecoder->abNalUnitBuf, pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufSize - 2,
			2);
		pH264VideoDecoder->iNalUnitBufSize = 2;

		if(NAL_UNIT_BUF_SIZE < pH264VideoDecoder->iNalUnitBufSize + pMpegPesPacketInfo->iPayloadSize)	//负载太大
		{
			pH264VideoDecoder->iNalUnitBufSize = 0;
		}
	}

	if(NAL_UNIT_BUF_SIZE < pMpegPesPacketInfo->iPayloadSize)	//负载太大
	{
		memcpy(pH264VideoDecoder->abNalUnitBuf, pMpegPesPacketInfo->pPayloadAddr + 
			(pMpegPesPacketInfo->iPayloadSize - NAL_UNIT_BUF_SIZE), NAL_UNIT_BUF_SIZE);
		pH264VideoDecoder->iNalUnitBufSize = NAL_UNIT_BUF_SIZE;
	}
	else
	{
		memcpy(pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufSize, pMpegPesPacketInfo->pPayloadAddr,
			pMpegPesPacketInfo->iPayloadSize);
		pH264VideoDecoder->iNalUnitBufSize += pMpegPesPacketInfo->iPayloadSize;
		iSrcDataOffset = pMpegPesPacketInfo->iPayloadSize;
	}

PARSE_NEXT_DATA_BLOCK:
	if(6 > pH264VideoDecoder->iNalUnitBufSize)
	{
		if(0 < pH264VideoDecoder->iNalUnitBufStartOff)
		{
			memmove(pH264VideoDecoder->abNalUnitBuf, 
				pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufStartOff, 
				pH264VideoDecoder->iNalUnitBufSize);
			pH264VideoDecoder->iNalUnitBufStartOff = 0;
		}
		goto RECEIVE_SAMPLE_END;
	}

	iNalStart = iNalEnd = -1;
	FindNalUnit(pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufStartOff, 
		pH264VideoDecoder->iNalUnitBufSize, &iNalStart, &iNalEnd);
	if(-1 == iNalStart)	//NAL包起始未找到
	{
		memmove(pH264VideoDecoder->abNalUnitBuf, 
			pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufStartOff + 
			pH264VideoDecoder->iNalUnitBufSize - 2, 2);
		pH264VideoDecoder->iNalUnitBufSize = 2;
		pH264VideoDecoder->iNalUnitBufStartOff = 0;
		goto RECEIVE_SAMPLE_END;
	}

	if(-1 == iNalEnd)	//没有找到NAL包结束
	{
		if(0 < pH264VideoDecoder->iNalUnitBufStartOff)
		{
			memmove(pH264VideoDecoder->abNalUnitBuf, 
				pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufStartOff, 
				pH264VideoDecoder->iNalUnitBufSize);
			pH264VideoDecoder->iNalUnitBufStartOff = 0;
		}
		goto RECEIVE_SAMPLE_END;
	}

	iNalPacketLen = iNalEnd - iNalStart;

	pNalUnitData = pH264VideoDecoder->abNalUnitBuf + pH264VideoDecoder->iNalUnitBufStartOff + iNalStart;
	iNalDataOff = 0;
	iNalUnitType = pNalUnitData[iNalDataOff] & 0x1F;

	//printf("nal_unit_type=%d, size=%d\n", iNalUnitType, iNalPacketLen);

	if(NAL_RBSP_PACKET_BUF_SIZE < iNalPacketLen)	//NAL_RBSP缓冲区不够，丢弃大包。
	{
		pH264VideoDecoder->iNalUnitBufStartOff += iNalEnd;
		pH264VideoDecoder->iNalUnitBufSize -= iNalEnd;
		goto PARSE_NEXT_DATA_BLOCK;
	}

	//NAL包转换为RBSP包
	iNalPacketLenForRbsp = iNalPacketLen;
	InitNalRbspPacket(&pH264VideoDecoder->packetNalRbsp);
	pH264VideoDecoder->packetNalRbsp.iNalRbspSize = sizeof(pH264VideoDecoder->packetNalRbsp.abNalRbspBuf);
	NalPacketToRbspPacket(pNalUnitData, &iNalPacketLenForRbsp, pH264VideoDecoder->packetNalRbsp.abNalRbspBuf, 
		&(pH264VideoDecoder->packetNalRbsp.iNalRbspSize));
	if(iNalPacketLenForRbsp != iNalPacketLen)
	{
		PRINT_FILE_LINENO_BUG_STR;
	}

	packetNalUnit.iNalUnitType = iNalUnitType;
	packetNalUnit.pNalRbspPacket = &(pH264VideoDecoder->packetNalRbsp);
	iRet = ParseNalUnitPacket(pH264VideoDecoder, &packetNalUnit);

	//调整至下一个包处理
	pH264VideoDecoder->iNalUnitBufStartOff += iNalEnd;
	pH264VideoDecoder->iNalUnitBufSize -= iNalEnd;
	goto PARSE_NEXT_DATA_BLOCK;

RECEIVE_SAMPLE_END:

	return iOutRet;
}

INT H264VideoDecoder_ResetState(PVOID pCookie)
{
	INT iOutRet = ERROR_SUCCESS;
	P_H264_VIDEO_DECODER pH264VideoDecoder = pCookie;

	do 
	{
		if(NULL == pH264VideoDecoder)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pH264VideoDecoder->iNalUnitBufSize = 0;
		pH264VideoDecoder->iNalUnitBufStartOff = 0;
		pH264VideoDecoder->bAlreadyGetSequenceParameterSet = FALSE;
		memset(&(pH264VideoDecoder->dataLastSequenceParaSet), 0x00, sizeof(pH264VideoDecoder->dataLastSequenceParaSet));
		memset(&(pH264VideoDecoder->packetNalRbsp), 0x00, sizeof(pH264VideoDecoder->packetNalRbsp));
	} while (FALSE);

	return iOutRet;
}

INT H264VideoDecoder_Destroy(PVOID pCookie)
{
	INT iOutRet = ERROR_SUCCESS;
	P_H264_VIDEO_DECODER pH264VideoDecoder = pCookie;

	do 
	{
		if(NULL == pH264VideoDecoder)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

#ifdef RECORD_MPEG_PES_PACKET_DATA
		if(pH264VideoDecoder->fpRecord)
		{
			iRet = fclose(pH264VideoDecoder->fpRecord);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pH264VideoDecoder->fpRecord = NULL;
		}
#endif	//RECORD_MPEG_PES_PACKET_DATA

		free(pH264VideoDecoder);
		pH264VideoDecoder = NULL;
	} while (FALSE);

	return iOutRet;
}

INT ParseNalUnitPacket(P_H264_VIDEO_DECODER pH264VideoDecoder, P_NAL_UINT_PACKET pNalUnitPacket)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, i = 0;
	P_BIT_STREAM pBitStream = NULL;

	if(NULL == pH264VideoDecoder || NULL == pNalUnitPacket)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto PARSE_NAL_UNIT_END;
	}

	if(NULL == pNalUnitPacket->pNalRbspPacket)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto PARSE_NAL_UNIT_END;
	}

	pBitStream = BitStream_Create();
	if(NULL == pBitStream)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		goto PARSE_NAL_UNIT_END;
	}

	iRet = BitStream_AttachBuffer(pBitStream, pNalUnitPacket->pNalRbspPacket->abNalRbspBuf, 
		pNalUnitPacket->pNalRbspPacket->iNalRbspSize);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto PARSE_NAL_UNIT_END;
	}

	if(NAL_UNIT_SPS == pNalUnitPacket->iNalUnitType)
	{
		P_NAL_UNIT_SPS_DATA pNalUnitSpsData = &(pH264VideoDecoder->dataLastSequenceParaSet);

		iRet = InitNalUnitSpsData(pNalUnitSpsData);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_BUG_STR;
		}

		pNalUnitSpsData->profile_idc = BitStream_Read_U8(pBitStream);
		pNalUnitSpsData->constraint_set0_flag = BitStream_Read_U1(pBitStream);
		pNalUnitSpsData->constraint_set1_flag = BitStream_Read_U1(pBitStream);
		pNalUnitSpsData->constraint_set2_flag = BitStream_Read_U1(pBitStream);
		pNalUnitSpsData->constraint_set3_flag = BitStream_Read_U1(pBitStream);
		pNalUnitSpsData->reserved_zero_4bits = BitStream_Read_U(pBitStream, 4);
		pNalUnitSpsData->level_idc = BitStream_Read_U8(pBitStream);
		pNalUnitSpsData->seq_parameter_set_id = BitStream_Read_UE(pBitStream);
		if(pNalUnitSpsData->profile_idc == 100 || pNalUnitSpsData->profile_idc == 110 ||
			pNalUnitSpsData->profile_idc == 122 || pNalUnitSpsData->profile_idc == 144 )
		{
			pNalUnitSpsData->chroma_format_idc = BitStream_Read_UE(pBitStream);
			if( pNalUnitSpsData->chroma_format_idc == 3 )
			{
				pNalUnitSpsData->residual_colour_transform_flag = BitStream_Read_U1(pBitStream);
			}
			pNalUnitSpsData->bit_depth_luma_minus8 = BitStream_Read_UE(pBitStream);
			pNalUnitSpsData->bit_depth_chroma_minus8 = BitStream_Read_UE(pBitStream);
			pNalUnitSpsData->qpprime_y_zero_transform_bypass_flag = BitStream_Read_U1(pBitStream);
			pNalUnitSpsData->seq_scaling_matrix_present_flag = BitStream_Read_U1(pBitStream);
			if( pNalUnitSpsData->seq_scaling_matrix_present_flag )
			{
				for( i = 0; i < 8; i++ )
				{
					pNalUnitSpsData->seq_scaling_list_present_flag[ i ] = BitStream_Read_U1(pBitStream);
					if( pNalUnitSpsData->seq_scaling_list_present_flag[ i ] )
					{
						if( i < 6 )
						{
							BitStream_Read_Scaling_List(pBitStream, &(pNalUnitSpsData->ScalingList4x4[ i ]), 16,
								pNalUnitSpsData->UseDefaultScalingMatrix4x4Flag[ i ]);
						}
						else
						{
							BitStream_Read_Scaling_List(pBitStream, &(pNalUnitSpsData->ScalingList8x8[ i - 6 ]), 64,
								pNalUnitSpsData->UseDefaultScalingMatrix8x8Flag[ i - 6 ] );
						}
					}
				}
			}
		}
		pNalUnitSpsData->log2_max_frame_num_minus4 = BitStream_Read_UE(pBitStream);
		pNalUnitSpsData->pic_order_cnt_type = BitStream_Read_UE(pBitStream);
		if( pNalUnitSpsData->pic_order_cnt_type == 0 )
		{
			pNalUnitSpsData->log2_max_pic_order_cnt_lsb_minus4 = BitStream_Read_UE(pBitStream);
		}
		else if( pNalUnitSpsData->pic_order_cnt_type == 1 )
		{
			INT i_offset_for_ref_frame_ArraySize;
			i_offset_for_ref_frame_ArraySize = sizeof(pNalUnitSpsData->offset_for_ref_frame) /
				sizeof(pNalUnitSpsData->offset_for_ref_frame[0]);
			pNalUnitSpsData->delta_pic_order_always_zero_flag = BitStream_Read_U1(pBitStream);
			pNalUnitSpsData->offset_for_non_ref_pic = BitStream_Read_SE(pBitStream);
			pNalUnitSpsData->offset_for_top_to_bottom_field = BitStream_Read_SE(pBitStream);
			pNalUnitSpsData->num_ref_frames_in_pic_order_cnt_cycle = BitStream_Read_UE(pBitStream);
			for( i = 0; i < pNalUnitSpsData->num_ref_frames_in_pic_order_cnt_cycle &&
				i < i_offset_for_ref_frame_ArraySize; i++ )
			{
				pNalUnitSpsData->offset_for_ref_frame[ i ] = BitStream_Read_SE(pBitStream);
			}
		}
		pNalUnitSpsData->num_ref_frames = BitStream_Read_UE(pBitStream);
		pNalUnitSpsData->gaps_in_frame_num_value_allowed_flag = BitStream_Read_U1(pBitStream);
		pNalUnitSpsData->pic_width_in_mbs_minus1 = BitStream_Read_UE(pBitStream);
		pNalUnitSpsData->pic_height_in_map_units_minus1 = BitStream_Read_UE(pBitStream);
		pNalUnitSpsData->frame_mbs_only_flag = BitStream_Read_U1(pBitStream);
		if( !pNalUnitSpsData->frame_mbs_only_flag )
		{
			pNalUnitSpsData->mb_adaptive_frame_field_flag = BitStream_Read_U1(pBitStream);
		}
		/*
		if(pNalUnitSpsData->frame_mbs_only_flag)
		{
			printf("%s\n", pNalUnitSpsData->frame_mbs_only_flag?"progressive":"interlaced");
		}
		else
		{
			printf("%s, %s\n", pNalUnitSpsData->frame_mbs_only_flag?"progressive":"interlaced",
				pNalUnitSpsData->mb_adaptive_frame_field_flag?"MBAFF":"PAFF");
		}
		*/

		pH264VideoDecoder->bAlreadyGetSequenceParameterSet = TRUE;
	}

PARSE_NAL_UNIT_END:
	if(pBitStream)
	{
		iRet = BitStream_Destroy(pBitStream);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		pBitStream = NULL;
	}

	return iOutRet;
}

INT H264VideoDecoder_IoControl(PVOID pCookie, UINT uiIoCtrlCode, PBYTE pControlData, INT iDataSize)
{
	INT iOutRet = ERROR_SUCCESS;
	P_H264_VIDEO_DECODER pH264VideoDecoder = pCookie;

	do 
	{
		if(NULL == pH264VideoDecoder)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		switch (uiIoCtrlCode)
		{
		case GET_H264_NAL_SPS_IS_VALID:
			{
				if(NULL == pControlData || sizeof(BOOL) != iDataSize)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
				*pControlData = pH264VideoDecoder->bAlreadyGetSequenceParameterSet;
				break;
			}
		case GET_H264_FRAME_FIELD_INFO:
			{
				P_H264_FRAME_FIELD_INFO pH264FrameFieldInfo = (P_H264_FRAME_FIELD_INFO)pControlData;
				if(NULL == pH264FrameFieldInfo || sizeof(H264_FRAME_FIELD_INFO) != iDataSize)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
				if(FALSE == pH264VideoDecoder->bAlreadyGetSequenceParameterSet)
				{
					iOutRet = ERROR_INVALID_STATE;
					break;
				}
				pH264FrameFieldInfo->bProgressive = pH264VideoDecoder->dataLastSequenceParaSet.frame_mbs_only_flag;
				pH264FrameFieldInfo->bH264Mbaff = pH264VideoDecoder->dataLastSequenceParaSet.mb_adaptive_frame_field_flag;
				break;
			}
		default:
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while (FALSE);

	return iOutRet;
}