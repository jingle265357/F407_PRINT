

#include "hal_debug.h"


#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "SPI.h"


//------------------------------------------------------------------------------
#define TCS_CS_L()		BUS_CS1_L()
#define TCS_CS_H()		BUS_CS1_H()
#define TCS_RST_L()	   	BUS_SW1_L()
#define TCS_RST_H()		BUS_SW1_H()	


//------------------------------------------------------------------------------
void TCDVDR_Init(void)
{
	//执行一次传感器复位
	TCS_RST_L();
	delay_ms(100);
	TCS_RST_H();
}
//----打开TC电源----------------------------------------------------------------
void TCDVDR_power_on_TC(void)
{
	//执行一次传感器复位
	TCS_RST_L();
	delay_ms(10);
	TCS_RST_H();
}
//----关闭TC电源----------------------------------------------------------------
void TCDVDR_power_off_TC(void)
{

}
//----写寄存器------------------------------------------------------------------
void TCDVDR_WriteRegister(uint8 AddrPort, uint32 Data)
{
	TCS_CS_L();
	HAL_Bus8BitWriteCtr(AddrPort);
	HAL_Bus8BitWriteData(Data);
	TCS_CS_H();
}
//----读寄存器------------------------------------------------------------------
void TCDVDR_ReadRegister(uint8 AddrPort, uint32 *Data)
{
	TCS_CS_L();
	HAL_Bus8BitWriteCtr(AddrPort);
	*Data = HAL_Bus8BitReadData();
	TCS_CS_H();
}
//----读取全部IMAGE-------------------------------------------------------------
sint32 TCDVDR_GrabImage(uint8 *ImgBuf, sint32 BufSize)
{
	u8 datnm;
	u8 *tmpbuff = ImgBuf;
	u32 cnt;

#ifdef M_DEBUG
	//sprintf(debug_buff,	"TCDVDR_GrabImage: %ld \r\n", BufSize);
	printf("TCDVDR_GrabImage: %ld \r\n",BufSize);
#endif

	TCS_CS_L();
	HAL_Bus8BitWriteCtr(0xFF);
	for(cnt=0; cnt<BufSize; cnt++)
	{
		do
		{
			datnm = HAL_Bus8BitReadData();
		}
		while(datnm == 0);	//过滤0

		*tmpbuff = datnm;
		tmpbuff++;
	}
	TCS_CS_H();
	return STERR_OK;
}

//----写入NVM-------------------------------------------------------------------
sint32 TCDVDR_NVMWrite(uint8 *src, uint32 offset, uint32 length)
{
#ifdef M_DEBUG
//	sprintf(debug_buff,	"TCDVDR_NVMWrite: %ld ", length);
//	printf(debug_buff);
#endif

	if( HAL_FlashWrite(offset, src, length) )
	{
#ifdef M_DEBUG
	printf("OK \r\n");
#endif
		return STERR_OK;
	}
	else
	{
#ifdef M_DEBUG
	printf("ERR \r\n");
#endif
		return STERR_ERROR;	
	}
}

//----读取NVM-------------------------------------------------------------------
sint32 TCDVDR_NVMRead(uint8 *dest, uint32 offset, uint32 length)
{
#ifdef M_DEBUG
//	sprintf(debug_buff,	"TCDVDR_NVMRead: %ld \r\n", length);
//	printf(debug_buff);
#endif

	HAL_FlashRead(offset, dest, length);
	return STERR_OK;
}

//----SLEEP---------------------------------------------------------------------
void TCDVDR_Sleep(uint32 period)
{
	__IO u32 i;	

	for(i=0; i<10000; i++);
}

//------------------------------------------------------------------------------
void* TCDVDR_Malloc(uint32 nSize)
{
#ifdef M_DEBUG
//	sprintf(debug_buff,	"TCDVDR_Malloc: %ld \r\n", nSize);
//	printf(debug_buff);
#endif

	return malloc(nSize);
}

//------------------------------------------------------------------------------
void TCDVDR_Free(void *ptr)
{
#ifdef M_DEBUG
	printf("TCDVDR_Free\r\n");
#endif

	free(ptr);
}



