#ifndef __HalDmaQueue_H
#define __HalDmaQueue_H

////////////////////////////////////////////////////////////////////////////////////
#include "STM32F4xx_HAL_CONF.h"
#include "MyTypedef.h"

//DMA队列结构
typedef struct {
    __IO u8	*out; 		//指向数据输出位置 
    __IO u8	*sta;		//指向Buf的开始位置
    __IO u8	*end;		//指向Buf的结束位置
    __IO u16 ndata;		//队列中有效数据字节数   
    __IO u16 space;		//队列的数据字节空间
	__IO u16 LastDmaCurrCnt;	//DMA最后一次剩余的待传输数据数目
}QueuePrar;

//--------------------------------------------------------------------------
void QueueCreate(u8 *databuff, u16 space, QueuePrar *Qprar);
u16 QueueNdata(u16 DmaCurrCnt, QueuePrar *Qprar);
char QueueGetch(QueuePrar *Qprar);
bool QueueRead(u8 *databuff, QueuePrar *Qprar, __IO u16 lenth);
bool QueueRead2(u8 *databuff, QueuePrar *Qprar, __IO u16 lenth);
void QueueFlush(__IO u16 ndata, QueuePrar *Qprar);										//清除队列

//////////////////////////////////////////////////////////////////////////////////	 
#endif
