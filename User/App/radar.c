/**
  ******************************************************************************
  * @file    radar.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   雷达避障传感器数据解析
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "radar.h"
#include "osusart.h"
#include "appdistance.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  传感器启动
  * @param  
  * @note   
  * @retval 
  */
void radar_Start(void)
{
	uint8_t comm[] = {0xAA, 0xAA, 0x00, 0x02, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55};
//	enableEXINT0();
	t_osscomm_sendMessage(comm, 14, USART1);  //停止
	ostmr_wait(1);  //wait 100ms
	t_osscomm_sendMessage(comm, 14, USART1);  //停止
	ostmr_wait(1);  //wait 100ms
	comm[4] = 0x02;
	comm[5] = 0x00;
	t_osscomm_sendMessage(comm, 14, USART1);  //获取设备
	ostmr_wait(1);  //wait 100ms
	comm[4] = 0xFE;
	comm[5] = 0x00;
	t_osscomm_sendMessage(comm, 14, USART1);  //开始-连接设备
	ostmr_wait(2);  //wait 100ms
	t_osscomm_sendMessage(comm, 14, USART1);  //开始-连接设备
	ostmr_wait(1);  //wait 100ms
}
/**
  * @brief  传感器启动发送距离信息
  * @param  
  * @note   
  * @retval 
  */
void radar_easy_Start(void)
{
	uint8_t comm[] = {0xAA, 0xAA, 0x00, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55};
	t_osscomm_sendMessage(comm, 14, USART1);  //开始-连接设备
//	ostmr_wait(1);  //wait 100ms
//	t_osscomm_sendMessage(comm, 14, USART1);  //开始-连接设备
}
/**
  * @brief  传感器返回的数据包解析
  * @param  
  * @note   
  * @retval 解析成功返回 1，失败返回 0
  */
//uint8_t radar_taget_no=0; //目标个数
//uint8_t radar_parse(const uint8_t *packet, uint16_t packet_len, uint16_t *distance)
//{
//	uint8_t ret =0;
//	radarData info;
//	uint16_t start_seq;
//	uint16_t end_seq;
//	uint16_t messageid_seq;
//	
//	if(packet_len <14)
//	{
//		return ret;
//	}	
//	start_seq = (packet[1] << 8)|packet[0];
//	end_seq = (packet[13] << 8)|packet[12];
//	messageid_seq = (packet[3] << 8)|packet[2];
//	
//	if(start_seq != 0xAAAA)
//	{
//		return ret;
//	}
//	if(end_seq != 0x5555)
//	{
//		return ret;
//	}
//	if(messageid_seq != 0x70C)  //0x70c目标输出信息，0x70b目标输出状态
//	{
//		if(messageid_seq ==0x70b)
//		{
//			if(packet[4] >0)  //目标个数
//			{
//				radar_taget_no = packet[4];
//				//存在目标
//			}	
//			else
//			{
//				radar_taget_no = 0;
//				*distance = 5000;  //接收到无目标状态时，将目标距离修改改为无效距离数据50m。
//				ret =1;
//			}
//		}
//		if(packet_len-14 >=14)
//		{
//			ret = radar_parse(packet+14, packet_len-14, distance);
//			return ret;
//		}
//		else
//			return ret;
//	}
//	
//	info.id = packet[4];
//	info.rcs = packet[5]*0.5-50;
//	info.range = (packet[6]*0x100+packet[7]);  //unit:cm
////	info.rollcount = 0;
////	info.snr = packet[11]-127;	
//	
////	if(info.range >=500)  //zkrt_debug
////	{
////		return 0;
////	}	
//	*distance = info.range;
//	
//	if(*distance >5000)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
//		*distance = 6000;
//	
//	if(*distance <30)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
//		*distance = 7000;
//	
////	printf("%d\n", *distance);
//	return 1;	
//}
uint8_t radar_parse(const uint8_t *packet, uint16_t packet_len, uint16_t *distance)
{
	uint8_t ret =0;
	radarData info;
	uint16_t start_seq;
	uint16_t end_seq;
	uint16_t messageid_seq;
	
	if(packet_len <14)
	{
		return ret;
	}	
	start_seq = (packet[1] << 8)|packet[0];
	end_seq = (packet[13] << 8)|packet[12];
	messageid_seq = (packet[3] << 8)|packet[2];
	
	if(start_seq != 0xAAAA)
	{
		return ret;
	}
	if(end_seq != 0x5555)
	{
		return ret;
	}
	if(messageid_seq != 0x70C)  //0x70c目标输出信息
	{
		if(packet_len-14 >=14)
		{
			ret = radar_parse(packet+14, packet_len-14, distance);
			return ret;
		}
		else
			return ret;
	}
	info.id = packet[4];
	info.rcs = packet[5]*0.5-50;
	info.range = (packet[6]*0x100+packet[7]);  //unit:cm
//	info.rollcount = 0;
//	info.snr = packet[11]-127;	
	*distance = info.range;
	if(*distance >5000)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
		*distance = DISTANCE_2HIGH;
	
	if(*distance <30)  //zkrt_notice:务必放在最后，如果此处还需要做数据处理务必考虑此处
		*distance = DISTANCE_2LOW;
	
	return 1;	
}
//uint8_t radar_parse(const uint8_t *packet, uint16_t packet_len, uint16_t *distance)
//{
//	radarData info;
//	uint16_t start_seq;
//	uint16_t end_seq;
//	uint16_t messageid_seq;
//	
//	if(packet_len !=42)
//	{
//		return 0;
//	}	
//	start_seq = (packet[1+28] << 8)|packet[0+28];
//	end_seq = (packet[13+28] << 8)|packet[12+28];
//	messageid_seq = (packet[3+28] << 8)|packet[2+28];
//	if(start_seq != 0xAAAA)
//	{
//		return 0;
//	}
//	if(messageid_seq != 0x70C)
//	{
//		return 0;
//	}	
//	if(end_seq != 0x5555)
//	{
//		return 0;
//	}
//	info.id = packet[4+28];
//	info.rcs = packet[5+28]*0.5-50;
//	info.range = (packet[6+28]*0x100+packet[7+28]);  //unit:cm
//	if(info.range >=500)
//	{
//		return 0;
//	}	
//	*distance = info.range;
//	printf("%d\n", *distance);
////	info.rollcount = 0;
////	info.snr = packet[11]-127;
//	return 1;	
//}
/**
*@
*/
