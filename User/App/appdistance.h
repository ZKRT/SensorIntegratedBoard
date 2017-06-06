/**
  ******************************************************************************
  * @file    appdistance.h 
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
#ifndef __APPDISTANCE_H
#define __APPDISTANCE_H 
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* Exported macro ------------------------------------------------------------*/
#define FRONT_NUM      0
#define RIGHT_NUM      1
#define BACK_NUM       2
#define LEFT_NUM       3

#define RADAR_NUM		   FRONT_NUM	
#define TOFLEFT_NUM		 LEFT_NUM	
#define TOFBACK_NUM		 BACK_NUM	
#define TOFRIGHT_NUM	 RIGHT_NUM	

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t distance_data[4];  //距离
}distance_info_t;	
/* Exported constants --------------------------------------------------------*/

//distance meaning
#define DISTANCE_NONE                 5000
#define DISTANCE_2LOW                 7000   //<30
#define DISTANCE_2HIGH                8000   //>5000

//distacne position adjust
////DJI植保飞机
#define D_P_FRONT                     65      //zkrt_todo 视实际安装情况，宏值做调整。
#define D_P_LEFT                      65
#define D_P_BACK                      65
#define D_P_RIGHT                     65

/* Exported functions ------------------------------------------------------- */
void appdistance_prcs(void);
void appdistance_init(void);

extern distance_info_t global_distance;
#endif /* __APPDISTANCE_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

