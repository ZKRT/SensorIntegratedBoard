/**
  ******************************************************************************
  * @file    usart.c 
  * @author  ZKRT
  * @version V1.0
  * @date    13-December-2016
  * @brief   ��������
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
#include "hw_usart.h"
#include "usart.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	uint8_t UsartIndex = 0;
  /* 8xUSARTs configuration --------------------------------------------------*/
  /* 8xUSARTs  configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  for(UsartIndex = 0; UsartIndex < USART_MAX_INDEX; UsartIndex++)
  {
    USART_Init(UsartInstance[UsartIndex], &USART_InitStructure);
    
    /* Enable 8xUSARTs Receive interrupts */
    USART_ITConfig(UsartInstance[UsartIndex], USART_IT_RXNE, ENABLE);
    USART_ITConfig(UsartInstance[UsartIndex], USART_IT_TXE, DISABLE);
    /* Enable the 8xUSARTs */
    USART_Cmd(UsartInstance[UsartIndex], ENABLE);
		USART_ClearFlag(UsartInstance[UsartIndex], USART_FLAG_TC);
		USART_ClearFlag(UsartInstance[UsartIndex], USART_FLAG_RXNE);
  }
}

///////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  //�������´���,֧��printf����,������Ҫѡ��use MicroLIB
  */
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{ 
	x = x;
}
int fputc(int ch, FILE *f)
{
  while (USART_GetFlagStatus(USART8, USART_FLAG_TC) == RESET)
    ;
  USART_SendData(USART8, (unsigned char) ch);

  return (ch);
}
#endif
///////////////////////////////////////////////////////////////////////////////////

/**
*@
*/
