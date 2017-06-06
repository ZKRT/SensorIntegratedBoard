/**
  ******************************************************************************
  * @file    ToFSensorOP.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   红外ToF传感器数据解析
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

#include "ToFSensorOP.h"
#include "osusart.h"
#include "commonzkrt.h"
#include "appdistance.h"

/**
  * @brief  传感器返回的数据包解析
  * @param  
  *			1、packet：数据包首地址，长度为固定为10个字节的数组
  *			2、data_buff[]：存储解析后的数据缓冲区
  * @note   先根据返回值判断是否解析成功，才使用data_buff[]的数据
  *			注意：确保packet和data_buff的内存区域不能重叠
  * @retval 解析成功返回 1，失败返回 0
  */
U8 packet_parser(U8 *packet, U8 data_buff[])
{
	U16 crc16_cal = 0x0000;
	U16 crc16_packet = 0x0000;
	U8 len = packet[1]+2;
	if(packet[0] != SENSOR_PACKET_START)
	{
		return 0;
	}
	crc16_packet = packet[len-2]<<8;//取出数据包crc16
	crc16_packet |= packet[len-1];
	crc16_cal = CRC16_Cal(packet+2, len-4);
	if(crc16_cal == crc16_packet)
	{
		memcpy(data_buff, packet+2, len-4);
		return 1;
	}
	return 0;
}


/**
  * @brief  停止连续测量
  * @param  无
  * @note   
  * @retval 无
  */
void ToF_continuousStop(void)
{
//	U8 comm[] = {0x0A, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x6F};
//	disableEXINT0();//关闭外部中断触发数据接收
//	uart_write(comm, 10);
}

///**
//  * @brief  单次获取测量结果
//  * @param  dbuf：数据缓冲区，为sensorData *类型
//  * @note   
//  * @retval 成功返回 1，失败返回 0
//  */
//U8 ToF_SingleshotCapture(sensorData *dbuf)
//{
//	U8 comm[SENSOR_DATA_LEN-4] = {0x0A, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x57};
//	U8 read_buff[SENSOR_DATA_LEN]={0};
//	SCON0_RI = 0;//非中断方式串口接收，先清除UART 接收中断， 根据自己平台决定是否需要此语句
//	read_buff[0]=0x00;
//	uart_write(comm, 10);
//	uart_read(read_buff, SENSOR_DATA_LEN, 1);
//	memset(dbuf, 0, sizeof(sensorData));
//	//memset(comm, 0, 10);
//	if(packet_parser(read_buff, comm) == 1)
//	{
//		dbuf->distance = (comm[3] << 8)|comm[4];
//		dbuf->magnitude = (comm[5] << 8)|comm[6];
//		dbuf->magnitude_exp = comm[7];
//		dbuf->ambient_adc = comm[8];
//		dbuf->precision = (comm[9] << 8)|comm[10];
//		return 1;
//	}
//	return 0;
//}

///**
//  * @brief  获取传感器设备信息
//  * @param  data_buff：数据缓冲区
//  * @note   数据格式参考datasheet
//  * @retval 成功返回 1，失败返回 0
//  */
//U8 ToF_Get_DeviceInfo(U8 *data_buff)
//{
//	U8 comm[10]={0x0A, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3C};
//	U8 read_buff[26] = {0};
//	U8 s_return = 0;
//	SCON0_RI = 0;//非中断方式串口接收，先清除UART 接收中断， 根据自己平台决定是否需要此语句
//	uart_write(comm, 10);
//	uart_read(read_buff, 26, 1);
//	s_return = packet_parser(read_buff, data_buff);
//	return s_return;
//}

/**
  * @brief  传感器开始连续测量
  * @param  无
  * @note   会开启外部中断触发数据接收
  * @retval 无
  */
void ToF_ContinuousStart(void)
{
	U8 comm[] = {0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x72};
//	enableEXINT0();
#ifndef USE_RADAR_FUN	
	t_osscomm_sendMessage(comm, 10, USART1);
#endif	
	t_osscomm_sendMessage(comm, 10, USART2);
	t_osscomm_sendMessage(comm, 10, USART3);
	t_osscomm_sendMessage(comm, 10, USART4);
}

