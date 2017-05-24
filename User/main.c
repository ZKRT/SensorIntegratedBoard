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
#include "led.h"

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
//uint8_t testbuf[100]="HELLO"; //zkrt_debug
//uint16_t testbuf_len;
int main(void)
{
	BSP_Init();
	ostmr_wait(50);  //wait 5s 
  /* Infinite Loop */
	appdistance_init();
	appcan_init();
	appupload_init();
	appled_init();
	
  while(1)
  {
		/* product code start, the proposed process should not be delayed*/
		appdistance_prcs();	   																	//避障传感器处理流程
    appupload_prcs();                                       //定时上传传感器数据
		appled_process();                                       //LED流程
		/* product code end*/
		
//		/* test code start*/
//		
//		/* this is test routine of usart interface in user layer*/
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
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART5);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART6)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART6);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART7)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART7);
//		}	
//		if(t_osscomm_ReceiveMessage(testbuf, &testbuf_len, USART8)==SCOMM_RET_OK)
//		{
//			t_osscomm_sendMessage(testbuf, testbuf_len, USART8);
//		}			
//		
//		/* test code end*/
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/
