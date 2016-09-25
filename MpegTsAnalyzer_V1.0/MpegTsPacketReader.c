#include "MpegTsPacketReader.h"
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
  #include <Windows.h>
#else
#endif
struct _MPEG_TS_PACKET_READER
{
	PVOID pCookie;
	MPEG_TS_PACKET_READER_OPS opsMpegTsPacketReader;
};

P_MPEG_TS_PACKET_READER 
MpegTsPacketReader_CreateWithCookie(PVOID pCookie, 
									P_MPEG_TS_PACKET_READER_OPS pMpegTsPacketReaderOps)
{
	P_MPEG_TS_PACKET_READER pMpegTsPacketReader = NULL;

	do{
		if(NULL == pCookie || NULL == pMpegTsPacketReaderOps)
		{			
			break;
		}

		pMpegTsPacketReader = malloc(sizeof(*pMpegTsPacketReader));
		if(NULL == pMpegTsPacketReader)
		{			
			break;
		}

		memset(pMpegTsPacketReader, 0x00, sizeof(pMpegTsPacketReader));
		pMpegTsPacketReader->pCookie = pCookie;
		pMpegTsPacketReader->opsMpegTsPacketReader = *pMpegTsPacketReaderOps;
	}while(FALSE);

	return pMpegTsPacketReader;
}

INT MpegTsPacketReader_ReceiveSample(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, 
									 P_MPEG_TS_PACKET_INFO pMpegTsPacketInfo)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsPacketReader || NULL == pMpegTsPacketInfo)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegTsPacketReader->opsMpegTsPacketReader.pfnReceiveSample)
		{
			iOutRet = pMpegTsPacketReader->opsMpegTsPacketReader.pfnReceiveSample(
				pMpegTsPacketReader->pCookie, pMpegTsPacketInfo);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
	} while (FALSE);

	return iOutRet;
}

INT MpegTsPacketReader_Destroy(P_MPEG_TS_PACKET_READER pMpegTsPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iOutRet = pMpegTsPacketReader->opsMpegTsPacketReader.pfnDestroy(
			pMpegTsPacketReader->pCookie);

		free(pMpegTsPacketReader);
		pMpegTsPacketReader = NULL;
	} while(FALSE);

	return iOutRet;
}

INT MpegTsPacketReader_RegisterPacketReader(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, BYTE maskPesStreamId,
											P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsPacketReader || NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegTsPacketReader->opsMpegTsPacketReader.pfnRegisterPacketReader)
		{
			iOutRet = pMpegTsPacketReader->opsMpegTsPacketReader.pfnRegisterPacketReader(pMpegTsPacketReader->pCookie,
				maskPesStreamId, pMpegPesPacketReader);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while(FALSE);

	return iOutRet;
}

INT MpegTsPacketReader_UnregisterPacketReader(P_MPEG_TS_PACKET_READER pMpegTsPacketReader, 
	P_MPEG_PES_PACKET_READER pMpegPesPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsPacketReader || NULL == pMpegPesPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegTsPacketReader->opsMpegTsPacketReader.pfnUnregisterPacketReader)
		{
			iOutRet = pMpegTsPacketReader->opsMpegTsPacketReader.pfnUnregisterPacketReader(
				pMpegTsPacketReader->pCookie, pMpegPesPacketReader);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while(FALSE);

	return iOutRet;
}

INT MpegTsPacketReader_ResetState(P_MPEG_TS_PACKET_READER pMpegTsPacketReader)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pMpegTsPacketReader)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(pMpegTsPacketReader->opsMpegTsPacketReader.pfnResetState)
		{
			iOutRet = pMpegTsPacketReader->opsMpegTsPacketReader.pfnResetState(pMpegTsPacketReader->pCookie);
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	} while (FALSE);

	return iOutRet;
}