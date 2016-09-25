#include "MpegPesPacketReader.h"
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
  #include <Windows.h>
#else
#endif

struct _MPEG_PES_PACKET_READER
{
	PVOID pCookie;
	MPEG_PES_PACKET_READER_OPS opsMpegPesPacketReader;
};

P_MPEG_PES_PACKET_READER 
MpegPesPacketReader_CreateWithCookie(PVOID pCookie, 
									P_MPEG_PES_PACKET_READER_OPS pMpegPesPacketReaderOps)
{
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader = NULL;

	do{
		if(NULL == pCookie || NULL == pMpegPesPacketReaderOps)
		{			
			break;
		}

		pMpegPesPacketReader = malloc(sizeof(*pMpegPesPacketReader));
		if(NULL == pMpegPesPacketReader)
		{			
			break;
		}

		memset(pMpegPesPacketReader, 0x00, sizeof(pMpegPesPacketReader));
		pMpegPesPacketReader->pCookie = pCookie;
		pMpegPesPacketReader->opsMpegPesPacketReader = *pMpegPesPacketReaderOps;
	}while(FALSE);

	return pMpegPesPacketReader;
}

INT MpegPesPacketReader_ReceiveSample(P_MPEG_PES_PACKET_READER pMpegPesPacketReader, 
									 P_MPEG_PES_PACKET_INFO pMpegPesPacketInfo)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegPesPacketReader || NULL == pMpegPesPacketInfo)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegPesPacketReader->opsMpegPesPacketReader.pfnReceiveSample)
		{
			iOutRet = pMpegPesPacketReader->opsMpegPesPacketReader.pfnReceiveSample(
				pMpegPesPacketReader->pCookie, pMpegPesPacketInfo);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegPesPacketReader_ResetState(P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegPesPacketReader->opsMpegPesPacketReader.pfnResetState)
		{
			iOutRet = pMpegPesPacketReader->opsMpegPesPacketReader.pfnResetState(pMpegPesPacketReader->pCookie);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegPesPacketReader_Destroy(P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iOutRet = pMpegPesPacketReader->opsMpegPesPacketReader.pfnDestroy(
			pMpegPesPacketReader->pCookie);

		free(pMpegPesPacketReader);
		pMpegPesPacketReader = NULL;
	} while(FALSE);

	return iOutRet;
}

INT MpegPesPacketReader_IoControl(P_MPEG_PES_PACKET_READER pMpegPesPacketReader, UINT uiIoCtrlCode, PBYTE pControlData, 
								  INT iDataSize)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if (NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegPesPacketReader->opsMpegPesPacketReader.pfnIoControl)
		{
			iOutRet = pMpegPesPacketReader->opsMpegPesPacketReader.pfnIoControl(pMpegPesPacketReader->pCookie,
				uiIoCtrlCode, pControlData, iDataSize);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while (FALSE);

	return iOutRet;
}