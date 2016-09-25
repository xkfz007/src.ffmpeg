#include "Nal.h"
#include <stdio.h>
#ifdef _WIN32
#include <assert.h>
#endif
#include <BaseErrDef.h>
#ifdef _WIN32
  #include <Windows.h>
#else
#endif
void FindNalUnit(PBYTE pDataBuf, INT iDataSize, INT * piStartOff, INT * piEndOff)
{
	int iId = 0;

	if(NULL == piStartOff || NULL == piEndOff)
	{
		goto FIND_END;
	}

	if(NULL == pDataBuf || 0 >= iDataSize)
	{
		*piStartOff = -1;
		*piEndOff = -1;

		goto FIND_END;
	}

	// find start
	*piStartOff = -1;
	*piEndOff = -1;
	iId = 0;

	//( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
	while((pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0x01) && 
		(pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0 || pDataBuf[iId+3] != 0x01))
	{
		iId++; // skip leading zero
		if (iId+4 >= iDataSize)  // did not find nal start
		{
			goto FIND_END; 
		}
	}

	if(pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0x01) // ( next_bits( 24 ) != 0x000001 )
	{
		iId++;
	}

	if(pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0x01) 
	{
		/* error, should never happen */ 
#ifdef	_WIN32
		assert(FALSE);
#endif
		goto FIND_END; 
	}
	iId += 3;
	*piStartOff = iId;

	//( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
	while((pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0) && 
		(pDataBuf[iId] != 0 || pDataBuf[iId+1] != 0 || pDataBuf[iId+2] != 0x01))
	{
		iId++;
		// FIXME the next line fails when reading a nal that ends exactly at the end of the data
		if(iId + 3 >= iDataSize)  // did not find nal end, stream ended first
		{
			*piEndOff = -1; 
			goto FIND_END;
		}
	}

	*piEndOff = iId;	

FIND_END:
	;
}

//务必确保BUFSIZE(pRbspPacketBuf) >= *piNalPacketSize
void NalPacketToRbspPacket(const PBYTE pNalPacketBuf, INT * piNalPacketSize, PBYTE pRbspPacketBuf, INT * piRbspPacketSize)
{
	// FIXME don't like using *nal_size etc syntax
	int i, j;

	do{
		if(NULL == pNalPacketBuf || 0 >= *piNalPacketSize || NULL == pRbspPacketBuf || *piRbspPacketSize < *piNalPacketSize)
		{
			if(piRbspPacketSize)
			{
				*piRbspPacketSize = 0;
			}
			break;
		}

		if(piRbspPacketSize)
		{
			*piRbspPacketSize = 0;
		}

		i = 1; // NOTE omits first byte of NAL which contains nal_ref_idc and nal_unit_type, this is NOT part of the RBSP
		j = 0;
		while( i < *piNalPacketSize )
		{
			if( i + 2 < *piNalPacketSize && 
				pNalPacketBuf[i] == 0x00 && pNalPacketBuf[i+1] == 0x00 && pNalPacketBuf[i+2] == 0x03 ) // next_bits( 24 ) == 0x000003
			{
				pRbspPacketBuf[ j   ] = pNalPacketBuf[ i   ];
				pRbspPacketBuf[ j+1 ] = pNalPacketBuf[ i+1 ];
				// buf[ i+2 ] == 0x03  // skip emulation_prevention_three_byte equal to 0x03 // this is guaranteed from the above condition
				i += 3; j += 2;
			}
			else if (i + 2 < *piNalPacketSize && 
				pNalPacketBuf[i] == 0x00 && pNalPacketBuf[i+1] == 0x00 && pNalPacketBuf[i+2] == 0x01 ) // next_bits( 24 ) == 0x000001 // start of next nal, we're done
			{
				break;
			}
			else
			{
				pRbspPacketBuf[ j ] = pNalPacketBuf[ i ];
				i += 1; j += 1;
			}
		}
		*piNalPacketSize = i;
		*piRbspPacketSize = j;
	}while(FALSE);
}

INT InitNalUnitSpsData(P_NAL_UNIT_SPS_DATA pNalUnitSpsData)
{
	INT iOutRet = ERROR_SUCCESS;

	do 
	{
		if(NULL == pNalUnitSpsData)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pNalUnitSpsData->profile_idc = 0;
		pNalUnitSpsData->constraint_set0_flag = 0;
		pNalUnitSpsData->constraint_set1_flag = 0;
		pNalUnitSpsData->constraint_set2_flag = 0;
		pNalUnitSpsData->constraint_set3_flag = 0;
		pNalUnitSpsData->reserved_zero_4bits = 0;
		pNalUnitSpsData->level_idc = 0;
		pNalUnitSpsData->frame_mbs_only_flag = TRUE;	// progressive
		pNalUnitSpsData->mb_adaptive_frame_field_flag = 0;	// PAFF
	} while (FALSE);

	return iOutRet;
}