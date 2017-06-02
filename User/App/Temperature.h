/**
  ******************************************************************************
  * @file    TEMPERATURE.h 
  * @author  ZKRT
  * @version V1.0
  * @date    11-May-2017
  * @brief   
	*					 + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H 
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#define RS485_PORT		GPIOA
#define RS485_PIN			GPIO_Pin_8
	
#define RS485_T   GPIO_SetBits(RS485_PORT,RS485_PIN)
#define RS485_R		GPIO_ResetBits(RS485_PORT,RS485_PIN)
	
#define DisplayData  1
	
typedef enum SENSOR_INDEX
{
	T_M1=0x01,				
	T_M2,
	T_M3,
	T_M4,
	T_LEFT,
	T_RIGHT
}SENSOR_ID;

typedef enum CONNECTION_STATE
{
	SENSOR_CONNECTED = 10,
	SENSOR_DISCONNECTED
}CONNECTION_STATE;

typedef enum WORK_STATE
{
	SENSOR_NORMAL = 20,
	SENSOR_ABNORMAL
}WORK_STATE;


typedef struct TEMPERATURE_DATA
{
	uint8_t 	type;  						/*Non-contact type or contact type*/
	uint8_t 	ID;								/*number of the sensor*/
	uint8_t 	ConnectionState;	/*Connection state of sensor with board*/
	uint8_t 	workState;				/*the work state of sensor*/
	uint8_t   TOMSB;
	uint8_t 	TOLSB;
	uint8_t		TAMSB;
	uint8_t 	TALSB;
	int16_t 	TOvalue;					/*TO value of temperature*/
	int16_t 	TAvalue;					/*TA value of temperature*/
	uint8_t 	freq;							/*frequence of sensor updata*/
}TemperatureSensor;

/* Exported constants --------------------------------------------------------*/
extern volatile uint8_t TemperatureFlag;
extern TemperatureSensor TSensor[7];
extern int16_t BodyTemperature;
/* Exported functions ------------------------------------------------------- */
void infrared_Parser(uint8_t infraredBuf[],uint16_t txLen,uint8_t ID);
void Atmosphere_Parser(uint8_t atmosphereBuf[],uint16_t txLen);
uint8_t  * ScanAtmosphereSensor(void);
void timertask(void);
void DisplayTemperatureValue(void);
void TemperatureSensorScanTask(void);
void TemperatureTask(void);
void TemperatureInit(void);
void TemperatureDataProcess(void);
void Temperature(void);
#endif /* __INFRARED_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

