#ifndef	_MPEG_TS_H_
#define	_MPEG_TS_H_

#define	MPEG_TS_PACKET_SIZE					188
#define	MPEG_TS_BASE_PACKET_HEADER_SIZE		4

#define	MPEG_TS_SYNC_BYTE_VAL		0x47

#define	PID_TS_PAT				0x00
#define	PID_TS_CAT				0x01	//Conditional Accept Table
//TSDT -- Transport Stream Description Table
#define	PID_TS_TSDT 			0x02
#define	PID_TS_NULL			0x1FFF

//adaption_field_control, 11，标识既有adaptation_field又有payload
typedef struct tagMPEG_TS_PACKET_HEADER
{
	unsigned int sync_byte :8;
	unsigned int transport_error_indicator  :1;	//high bit
	unsigned int payload_unit_start_indicator :1;	//有效载荷单元起始指示符
	unsigned int transport_priority :1;	//优先传输?
	unsigned int pid :13;
	unsigned int transport_scrambling_control :2;	//传输加扰控制类型
	unsigned int adaption_field_control :1;	// 1 -- only include additional info but not include payload
	unsigned int payload_field_control : 1;	//有效载荷标识
	unsigned int continuity_counter :4;	//连续计数器(针对每个pid的计数0~15)
}MPEG_TS_PACKET_HEADER, *P_MPEG_TS_PACKET_HEADER;

//自适应区
typedef struct tagMPEG_TS_ADAPTIVE_INFO{
	unsigned char break_indicator:1;	//间断指示符
	unsigned char random_access_indicator:1;	//随机指示符
	unsigned char base_bitrate_optimize_indicator:1;	//基本码流优化指示符
	unsigned char PCR_flag:1;	//PCR flag
	unsigned char Joint_flag:1;	//接点标志
	unsigned char TransformSpecifiedData_flag:1;	//传输专用标志
	unsigned char original_PCR_flag:1;	//原始PCR flag
	unsigned char AdaptiveExtension_flag:1;	//自适应区扩展标志
}MPEG_TS_ADAPTIVE_INFO, *P_MPEG_TS_ADAPTIVE_INFO;

typedef struct tagMPEG_TS_PACKET_INFO{
	unsigned short int usPid;
	BOOL bPayloadUnitStartIndicator;
	INT iPayloadSize;
	PBYTE pPayloadAddr;
	BOOL bPcrBaseAvailable;
	UINT64 uiPcrBase;
}MPEG_TS_PACKET_INFO, *P_MPEG_TS_PACKET_INFO;

//PAT --  Program Association Table
typedef struct tagMPEG_TS_PAT
{
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned zero : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned transport_stream_id : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;	//if 1, PAT is available currently
	unsigned section_number : 8;
	unsigned last_section_number : 8; 
	unsigned program_number : 16;
	unsigned reserved_3 : 3;
	unsigned network_PID : 13;
	unsigned program_map_PID : 13; 	//Pid of PMT
	unsigned CRC_32 : 32; 
}MPEG_TS_PAT, *P_MPEG_TS_PAT; 

//PMT -- Program Map Table
typedef struct tagMPEG_TS_PMT
{
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned zero : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned program_number : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;
	unsigned section_number : 8;
	unsigned last_section_number : 8;
	unsigned reserved_3 : 3;
	unsigned PCR_PID : 13;
	unsigned reserved_4 : 4;
	unsigned program_info_length : 12;	//bytes number

	unsigned stream_type : 8;
	unsigned reserved_5 : 3;
	unsigned elementary_PID : 13;
	unsigned reserved_6 : 4;
	unsigned ES_info_length : 12; 
	unsigned CRC_32 : 32; 
}MPEG_TS_PMT, *P_MPEG_TS_PMT; 

#endif	//_MPEG_TS_H_

