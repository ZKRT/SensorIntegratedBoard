/**
  ******************************************************************************
  * @file    appcan.h 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   
	*					 + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPCAN_H
#define __APPCAN_H 
/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported macro ------------------------------------------------------------*/
#define START_CODE										0xEB
#define END_CODE										  0xBE

#define CANUP_DATALEN									0x18

#define HEARTBEAT_CMD								  0x12

/* Exported types ------------------------------------------------------------*/
#pragma pack(push, 1)
typedef struct _zkrt_packet_t
{
	uint8_t start_code;
	uint8_t cmd_code;
	uint8_t datalen;
	uint16_t reserved;
	uint16_t front_D;
	uint16_t right_D;
	uint16_t back_D;
	uint16_t left_D;
	uint16_t body_T;
	uint16_t m1_T;
	uint16_t m2_T;
	uint16_t m3_T;
	uint16_t m4_T;
	uint16_t left_T;
	uint16_t right_T;
	uint16_t crc;
	uint8_t end_code;
}Can_upload_packet_t;
#pragma pack(pop)
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void appcan_init(void);
void appcan_hb_pack(void);
uint8_t appcan_hbpacket_send(void);
#endif /* __APPCAN_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

