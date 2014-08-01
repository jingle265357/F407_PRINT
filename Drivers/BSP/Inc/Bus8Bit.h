#ifndef __HalBus8Bit_H
#define __HalBus8Bit_H 			   
//////////////////////////////////////////////////////////////////////////////////	 
#include "MyTypedef.h"
//#include "MidSystemClock.h"

#include "STM32F4xx_HAL_CONF.h"

////////////////////////////////////////////////////////////////////////////////// 
void HAL_Bus8BitInit(u16 WaitTime);		//总线接口初始化

void HAL_Bus8BitWriteCtr(u16 ch);		//总线写控制字节
void HAL_Bus8BitWriteData(u16 ch);		//总线写数据字节
u16 HAL_Bus8BitReadCtr(void);			//总线读控制字节
u16 HAL_Bus8BitReadData(void);			//总线读数据字节



#endif

