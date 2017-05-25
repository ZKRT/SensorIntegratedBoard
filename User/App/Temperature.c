/**
  ******************************************************************************
  * @file    TEMPERATURE.c 
  * @author  ZKRT
  * @version V1.0
  * @date    11-May-2017
  * @brief   infrared-GY-MCU90614
							and K type thermocouple
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
	PACKAGE:
	Byte0: 0x5A package head
	Byte1: 0x5A package head
	Byte2: 0X45 type of package(0x45:temperature value)
	Byte3: 0x04 length
	Byte4: 0x00~0xFF 	MSB1
	Byte5: 0x00~0xFF 	LSB1       
	Byte6: 0x00~0xFF 	MSB2
	Byte7: 0x00~0xFF 	LSB2
	Byte8: 0x00~0xFF 	CHECK
	Byte9: 0x01~0x15 	ID ,default is 0x14
	
	Calculate:
	temperature = MSB <<8 | LSB8
	
	Example:
		5A- 5A- 45- 04- 0C- 78- 0D- 19- A7
		
		TO =  0X0C78->3192/100 = 31.92 ℃
		TA =  0X0D19->3353/100 = 33.53 ℃
		
	COMMAND:
		*****
		
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
//#include "radar.h"
#include "Temperature.h"
#include "osusart.h"
#
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  parse infra-red data
  * @param  
  * @note   
  * @retval succeed 1，failed 0
  */

TemperatureSensor TSensor[7];
int8_t AtmosphereSensorNO = T_M1;
uint8_t CallSensorPackage[4];
volatile uint8_t TemperatureFlag=0;

void TemperatureInit(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  /* Configure pins as AF pushpull */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = RS485_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(RS485_PORT, &GPIO_InitStructure); 
	
	RS485_R;
}	
	
/*红外探头数据处理*/
void infrared_Parser(uint8_t infraredBuf[],uint16_t txLen,uint8_t ID)
{
		uint8_t sumCheck;
		uint8_t i;
	
		/*accumulation*/
		for(sumCheck=0,i=0;i<(infraredBuf[3]+4);i++)//infraredBuf[3]=4
			sumCheck+=infraredBuf[i]; 
		
		if(sumCheck==infraredBuf[i])//commpare
		{
			if(ID==T_LEFT)
			{
				TSensor[T_LEFT].ConnectionState = SENSOR_CONNECTED;
				TSensor[T_LEFT].TOMSB = 	infraredBuf[4];
				TSensor[T_LEFT].TOLSB	=		infraredBuf[5];
				TSensor[T_LEFT].TAMSB = 	infraredBuf[6];
				TSensor[T_LEFT].TALSB	=		infraredBuf[7];
				TSensor[T_LEFT].TOvalue = (infraredBuf[4]<<8)|infraredBuf[5];
				TSensor[T_LEFT].TAvalue = (infraredBuf[6]<<8)|infraredBuf[7];
			}
			else if(ID==T_RIGHT)
			{
				TSensor[T_RIGHT].ConnectionState = SENSOR_CONNECTED;
				TSensor[T_RIGHT].TOMSB 	= 	infraredBuf[4];
				TSensor[T_RIGHT].TOLSB	=		infraredBuf[5];
				TSensor[T_RIGHT].TAMSB 	= 	infraredBuf[6];
				TSensor[T_RIGHT].TALSB	=		infraredBuf[7];
				TSensor[T_RIGHT].TOvalue = (infraredBuf[4]<<8)|infraredBuf[5];
				TSensor[T_RIGHT].TAvalue = (infraredBuf[6]<<8)|infraredBuf[7];
			}
		}
}

/*数据清除*/
void AtmosphereClear(uint8_t begin,uint8_t end )
{
	uint8_t i=0;
	for(i=begin;i<end;i++)
		{
				TSensor[i].ConnectionState = SENSOR_DISCONNECTED;
				TSensor[i].TOMSB 	= 	0;
				TSensor[i].TOLSB	=		0;
				TSensor[i].TAMSB 	= 	0;
				TSensor[i].TALSB	=		0;
				TSensor[i].TOvalue = 0;
				TSensor[i].TAvalue = 0;
		}
}

void TimingGetSensorStatusTask()
{
	 
}


