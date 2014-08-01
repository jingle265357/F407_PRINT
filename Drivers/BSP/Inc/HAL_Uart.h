#ifndef __HAL_Uart_H
#define __HAL_Uart_H

////////////////////////////////////////////////////////////////////////////////////
#include "MyTypedef.h"
#include "STM32F4xx_HAL_CONF.h"
#include "STM32F4xx_HAL_UART.h"


#ifndef COM1
	#define COM1 0x00
#endif

#ifndef COM2
	#define COM2 0x01
#endif

#ifndef COM3
	#define COM3 0x02
#endif

bool ComOpen(u8 com, u32 baud, u8 width, u8 stop, u8 par);

//----初始化UART----------------------------------------------------------------
void ComInit(u8 com, u32 baud, u8 width, u8 stop, u8 par);
//----获取接收字节数------------------------------------------------------------
u16 ComGetRecvCnt(u8 com);
//----读取数据------------------------------------------------------------------
bool ComRecvDatas(u8 com, u8 *buff, u16 lenth);
//----清除接收缓冲区------------------------------------------------------------
void ComClearRecvBuff(u8 com);
//----发送字符串----------------------------------------------------------------
void ComSendDatas(u8 com, u8* buff, u16 lenth);


#endif
