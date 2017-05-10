#include "can.h"

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

	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;//��ͨģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=8;
	
//������11�������󣬽���CAN��ʼ��		
	CAN_Init(CAN, &CAN_InitStructure);// ��ʼ��CAN1

//���ù�����0�������κ����ݣ�ֻ����ID=1������
	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
	CAN_FilterInitStructure.CAN_FilterIdHigh=(DEVICE_SELF_TYPE<<5); //32λID���������ID[3:0]
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK������0000������㲥
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

	CAN_ITConfig(CAN,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����
	
	return 0;
}

void CEC_CAN_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg RxMessage;
	
  CAN_Receive(CAN, CAN_FIFO0, &RxMessage);//CAN�豸���FIFO0�����ݣ���ȡ�����浽RxMessage�ṹ����
	
	for (i = 0; i < RxMessage.DLC; i++)//�������ݳ�������������洢����
	{
		can1_rx_buff[can1_rx_buff_store] = RxMessage.Data[i];
		
		can1_rx_buff_store++;
		if (can1_rx_buff_store == CAN_BUFFER_SIZE)
		{
			can1_rx_buff_store = 0;
		}
	}
//	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
//	led_rx_count = 1;
}

//ͨ���ж�get��store��λ�ù�ϵ�������Ƿ�����ֵ
uint8_t CAN1_rx_check(void)
{
	if (can1_rx_buff_store == can1_rx_buff_get)
		return 0;
	else 
		return 1;
}

//������ֵ��ʱ��ȡ��һ��ֵ
uint8_t CAN1_rx_byte(void)
{
	uint8_t ch;	
	
	ch = can1_rx_buff[can1_rx_buff_get];//��ch��¼�����յ���һ������
	
	can1_rx_buff_get++;
	if (can1_rx_buff_get == CAN_BUFFER_SIZE)
	{
		can1_rx_buff_get = 0;
	}
	
	return ch;
}

uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
{
  uint8_t mbox;
  uint16_t i=0;
  CanTxMsg TxMessage;						           
  TxMessage.StdId=(DEVICE_SELF_TYPE<<4);   
  TxMessage.ExtId=0x00;				             
  TxMessage.IDE=CAN_Id_Standard;          
  TxMessage.RTR=CAN_RTR_Data;		         
  TxMessage.DLC=len;						          
  for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];                
  
	mbox= CAN_Transmit(CAN, &TxMessage);    
	
  i=0;
  while((CAN_TransmitStatus(CAN, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	
	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
//	led_tx_count = 1;
	
  if(i>=0XFFF)
	{
		CAN_Mode_Init();
		return 1;
  }
	return 0;		//�ɹ�����
}

//����ģ������ݷ��ظ�����ģ��
uint8_t CAN1_send_message_fun(uint8_t *message, uint8_t len)
{
	//����һ��50����
	uint8_t count;		             
	uint8_t time;
	
	time = len/8;           
	
	for (count = 0; count < time; count++)
	{
		Can_Send_Msg(message, 8);
		message += 8;
//		delay_xms(1);
	}
	if (len%8)                         
	{
		Can_Send_Msg(message, len%8);
//		delay_xms(1);
	}
	
	return 0;
}