///**
//  * @brief  设置传感器距离偏移值
//  * @param  offset：单位为毫米（mm）
//  * @note   可通过此方法校正传感器的近距离测量
//  * @retval 成功返回 1，失败返回 0
//  */
//U8 ToF_Set_DistanceOffset(S16 offset_mm)
//{
//					  //报头 命令	   key	v_MSB v_LSB  unuse unuse unuse crc_MSB crc_LSB
//	U8 comm[10]={0x0A, 0x38, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
////	U8 read_buff[5] = {0};
//	U8 s_return = 0;
//	U16 crc = 0x0000;
//	comm[3] = offset_mm >> 8;
//	comm[4] = offset_mm;
//	crc = CRC16_Cal(comm, 8);
//	comm[8] = crc >> 8;
//	comm[9] = crc;
//	SCON0_RI = 0;//非中断方式串口接收，先清除UART 接收中断， 根据自己平台决定是否需要此语句
//	uart_write(comm, 10);
//	uart_read(read_buff, 5, 1);
//	packet_parser(read_buff, &s_return);
//	return s_return;
//}


///**
//  * @brief  设置滤波器灵敏度
//  * @param  val:滤波器灵敏调节值
//  * @note   具体使用参考datasheet
//  * @retval 成功返回 1，失败返回 0
//  */
//U8 ToF_Set_Filter(S16 val)
//{
//					 //报头 命令	key	v_MSB v_LSB  unuse unuse unuse crc_MSB crc_LSB
//	U8 comm[10]={0x0A, 0x3D, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
////	U8 read_buff[5] = {0};
//	U8 s_return = 0;
//	U16 crc = 0x0000;
//	comm[3] = val >> 8;
//	comm[4] = val;
//	crc = CRC16_Cal(comm, 8);
//	comm[8] = crc >> 8;
//	comm[9] = crc;
//	uart_write(comm, 10);
//	uart_read(read_buff, 5, 1);
//	packet_parser(read_buff, &s_return);
//	return s_return;
//}

///**
//  * @brief  获取AFE温度值
//  * @param  afe_temp：温度值存储缓冲区,无符号整数，实际温度值等于此值除以100
//  * @note   先根据返回值判断是否解析成功，才使用afe_temp缓冲区的数据
//  * @retval 成功返回 1，失败返回 0
//  */
//U8 ToF_Get_AFETemp(U16 *afe_temp)
//{
//	U8 comm[10]={0x0A, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x86};
//	U8 read_buff[6] = {0};
//	U8 data_buff[2] = {0};
//	SCON0_RI = 0;
//	uart_write(comm, 10);
//	uart_read(read_buff, 6, 1);//返回包为6个字节
//	if(1 == packet_parser(read_buff, data_buff))
//	{
//		*afe_temp = (data_buff[0] << 8)|data_buff[1];
//		return 1;
//	}
//	return 0;
//}

//zkrt fuction start
/**
  * @brief  传感器返回的数据包解析
  * @param  
  *			1、packet：数据包首地址
  * @note   
  * @retval 解析成功返回 1，失败返回 0
  */
U8 ToF_parse(U8 *packet, U16 packet_len, U16 *distance)
{
	U16 crc16_cal = 0x0000;
	U16 crc16_packet = 0x0000;
	U8 len = packet[1];
	if(packet[0] != SENSOR_PACKET_START)
	{
		return 0;
	}
	if(len!= 0x0D)  //距离数据长度为0x0D
	{
		return 0;
	}	
	crc16_packet = packet[len]<<8;//取出数据包crc16
	crc16_packet |= packet[len+1];
	crc16_cal = CRC16_Cal(packet+2, len-2);
	
	if(crc16_cal != crc16_packet)
	{
		return 0;
	}
	sensorData dbuf;
	dbuf.distance = ((packet[5] << 8)|packet[6])/10;
	dbuf.magnitude = (packet[7] << 8)|packet[8];
	dbuf.magnitude_exp = packet[9];
	dbuf.ambient_adc = packet[10];
	dbuf.precision = (packet[11] << 8)|packet[12];

	if((float)((dbuf.magnitude)/100) < 43) //不可信时，将距离数据修改为无效数据
	{
		*distance = DISTANCE_NONE;
		return 1;
	}
	
	*distance = dbuf.distance;  //unit:cm
	
	if(*distance >5000)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
		*distance = DISTANCE_2HIGH;
	
	if(*distance <30)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
		*distance = DISTANCE_2LOW;
	
	return 1;	
}
//zkrt fuction end
