#ifndef	_NAL_H_
#define _NAL_H_

#include <BaseTypeDef.h>

#define	NAL_UNIT_SPS			7		//sequence parameter set

typedef struct{
	BYTE profile_idc;
	BYTE constraint_set0_flag : 1;
	BYTE constraint_set1_flag : 1;
	BYTE constraint_set2_flag : 1;
	BYTE constraint_set3_flag : 1;
	BYTE reserved_zero_4bits : 4;
	BYTE level_idc;
	INT seq_parameter_set_id;
	INT chroma_format_idc;
	BYTE residual_colour_transform_flag : 1;
	INT bit_depth_luma_minus8;
	INT bit_depth_chroma_minus8;
	BYTE qpprime_y_zero_transform_bypass_flag : 1;
	BYTE seq_scaling_matrix_present_flag : 1;
	BYTE seq_scaling_list_present_flag[8];
	//INT * ScalingList4x4[6];
	INT ScalingList4x4[6];
	int UseDefaultScalingMatrix4x4Flag[6];
	//int* ScalingList8x8[2];
	int ScalingList8x8[2];
	int UseDefaultScalingMatrix8x8Flag[2];
	int log2_max_frame_num_minus4;
	int pic_order_cnt_type;
	int log2_max_pic_order_cnt_lsb_minus4;
	int delta_pic_order_always_zero_flag;
	int offset_for_non_ref_pic;
	int offset_for_top_to_bottom_field;
	int num_ref_frames_in_pic_order_cnt_cycle;
	int offset_for_ref_frame[256];
	int num_ref_frames;
	int gaps_in_frame_num_value_allowed_flag;
	int pic_width_in_mbs_minus1;
	int pic_height_in_map_units_minus1;
	BYTE frame_mbs_only_flag : 1;	// 1 -- progressive, 0 -- interlaced
	BYTE mb_adaptive_frame_field_flag : 1;	// 0 -- PAFF, 1 -- MBAFF
}NAL_UNIT_SPS_DATA, *P_NAL_UNIT_SPS_DATA;

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

void FindNalUnit(PBYTE pDataBuf, INT iDataSize, INT * piStartOff, INT * piEndOff);
void NalPacketToRbspPacket(const PBYTE pNalPacketBuf, INT * piNalPacketSize, PBYTE pRbspPacketBuf, INT * piRbspPacketSize);
INT InitNalUnitSpsData(P_NAL_UNIT_SPS_DATA pNalUnitSpsData);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_NAL_H_