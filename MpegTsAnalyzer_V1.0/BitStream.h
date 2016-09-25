#ifndef _BIT_STREAM_H_
#define _BIT_STREAM_H_

#ifdef _WIN32
#include <Windows.h>
#endif
#include <BaseTypeDef.h>

typedef struct tagBIT_STREAM BIT_STREAM;
typedef BIT_STREAM * P_BIT_STREAM;

P_BIT_STREAM BitStream_Create();
INT BitStream_AttachBuffer(P_BIT_STREAM pBitStream, PBYTE pBuf, INT iBufSize);
INT BitStream_getLastErrNo(P_BIT_STREAM pBitStream);
BOOL BitStream_getEof(P_BIT_STREAM pBitStream);
UINT32 BitStream_Read_U1(P_BIT_STREAM pBitStream);
UINT32 BitStream_Read_U8(P_BIT_STREAM pBitStream);
UINT32 BitStream_Read_UE(P_BIT_STREAM pBitStream);
UINT32 BitStream_Read_U(P_BIT_STREAM pBitStream, int n);
INT32 BitStream_Read_SE(P_BIT_STREAM pBitStream);
void BitStream_Read_Scaling_List(P_BIT_STREAM pBitStream, int* scalingList, int sizeOfScalingList, 
								 int useDefaultScalingMatrixFlag );
INT BitStream_Destroy(P_BIT_STREAM pBitStream);

#endif	//_BIT_STREAM_H_
