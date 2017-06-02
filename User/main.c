/**
  ******************************************************************************
  * @file    main.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "BSP.h"
#include "sys.h"
#include "osusart.h"
#include "appdistance.h"
#include "appcan.h"
#include "appupload.h"
#include "Temperature.h"
#include "led.h"
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint8_t testbuf[100]="HELLO";
uint16_t testbuf_len;
int main(void)
{
	BSP_Init();
	ostmr_wait(50);  //wait 5 senconds for sensor for ready
	TemperatureInit();
  /* Infinite Loop */
	appdistance_init();
	appcan_init();
	appupload_init();
	appled_init();
	
	t_systmr_insertQuickTask(TemperatureTask,50,OSTMR_PERIODIC);
	//t_ostmr_insertTask(,500,OSTMR_PERIODIC);
	
  while(1)
  {
		/* product code start, the proposed process should not be delayed*/
		appdistance_prcs();	   																	//避障传感器处理流程
    appupload_prcs();                                       //定时上传传感器数据
		appled_process();                                       //LED流程
		/* product code end*/
		
//		/* test code start*/
//		
//		if(TemperatureFlag)
//		{
//			TemperatureSensorScanTask();
//			TemperatureFlag = 0;
//		}
//			DisplayTemperatureValue();
		
		/* this is test routine of usart interface in user layer*/
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART1)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART1);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART2)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART2);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART3)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART3);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART4)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART4);
//		}			
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART5)==SCOMM_RET_OK)
//		{
//			//t_osscomm_sendMessage(testbuf, testbuf_len, USART5);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART6)==SCOMM_RET_OK)
//		{
//				/*get left temperature sensor value*/
//				infrared_Parser(testbuf,testbuf_len,T_LEFT);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART7)==SCOMM_RET_OK)
//		{
//				/*get right temperature sensor value*/
//				infrared_Parser(testbuf,testbuf_len,T_RIGHT);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART8)==SCOMM_RET_OK)
//		{
//				/*get atmosphere temperature sensors value*/
//				Atmosphere_Parser(testbuf,testbuf_len);
//		}
//		
//		/* test code end*/
			Temperature();
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/