/*K型热电偶探头数据处理*/
void Atmosphere_Parser(uint8_t atmosphereBuf[],uint16_t txLen)
{
		uint8_t sumCheck;
		uint8_t i;
		uint8_t ID;
		
		/*accumulation*/
		for(sumCheck=0,i=0;i<(atmosphereBuf[3]+4);i++)//atmosphereBuf[3]=4
			sumCheck+=atmosphereBuf[i]; 
		
		if(sumCheck==atmosphereBuf[i++])//commpare
		{
				ID = atmosphereBuf[i];   //The last buf is the ID of sensor
			
				//AtmosphereClear(1,8);
			
				if(ID==T_M1)
				{		
						TSensor[T_M1].ConnectionState = SENSOR_CONNECTED;
						TSensor[T_M1].TOMSB 	= 	atmosphereBuf[4];
						TSensor[T_M1].TOLSB		=		atmosphereBuf[5];
						TSensor[T_M1].TAMSB 	= 	atmosphereBuf[6];
						TSensor[T_M1].TALSB		=		atmosphereBuf[7];
						TSensor[T_M1].TOvalue = (atmosphereBuf[4]<<8)|atmosphereBuf[5];
						TSensor[T_M1].TAvalue = (atmosphereBuf[6]<<8)|atmosphereBuf[7];
				}
				else if(ID==T_M2)
					{
						TSensor[T_M2].ConnectionState = SENSOR_CONNECTED;
						TSensor[T_M2].TOMSB 	= 	atmosphereBuf[4];
						TSensor[T_M2].TOLSB		=		atmosphereBuf[5];
						TSensor[T_M2].TAMSB 	= 	atmosphereBuf[6];
						TSensor[T_M2].TALSB		=		atmosphereBuf[7];
						TSensor[T_M2].TOvalue = (atmosphereBuf[4]<<8)|atmosphereBuf[5];
						TSensor[T_M2].TAvalue = (atmosphereBuf[6]<<8)|atmosphereBuf[7];
					}
				else if(ID==T_M3)
					{
						TSensor[T_M3].ConnectionState = SENSOR_CONNECTED;
						TSensor[T_M3].TOMSB 	= 	atmosphereBuf[4];
						TSensor[T_M3].TOLSB		=		atmosphereBuf[5];
						TSensor[T_M3].TAMSB 	= 	atmosphereBuf[6];
						TSensor[T_M3].TALSB		=		atmosphereBuf[7];
						TSensor[T_M3].TOvalue = (atmosphereBuf[4]<<8)|atmosphereBuf[5];
						TSensor[T_M3].TAvalue = (atmosphereBuf[6]<<8)|atmosphereBuf[7];
					}
				else if(ID==T_M4)
					{
						TSensor[T_M4].ConnectionState = SENSOR_CONNECTED;
						TSensor[T_M4].TOMSB 	= 	atmosphereBuf[4];
						TSensor[T_M4].TOLSB		=		atmosphereBuf[5];
						TSensor[T_M4].TAMSB 	= 	atmosphereBuf[6];
						TSensor[T_M4].TALSB		=		atmosphereBuf[7];
						TSensor[T_M4].TOvalue = (atmosphereBuf[4]<<8)|atmosphereBuf[5];
						TSensor[T_M4].TAvalue = (atmosphereBuf[6]<<8)|atmosphereBuf[7];
					}
		}
}


/*用于扫描K型热电偶探头*/
uint8_t  * ScanAtmosphereSensor(void)
{
		uint8_t sumCheck;
		int i=0,k;		
		
		if(AtmosphereSensorNO>T_M4)
			AtmosphereSensorNO = T_M1;
		else if(AtmosphereSensorNO<T_M1)
			AtmosphereSensorNO = T_M4;
		
		CallSensorPackage[i++] = 0xA5; 										//head
		CallSensorPackage[i++] = 0x85;										//cmd
		CallSensorPackage[i++] = AtmosphereSensorNO++;		//number
		
		for(sumCheck=0,k=0;k<3;k++)
				sumCheck+=CallSensorPackage[k]; 
		
		CallSensorPackage[i++]  = sumCheck;					//sum of check
		
		return CallSensorPackage;
}

/*扫描任务，标记*/
void TemperatureTask(void)
{
	if(!TemperatureFlag)
		TemperatureFlag = 1;
}

/*扫描空气探头*/
void TemperatureSensorScanTask(void)
{
		scommReturn_t SendFlag;
		//uint8_t temp[]={0xa5,0x85,0x01,0x2b};
		uint8_t *temp;
		temp = ScanAtmosphereSensor();
		RS485_T;
		SendFlag =t_osscomm_sendMessage(temp, 4, USART8);
		systmr_quickWait(1);
		RS485_R;
}

/*显示各个探头的值*/
void DisplayTemperatureValue(void)
{
		int i;
		for(i=1;i<7;i++)
		{
			if(TSensor[i].ConnectionState == SENSOR_CONNECTED)
					printf("[%d]%4d ",i ,TSensor[i].TOvalue/10);
			else				
					printf("[%d]%4d ",i ,0);

		}
		printf("\r\n");
}

/**
*@
*/
