/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  */ 

#ifndef __HW_USART_H
#define __HW_USART_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "port.h"

#define USART_MAX_INDEX   8

#define USART_DATA_SIZE      		200

#define USART_TX_DATA_SIZE      USART_DATA_SIZE
#define USART_RX_DATA_SIZE      USART_DATA_SIZE

#define UART0_FRM_TMR_RESTART     12       /*uart1 frame timer 12ms*/
#define UART0_CHAR_TMR_RESTART    2        /*uart1 char timer 4ms*//*此时间应大于接收1字节数据所需时间,所以不同波特率
                                                该值也应不一样.值为4时,bps须大于2400bps,值为2时,bps须大于4800*/  //zkrt_debug

#define UART_CHANNEL_IDLE			0					
#define UART_CHANNEL_BUSY			1
#define UART_CHANNEL_BUSY_TIMEROUT		3	//3s

/* Private macro -------------------------------------------------------------*/
typedef struct
{
  uint16_t              scommTxLen;                         /*tx data length*/
  uint8_t              *pscommTxContent;
}scommTxBuf_t;

typedef struct
{
  uint16_t              scommRcvLen;
  uint8_t              *pscommRcvContent;
}scommRcvBuf_t;
typedef enum
{
  URX_BUF_IDLE = 0,
  URX_BUF_BUSY,        /**/
  URX_BUF_COMPL,       /*receiver complete, but frame format haven't been checken*/          
  URX_BUF_READY        /*receiver complete, and frame format ok*/
}uRxBufStatus_t;    

typedef enum
{
  UTX_BUF_IDLE = 0,
  UTX_BUF_BUSY,        /**/
  UTX_BUF_COMPL,       /*receiver complete, but frame format haven't been checken*/          
  UTX_BUF_READY        /*receiver complete, and frame format ok*/
}uTxBufStatus_t;  

typedef enum
{
  SCOMM_RET_NONE = 0,
  SCOMM_RET_ERR_PARAM,
	SCOMM_RET_TXING,
  SCOMM_RET_TIMEOUT,
  SCOMM_RET_NOREADY,
  SCOMM_RET_OK,
  SCOMM_RET_PORT_ERR,
}scommReturn_t;

typedef enum 
{
  COM1 = 0,
  COM4 = 1,
	COM5 = 2
} COM_TypeDef;  
extern void _USART1_IRQHandler(void);
extern void _USART2_IRQHandler(void);
extern void _MYUSART3_IRQHandler(void);
extern void _USART4_IRQHandler(void);
extern USART_TypeDef* UsartInstance[USART_MAX_INDEX];
#endif
