/**
  ******************************************************************************
  * @file    appcan.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   can应用数据通信
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
#include "sys.h"
#include "can.h"
#include "appcan.h"
#include "appdistance.h"
#include "appcan.h"
#include "commonzkrt.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Can_upload_packet_t Upload_info_st;     //can 定时上传心跳包结构体

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  appcan_init
  * @param  None
  * @retval None
  */
void appcan_init(void)
{
	Upload_info_st.start_code = START_CODE;
	Upload_info_st.end_code = END_CODE;
	Upload_info_st.datalen = CANUP_DATALEN;
	Upload_info_st.cmd_code = HEARTBEAT_CMD;
}
/**
  * @brief  appcan_hb_pack  心跳包封装
  * @param  None
  * @retval None
  */
void appcan_hb_pack(void)
{
	//distance pack
	Upload_info_st.front_D = global_distance.distance_data[FRONT_NUM];
	Upload_info_st.left_D = global_distance.distance_data[LEFT_NUM];
	Upload_info_st.back_D = global_distance.distance_data[BACK_NUM];
	Upload_info_st.right_D = global_distance.distance_data[RIGHT_NUM];
	//temperature pack  //zkrt_todo: 温度封包 曾工赶紧做
	
	//crc pack
	Upload_info_st.crc = CRC16_Cal(((uint8_t*)&Upload_info_st)+3, Upload_info_st.datalen);
}
/**
  * @brief  appcan_hbpacket_send  心跳包封装
  * @param  None
  * @retval None
  */
uint8_t appcan_hbpacket_send(void)
{
	uint8_t ret;
	appcan_hb_pack();
	ret = CAN1_send_message_fun((uint8_t*)&Upload_info_st, Upload_info_st.datalen+5);  //len=data len+datalen+start+end+crc2
	return ret;
}

/**
*@
*/
