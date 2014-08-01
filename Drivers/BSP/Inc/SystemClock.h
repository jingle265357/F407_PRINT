#ifndef __HAL_SystemClock_H
#define __HAL_SystemClock_H 	
//------------------------------------------------------------------------------
#include "STM32F4xx_HAL_CONF.h"
#include "MyTypedef.h"



void SystemClock_Config(void);
void delay_mini(__IO u16 s);
void delay_ms(__IO u16 nms);
void delay_us(__IO u16 nus);


//------------------------------------------------------------------------------
#endif
