/**
  ******************************************************************************
  * @file    radar.h 
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
#ifndef __RADAR_H
#define __RADAR_H 
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t id;
	uint8_t rcs;
	uint16_t range;
	uint8_t rollcount;
	uint8_t snr;	
}radarData;

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void radar_Start(void);
void radar_easy_Start(void);
uint8_t radar_parse(const uint8_t *packet, uint16_t packet_len, uint16_t *distance);
#endif /* __RADAR_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

