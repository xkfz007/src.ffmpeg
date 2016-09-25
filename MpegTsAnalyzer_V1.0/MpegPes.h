#ifndef	_MPEG_PES_H_
#define	_MPEG_PES_H_

#define MPEG_PES_PACKET_START_PREFIX		"\x00\x00\x01"

/*
offset 0: 00 00 01 BA, 程序流起始码
	      00 00 01 BB, system_header_start_code: 程序流系统首部起始码
	      00 00 01 BC, program_stream_map
	      00 00 01 BE, padding_stream, 填充流
	      00 00 01 E0, video stream, 视频流ID=E0(1110 xxxx)
	      00 00 01 C0, audio   stream: 音频流:ID=C0(110x   xxxx)
	      00 00 01 B3, sequence_header_code: 视频序列头部 
	      00 00 01 B5, extension_start_code: 图像编码扩展
	      00 00 01 B8, group_start_code: 图组头   TIMECODE(00:00.20-00) 
	      00 00 01 00, picture_start_code: 图像头(单幅图像)--帧类型(1-I,2-P,3-B)1
	      00 00 01 01, slice_start_code: 组块:ID=01     
	      00 00 01 02, slice_start_code: 组块:ID=02     
	      00 00 01 03, slice_start_code: 组块:ID=03     
	      00 00 01 04, slice_start_code: 组块:ID=04     
	      00 00 01 05, slice_start_code: 组块:ID=05     
	      00 00 01 06, slice_start_code: 组块:ID=06     
*/

/*
In Program Streams, the stream_id for DTS is “private_stream_1” = 1011 1101 = 0xBD
*/
//PES packet header
typedef struct tagMPEG_PES_HEADER
{
	unsigned char prefixPesStartCode[3];	//包头起始码前缀, "\x00\x00\x01"
	unsigned char idStream;	//数据流识别, LS bits give stream number
	UINT uiPesPacketLength;	//network byte order, 27 6A->0x276A
	/*
	If stream ID is Private Stream 2 or Padding Stream, skip to data bytes.
	*/
	//基本码流特有信息(3~259Bytes)
	unsigned short int pes_scrambling_control : 2;	//PES加扰控制
	unsigned short int pes_priority : 1;	//PES优先
	unsigned short int data_position_indicator : 1;	//数据定位指示符
	unsigned short int copyright : 1;	//版权
	unsigned short int original_or_copy : 1;	//原版或拷贝
	unsigned short int PTS_or_DTS_flag : 2;	//PTS/DTS标志
	unsigned short int ESCR_flag : 1;	//ESCR标志
	unsigned short int base_stream_rate_flag : 1;	//基本流速率标志
	unsigned short int DSM_trick_mode_flag : 1;	//DSM特技方式标志
	unsigned short int additional_info_copy : 1;	//附加的信息拷贝
	unsigned short int PES_CRC_flag : 1;	//PES CRC标志
	unsigned short int PES_extension_flag : 1;	//PES扩展标志
	unsigned char ucPesPacketHeaderLen;	//PES包头长
	UINT64 uiPts;	//presentation timestamp
	UINT64 uiDts;
}MPEG_PES_HEADER, *P_MPEG_PES_HEADER;

typedef struct tagMPEG_PES_PACKET_INFO{
	BYTE idPesStream;
	PBYTE pPayloadAddr;
	INT iPayloadSize;
}MPEG_PES_PACKET_INFO, *P_MPEG_PES_PACKET_INFO;

//MPEG ES packet
typedef struct tagMPEG_ES_HEADER
{
	unsigned char eFrameType[4];	//"\x00\x00\0x01\x00", I,B,P frame type
}MPEG_ES_HEADER, *P_MPEG_ES_HEADER;

#endif	//_MPEG_PES_H_

