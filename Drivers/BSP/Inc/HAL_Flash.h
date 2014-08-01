#ifndef __HAL_Flash_H
#define __HAL_Flash_H 	
//------------------------------------------------------------------------------
#include <string.h>
#include "MyTypedef.h"
#include "stm32f4xx_hal_flash.h"


void HAL_FlashRead(u32 addr, u8* buff, u16 bufflen);		//FLASH读参数
bool HAL_FlashWrite(u32 addr, u8* buff, u16 bufflen);		//FLASH随及写参数



//------------------------------------------------------------------------------
#endif
