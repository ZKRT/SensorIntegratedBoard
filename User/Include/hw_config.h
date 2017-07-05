/**
  ******************************************************************************
  * @file    hw_config.h 
  * @author  ZKRT
  * @version V1.0
  * @date    13-December-2016
  * @brief   hardware configure message
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported macro ------------------------------------------------------------*/
#define USE_RADAR_FUN
#define DisplayData  0  //zkrt_debug

/** @defgroup hardware open or close control
  * @{
  */             
//#define USE_LED_FUN			


/**
  * @}
  */ 

//unit test debug
//#define CanSend2SubModule_TEST   //zkrt_debug


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup clock label
  * @{
  */ 
#define SYSTEM_CLK	              48
#define APB_CLK		                48
#define AHB_CLK                   48
#define APB_TIMER_CLK	            48

/**
  * @}
  */ 

/** @defgroup fast timer task config
  * @{
  */ 
#define	RCC_TIMER_1ST																RCC_APB1Periph_TIM6
#define	TIMER_CHANNEL_1ST														TIM6
#define	TIMER_IRQ_CHANNEL_1ST												TIM6_DAC_IRQn
/**
  * @}
  */ 

/** @defgroup quick fast timer config
  * @{
  */ 
#define	RCC_TIMER_2ND																RCC_APB1Periph_TIM7
#define	TIMER_CHANNEL_2ND														TIM7
#define	TIMER_IRQ_CHANNEL_2ND												TIM7_IRQn
/**
  * @}
  */ 

/** @defgroup pin define 
  * @{
  */ 
/**
  * @}
  */ 


/* Exported functions ------------------------------------------------------- */

#endif /* __HW_CONFIG_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

