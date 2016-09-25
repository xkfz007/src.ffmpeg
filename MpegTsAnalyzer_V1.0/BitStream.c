#include "BitStream.h"
#include <BaseErrDef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tagBIT_STREAM
{
	PBYTE pStart;
	PBYTE pCur;
	PBYTE pEnd;
	BYTE bitsLeft;
	INT iLastErrNo;
};

P_BIT_STREAM BitStream_Create()
{
	P_BIT_STREAM pBitStream = NULL;

	do 
	{
		pBitStream = malloc(sizeof(*pBitStream));
		if(NULL == pBitStream)
		{
			break;
		}

		memset(pBitStream, 0x00, sizeof(*pBitStream));
	} while(FALSE);

	return pBitStream;
}

INT BitStream_AttachBuffer(P_BIT_STREAM pBitStream, PBYTE pBuf, INT iBufSize)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pBitStream || NULL == pBuf || 0 >= iBufSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pBitStream->pStart = pBuf;
		pBitStream->pCur = pBuf;
		pBitStream->pEnd = pBuf + iBufSize;
		pBitStream->bitsLeft = 8;
	} while(FALSE);

	return iOutRet;
}

INT BitStream_getLastErrNo(P_BIT_STREAM pBitStream)
{
	return (pBitStream?pBitStream->iLastErrNo:0);
}

BOOL BitStream_getEof(P_BIT_STREAM pBitStream)
{
	BOOL bOutRet = TRUE;

	do 
	{
		if(NULL == pBitStream)
		{
			bOutRet = TRUE;
			break;
		}

		if(pBitStream->pCur >= pBitStream->pEnd)
		{
			bOutRet = TRUE;
		}
		else
		{
			bOutRet = FALSE;
		}
	} while(FALSE);

	return bOutRet;
}

UINT32 BitStream_Read_U1(P_BIT_STREAM pBitStream)
{
	UINT32 r = 0;
	pBitStream->iLastErrNo = ERROR_SUCCESS;

	if(BitStream_getEof(pBitStream))
	{
		pBitStream->iLastErrNo = ERR_END_OF_FILE;	
		return 0; 
	}

	pBitStream->bitsLeft--;
	r = ((*(pBitStream->pCur)) >> pBitStream->bitsLeft) & 0x01;

	if(pBitStream->bitsLeft == 0) 
	{
		pBitStream->pCur++; 
		pBitStream->bitsLeft = 8; 
	}

	return r;
}

UINT32 BitStream_Read_U8(P_BIT_STREAM pBitStream)
{
	return BitStream_Read_U(pBitStream, 8);
}

UINT32 BitStream_Read_U(P_BIT_STREAM pBitStream, int n)
{
	UINT32 r = 0;
	int i;

	pBitStream->iLastErrNo = ERROR_SUCCESS;

	for (i = 0; i < n; i++)
	{
		r |= ( BitStream_Read_U1(pBitStream) << ( n - i - 1 ) );
	}

	return r;
}

UINT32 BitStream_Read_UE(P_BIT_STREAM pBitStream)
{
	INT32 r = 0;
	int i = 0;

	while( BitStream_Read_U1(pBitStream) == 0 && i < 32 && !BitStream_getEof(pBitStream) )
	{
		i++;
	}
	r = BitStream_Read_U(pBitStream, i);
	r += (1 << i) - 1;

	return r;
}

INT32 BitStream_Read_SE(P_BIT_STREAM pBitStream) 
{
	INT32 r = BitStream_Read_UE(pBitStream);
	if (r & 0x01)
	{
		r = (r+1)/2;
	}
	else
	{
		r = -(r/2);
	}

	return r;
}

//7.3.2.1.1 Scaling list syntax
void BitStream_Read_Scaling_List(P_BIT_STREAM pBitStream, int* scalingList, int sizeOfScalingList, 
								 int useDefaultScalingMatrixFlag )
{
	int j;

	int lastScale = 8;
	int nextScale = 8;
	for( j = 0; j < sizeOfScalingList; j++ )
	{
		if( nextScale != 0 )
		{
			int delta_scale = BitStream_Read_SE(pBitStream);
			nextScale = ( lastScale + delta_scale + 256 ) % 256;
			useDefaultScalingMatrixFlag = ( j == 0 && nextScale == 0 );
		}
		scalingList[ j ] = ( nextScale == 0 ) ? lastScale : nextScale;
		lastScale = scalingList[ j ];
	}
}

INT BitStream_Destroy(P_BIT_STREAM pBitStream)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pBitStream)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		free(pBitStream);
		pBitStream = NULL;
	} while(FALSE);

	return iOutRet;
}