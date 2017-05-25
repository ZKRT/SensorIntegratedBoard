#include "hw_config.h"
#include "port.h"
#include "BSP.h"
#include "usart.h"
#include "ostmr.h"
#include "osqtmr.h"
#include "osusart.h"
#include "can.h"

static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);


/*
 * 函数名：BSP_Init
 * 描  述：时钟初始化、硬件初始化
 * 输  入：无
 * 输  出：无
 */
void BSP_Init(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
	this is done through SystemInit() function which is called from startup
	file (startup_stm32f0xx.s) before to branch to application main.
	To reconfigure the default setting of SystemInit() function, refer to
	system_stm32f0xx.c file
	*/ 
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
	/* error */ 
		while (1);
	}
	/* Configure clock GPIO, USARTs */
	RCC_Configuration();

	/* Configure GPIO Pin Tx/Rx for Usart communication */
	GPIO_Configuration();

	/* Configure NVIC */
	NVIC_Configuration();
	
	b_ostmr_init(); 
	b_systmr_init();
	os_usart_init();
	usart_config();
	CAN_Mode_Init();
}
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{   
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD|RCC_AHBPeriph_GPIOF, ENABLE);
  
  /* Enable 8xUSARTs Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_USART4|RCC_APB1Periph_USART5, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6|RCC_APB2Periph_USART7|RCC_APB2Periph_USART8|RCC_APB2Periph_USART1, ENABLE);
	
	/* Enable CAN Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /* USART2 Pins configuration ************************************************/     //change
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);                            
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  /* USART3 Pins configuration  ***********************************************/     //change
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  /* USART4 Pins configuration  ***********************************************/     //change
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_4);    
 
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  /* USART5 Pins configuration*************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_4);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* USART6 Pins configuration*************************************************/    //change
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  /* USART7 Pins configuration  *************************************************/    //change
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
    
  /* USART8 Pins configuration  *************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
  /*CAN Pins configuration  *************************************************/    //add 
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	
	/*LED Pins configuration  *************************************************/    //add 
  /* Connect pin to Periph */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 	
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIMER_IRQ_CHANNEL_1ST IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ_CHANNEL_1ST;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* TIMER_IRQ_CHANNEL_2ND IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ_CHANNEL_2ND;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* USART1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* USART2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* USART3_8 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_8_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* CAN IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

