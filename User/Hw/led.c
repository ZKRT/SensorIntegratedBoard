/**
  ******************************************************************************
  * @file    led.c
  * @author  ZKRT
  * @version V1.0
  * @date    18-May-2017
  * @brief   led
  *           + (1)...
								PB13£ºRUN LED
								PB14£ºCAN SEND LED						
  *         
 ===============================================================================
  * @attention
  *
  * ...
  *
  ******************************************************************************  
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t _run_led_flag;
volatile uint8_t _run_led_cnt;
volatile uint8_t _can_led_flag;
volatile uint8_t _can_led_cnt;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  led_msTask.
  * @param  None
  * @retval None
  */
void led_msTask(void)
{
	if(_run_led_cnt)
	{
		_run_led_cnt--;
		if(!_run_led_cnt)
		{
			_run_led_flag = 1;
		}
	}	
	if(_can_led_cnt)
	{
		_can_led_cnt--;
		if(!_can_led_cnt)
		{
			_can_led_flag = 1;
		}
	}		
}
/**
  * @brief  led_init.
  * @param  None
  * @retval None
  */
void appled_init(void)
{
//	_RUN_LED = 0;
//	_CAN_LED = 0;
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
  _run_led_flag = 0;
	_run_led_cnt = 5;
	_can_led_cnt = 0;
  _can_led_flag = 0;
	t_ostmr_insertTask(led_msTask, 100, OSTMR_PERIODIC); //100ms task
}
/**
  * @brief  led_process. ledµÆ¿ØÖÆ
  * @param  None
  * @retval None
  */
void appled_process(void)
{
	if(_can_led_flag)
	{
		_can_led_flag = 0;
//		_can_led_cnt = 0;
//		_CAN_LED = 1;
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
	}
	
	if(_run_led_flag)
	{
		_run_led_flag = 0;
		_run_led_cnt = 5;
//		_RUN_LED= ~_RUN_LED;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		}
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		}	
	}	
}	
/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/
