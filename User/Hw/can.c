#include "can.h"
#include "led.h"

volatile uint8_t can1_rx_buff[CAN_BUFFER_SIZE];
volatile uint16_t can1_rx_buff_store = 0;
uint16_t can1_rx_buff_get = 0;

uint8_t CAN_Mode_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	CAN_DeInit(CAN);
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;

	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;//普通模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=8;
	
//设置完11个参数后，进行CAN初始化		
	CAN_Init(CAN, &CAN_InitStructure);// 初始化CAN1

//设置过滤器0，接收任何数据，只接受ID=1的数据
	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh=(DEVICE_SELF_TYPE<<5); //32位ID，这个保存ID[3:0]
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK，保存0000，代表广播
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

	CAN_ITConfig(CAN,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许
	
	return 0;
}

void CEC_CAN_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg RxMessage;
	
  CAN_Receive(CAN, CAN_FIFO0, &RxMessage);//CAN设备里的FIFO0的数据，读取出来存到RxMessage结构体里
	
	for (i = 0; i < RxMessage.DLC; i++)//按照数据长度往缓冲区里存储数据
	{
		can1_rx_buff[can1_rx_buff_store] = RxMessage.Data[i];
		
		can1_rx_buff_store++;
		if (can1_rx_buff_store == CAN_BUFFER_SIZE)
		{
			can1_rx_buff_store = 0;
		}
	}
	if(!_can_led_cnt)
	{	
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		_can_led_cnt = 5;
	}
}

//通过判断get和store的位置关系来决定是否有新值
uint8_t CAN1_rx_check(void)
{
	if (can1_rx_buff_store == can1_rx_buff_get)
		return 0;
	else 
		return 1;
}

//当有新值的时候，取出一个值
uint8_t CAN1_rx_byte(void)
{
	uint8_t ch;	
	
	ch = can1_rx_buff[can1_rx_buff_get];//用ch记录下来收到的一个数据
	
	can1_rx_buff_get++;
	if (can1_rx_buff_get == CAN_BUFFER_SIZE)
	{
		can1_rx_buff_get = 0;
	}
	
	return ch;
}
//uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
//{
//	uint8_t status;
//  uint8_t mbox;
//  uint16_t i=0;
//  CanTxMsg TxMessage;						           
//  TxMessage.StdId=(DEVICE_SELF_TYPE<<4);   
//  TxMessage.ExtId=0x00;				             
//  TxMessage.IDE=CAN_Id_Standard;          
//  TxMessage.RTR=CAN_RTR_Data;		         
//  TxMessage.DLC=len;						          
//  for(i=0;i<len;i++)
//	TxMessage.Data[i]=msg[i];                
//  printf("11111111111111111111\n");
//	mbox= CAN_Transmit(CAN, &TxMessage);
//  i=0;
//  while((CAN_TransmitStatus(CAN, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	
//	
////	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
////	led_tx_count = 1;
//	
//  if(i>=0XFFF)
//	{
//		CAN_Mode_Init();
//		printf("can send fail\n");
//		return 1;
//  }
//	return 0;		//成功返回
//}
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
{
	uint8_t status;
  uint8_t mbox;
  uint16_t i=0;
  CanTxMsg TxMessage;
	
//pack data	
  TxMessage.StdId=(DEVICE_SELF_TYPE<<4);   
  TxMessage.ExtId=0x00;				             
  TxMessage.IDE=CAN_Id_Standard;          
  TxMessage.RTR=CAN_RTR_Data;		         
  TxMessage.DLC=len;						          
  for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];
	
//transmit	
	i=0;
	mbox= CAN_Transmit(CAN, &TxMessage); 
	while((mbox==CAN_TxStatus_NoMailBox)&&(i<0x4FFF))
	{
		i++;
		mbox= CAN_Transmit(CAN, &TxMessage); 
//		printf("CAN_TxStatus_NoMailBox fail\n");
	}	
	if(mbox==CAN_TxStatus_NoMailBox)
	{
//		printf("can CAN_TxStatus_NoMailBox fail exceed\n");
//		CAN_Mode_Init();  //zkrt_notice 不应该重启
		return 1;
	}	
//check transmitstatus	
  i=0;
	status = CAN_TransmitStatus(CAN, mbox);
	while((status!=CAN_TxStatus_Ok)&&(i<0x4FFF))
	{
		i++;
		status = CAN_TransmitStatus(CAN, mbox);
	}	
  if(i>=0x4FFF)
	{
		if(status == CAN_TxStatus_Pending)
		{
			printf("can mbox[%d] CAN_TxStatus_Pending\n", mbox);
		}
		else if(status == CAN_TxStatus_Failed)
		{
			CAN_Mode_Init();
			printf("can mbox[%d] CAN_TxStatus_Failed\n", mbox);
			return 1;
		}
  }
//	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
//	led_tx_count = 1;	
	return 0;		//成功返回
}

//将子模块的数据返回给管理模块
uint8_t CAN1_send_message_fun(uint8_t *message, uint8_t len)
{
	//假设一共50个字
	uint8_t count;		             
	uint8_t time;
	uint8_t ret;
	
	time = len/8;           
	
	for (count = 0; count < time; count++)
	{
		ret = Can_Send_Msg(message, 8);
		if(ret)
			return ret;
		message += 8;
	}
	if (len%8)                         
	{
		ret = Can_Send_Msg(message, len%8);
	}
	return ret;
}
