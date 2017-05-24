#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h" 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//#define _RUN_LED                                     PBout(13)
//#define _CAN_LED                                     PBout(14)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void appled_init(void);
void appled_process(void);

extern volatile uint8_t _can_led_cnt;
#endif
