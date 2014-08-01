#include "hal_debug.h"

#include "fingerprint.h"
#include "stm32f4xx_hal.h"

//----指纹设备初始化------------------------------------------------------------
void FINT_DeviceInit(void)			
{
	TCDVDR_Init();
}
//----自检----------------------------------------------------------------------
bool FINT_SelfTest(void)
{
	int st_err;
	uint32 ZeroCount;
	
	st_err = PSIQualNVMOperation();
	if(st_err != STERR_OK)
		return FALSE;
	st_err = PSIQualRegisterDefaults(TCS1C);
	if(st_err != STERR_OK)
		return FALSE;
	st_err = PSIQualRegisterOperation();
	if(st_err != STERR_OK)
		return FALSE;
	st_err = PSIQualTestFIFO(&ZeroCount);
	if(st_err != STERR_OK)
		return FALSE;

	return TRUE;
}

long ttt,ttt1;


void FINT_Send(u8* ImgBuff)
{
	int y, x;
	u8 buff[230];

	for(y=0; y<224; y++)
	{
		//灰度分类
		for(x=0; x<224; x++, ImgBuff++)
		{
			if( *ImgBuff < 42 )
				buff[x] = ' ';
			else
			if( *ImgBuff < 84 )
				buff[x] = '.';
			else
			if( *ImgBuff < 126 )
				buff[x] = ':';
			else
			if( *ImgBuff < 168 )
				buff[x] = 'I';
			else
			if( *ImgBuff < 210 )
				buff[x] = 'Y';
			else
				buff[x] = 'M';
		}
		strcpy( (char*)&buff[x], "\r\n");
		//图象发送
		printf(buff);

	}


//	ttt1 = HAL_GetTick();
}


//----采集----------------------------------------------------------------------
bool FINT_gather(void)
{
	int st_err;
	SENSOR_DATA sensor_data;
	sint32 VendorCode;
	u8 *ImgBuff;

	ImgBuff = (uint8*)TCDVDR_Malloc(256*360);


	ttt = HAL_GetTick();

	st_err = STInitialize(&sensor_data);
	delay_ms(1);
	st_err = STAuthentify(&VendorCode);
	delay_ms(1);
	st_err = STGrab(ImgBuff, 224, 0, 1, 224, 0, 1);

	FINT_Send(ImgBuff);


	TCDVDR_Free(ImgBuff);

	if(st_err != STERR_OK)
		return FALSE;
	return TRUE;
}

//----校正----------------------------------------------------------------------
bool FINT_calibrate(void)
{
	u8 *BadCol, *BadRow;
	int st_err;

	st_err = calibrate();
	if(st_err != STERR_OK)
		return FALSE;
	delay_ms(10);
	BadCol = (uint8*)TCDVDR_Malloc(32*4);
	BadRow = (uint8*)TCDVDR_Malloc(32*4);
	st_err = GetBadLinesReport((sint32*)BadCol, (sint32*)BadRow);
	if(st_err != STERR_OK)
		return FALSE;
	TCDVDR_Free(BadCol);
	TCDVDR_Free(BadRow);
	return TRUE;
}
