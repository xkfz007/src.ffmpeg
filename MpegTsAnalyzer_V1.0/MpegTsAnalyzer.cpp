// MpegTsInfo.cpp : Defines the entry point for the console application.
//

#ifdef	_WIN32
#include "stdafx.h"
#endif
#include "MpegTsDemux.h"
#include "MpegPesDemux.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#ifdef	_WIN32
#include <conio.h>
#endif
#include "H264VideoDecoder.h"
#ifdef	_WIN32
#include <share.h>
#endif
#include "MpegPesPacketReader.h"
#include <string2.h>
#include <StrOpMacro.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#endif

typedef struct
{
	char szInFile[128];
	INT iVideoPid, iAudioPid;
}CMD_LINE_PARA, *P_CMD_LINE_PARA;

static INT ParseCmdLinePara(int argc, char * argv[], P_CMD_LINE_PARA pCmdLinePara);

INT ParseCmdLinePara(int argc, char * argv[], P_CMD_LINE_PARA pCmdLinePara)
{
	INT iOutRet = ERROR_SUCCESS, iId = 0;

	do{
		if(3 >= argc)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			printf("Usage: %s -InFile <TsFile> -VPid <VideoPid> -APid <AudioPid>\n", argv[0]);
			break;
		}

		for(iId=1; iId<argc; iId++)
		{
			if(0 == strcmp_safe("-InFile", argv[iId]))
			{
				if(argc -1 < iId+1)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					printf("invalid parameter pairs: %s\n", argv[iId]);
					break;
				}
				iId++;
				SAFE_STRNCPY(pCmdLinePara->szInFile, argv[iId]);
			}
			else if(0 == strcmp_safe("-VPid", argv[iId]))
			{
				if(argc -1 < iId+1)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					printf("invalid parameter pairs: %s\n", argv[iId]);
					break;
				}
				iId++;
				pCmdLinePara->iVideoPid = atoi(argv[iId]);
			}
			else if(0 == strcmp_safe("-APid", argv[iId]))
			{
				if(argc -1 < iId+1)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					printf("invalid parameter pairs: %s\n", argv[iId]);
					break;
				}
				iId++;
				pCmdLinePara->iAudioPid = atoi(argv[iId]);
			}
			else
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				printf("invalid parameter: %s\n", argv[iId]);
			}
		}
	}while(FALSE);

	return iOutRet;
}

#ifdef	_WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char * argv[])
#endif	
{
	INT iRet = 0, iCntRead = 0;
	FILE * fpTS = NULL;
	BYTE abReadBuf[32*1024];
	BOOL bGet = FALSE;
	P_MPEG_TS_DEMUXER pMpegTsDemuxer = NULL;
	P_MPEG_TS_PACKET_READER pMpegTsPacketReader = NULL, pMpegAudioTsPacketReader = NULL;
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader = NULL;
	CMD_LINE_PARA paraCmdLine;

	memset(&paraCmdLine, 0x00, sizeof(paraCmdLine));	
	paraCmdLine.iVideoPid = PID_TS_NULL;
	paraCmdLine.iAudioPid = PID_TS_NULL;

	do{
#ifdef	_WIN32		
		iRet = ParseCmdLinePara(argc, (char **)argv, &paraCmdLine);
#else
		iRet = ParseCmdLinePara(argc, argv, &paraCmdLine);
#endif
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		
#ifdef _WIN32
		fpTS = _fsopen(paraCmdLine.szInFile, "rb", _SH_DENYWR);
#else
		fpTS = fopen(paraCmdLine.szInFile, "rb");
#endif
		if(NULL == fpTS)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		pMpegTsDemuxer = MpegTsDemuxer_Create();
		if(NULL == pMpegTsDemuxer)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		pMpegTsPacketReader = MpegPesDemux_CreateTsPacketReader();
		if(NULL == pMpegTsPacketReader)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		pMpegPesPacketReader = H264VideoDecoder_CreateMpegPesPackertReader();
		if(NULL == pMpegPesPacketReader)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		pMpegAudioTsPacketReader = MpegPesDemux_CreateTsPacketReader();
		if(NULL == pMpegAudioTsPacketReader)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		if(PID_TS_NULL != paraCmdLine.iVideoPid)
		{
			iRet = MpegTsDemuxer_RegisterPacketReader(pMpegTsDemuxer, paraCmdLine.iVideoPid, pMpegTsPacketReader);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
		}

		if(PID_TS_NULL != paraCmdLine.iAudioPid)
		{
			iRet = MpegTsDemuxer_RegisterPacketReader(pMpegTsDemuxer, paraCmdLine.iAudioPid, pMpegAudioTsPacketReader);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
		}

		iRet = MpegTsPacketReader_RegisterPacketReader(pMpegTsPacketReader, MPEG_PES_STREAM_ID_VIDEO_MASK, 
			pMpegPesPacketReader);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}

		while(TRUE)
		{
			iCntRead = fread(abReadBuf, 1, sizeof(abReadBuf), fpTS);
			if(0 >= iCntRead)
			{
				break;
			}

			iRet = MpegTsDemuxer_ReceiveSample(pMpegTsDemuxer, abReadBuf, iCntRead);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}

			iRet = MpegPesPacketReader_IoControl(pMpegPesPacketReader, GET_H264_NAL_SPS_IS_VALID, (PBYTE)(&bGet), sizeof(bGet));
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}

#if	0
			if(bGet)
			{
				H264_FRAME_FIELD_INFO infoH264FrameField;
				memset(&infoH264FrameField, 0x00, sizeof(infoH264FrameField));
				iRet = MpegPesPacketReader_IoControl(pMpegPesPacketReader, GET_H264_FRAME_FIELD_INFO, 
					(PBYTE)(&infoH264FrameField), sizeof(infoH264FrameField));
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_FILE_LINENO_IRET_BUG_STR;
				}
				PRINT_FILE_LINENO;
				if(infoH264FrameField.bProgressive)
				{
					printf("progressive\n");
				}
				else
				{
					printf("interlaced, %s\n", infoH264FrameField.bH264Mbaff?"MBAFF":"PAFF");
				}
				break;
			}
#endif
		}	
	}while(FALSE);

	if(pMpegTsDemuxer)
	{
		iRet = MpegTsDemuxer_Destroy(pMpegTsDemuxer);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		pMpegTsDemuxer = NULL;
	}

	if(pMpegTsPacketReader)
	{
		iRet = MpegTsPacketReader_Destroy(pMpegTsPacketReader);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		pMpegTsPacketReader = NULL;
	}

	if(pMpegAudioTsPacketReader)
	{
		iRet = MpegTsPacketReader_Destroy(pMpegAudioTsPacketReader);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		pMpegAudioTsPacketReader = NULL;
	}

	if(pMpegPesPacketReader)
	{
		iRet = MpegPesPacketReader_Destroy(pMpegPesPacketReader);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
		pMpegPesPacketReader = NULL;
	}

	if(fpTS)
	{
		iRet = fclose(fpTS);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
		fpTS = NULL;
	}

#ifdef	_WIN32
	_getch();
#endif

	return 0;
}

