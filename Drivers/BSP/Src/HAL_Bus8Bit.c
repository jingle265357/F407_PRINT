
#include "HAL_Bus8Bit.h"



////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
static u16 BusWaitTime;		//低速总线等待时间
static void delay_bus_mini(u16 cnt)
{
	
}



////////////////////////////////////////////////////////////////////////////////
//----总线接口初始化------------------------------------------------------------
void HAL_Bus8BitInit( u16 WaitTime )
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//配置8Bit总线接口
    BUS_8Bit_CLK_ENABLE();
	BUS_Outout(0xFF);

   	GPIO_InitStruct.Pin = BUS_8Bit_PIN;		
 	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  	GPIO_InitStruct.Pull = GPIO_PULLUP;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
   	HAL_GPIO_Init(BUS_8Bit_GPIO, &GPIO_InitStruct); 

	//----
	//配置控制接口
	BUS_WR_CLK_ENABLE();
	BUS_RD_CLK_ENABLE();
	BUS_CD_CLK_ENABLE();
	BUS_SW1_CLK_ENABLE();
	BUS_SW2_CLK_ENABLE();
	BUS_CS1_CLK_ENABLE();
	BUS_CS2_CLK_ENABLE();

	//控制接口全部置高电平
	BUS_WR_H();
	BUS_RD_H();
	BUS_CD_H();
	BUS_SW1_H();
	BUS_SW2_H();
	BUS_CS1_H();
	BUS_CS2_H();

  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_PULLUP;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

   	GPIO_InitStruct.Pin = BUS_WR_PIN;		//WR				
   	HAL_GPIO_Init(BUS_WR_GPIO, &GPIO_InitStruct); 
   	GPIO_InitStruct.Pin = BUS_RD_PIN;		//RD
   	HAL_GPIO_Init(BUS_RD_GPIO, &GPIO_InitStruct); 
   	GPIO_InitStruct.Pin = BUS_CD_PIN;		//CD
   	HAL_GPIO_Init(BUS_CD_GPIO, &GPIO_InitStruct);
	 
   	GPIO_InitStruct.Pin = BUS_SW1_PIN;		//
   	HAL_GPIO_Init(BUS_SW1_GPIO, &GPIO_InitStruct); 
   	GPIO_InitStruct.Pin = BUS_SW2_PIN;		//
   	HAL_GPIO_Init(BUS_SW2_GPIO, &GPIO_InitStruct); 

   	GPIO_InitStruct.Pin = BUS_CS1_PIN;		//
   	HAL_GPIO_Init(BUS_CS1_GPIO, &GPIO_InitStruct); 
   	GPIO_InitStruct.Pin = BUS_CS2_PIN;		//
   	HAL_GPIO_Init(BUS_CS2_GPIO, &GPIO_InitStruct); 

	//配置低速总线等待时间
//	BusWaitTime = WaitTime;
}


//----总线写控制字节------------------------------------------------------------
void HAL_Bus8BitWriteCtr(u16 ch)
{
	BUS_CD_L();
	BUS_Outout(ch);
	BUS_WR_L();
	delay_bus_mini(BusWaitTime);
	BUS_WR_H();
	BUS_Outout(0xFF);
}
//----总线写数据字节------------------------------------------------------------
void HAL_Bus8BitWriteData(u16 ch)
{
	BUS_CD_H();
	BUS_Outout(ch);
	BUS_WR_L();
	delay_bus_mini(BusWaitTime);
	BUS_WR_H();
	BUS_Outout(0xFF);
}
//----总线读控制字节------------------------------------------------------------
u16 HAL_Bus8BitReadCtr(void)
{
	u32 ch;

	BUS_CD_L();
	BUS_RD_L();
	delay_bus_mini(BusWaitTime);
	ch = BUS_Input();
	BUS_RD_H();
	return ch & 0x000000FF;
}
//----总线读数据字节------------------------------------------------------------
u16 HAL_Bus8BitReadData(void)
{
	u32 ch;

	BUS_CD_H();
	BUS_RD_L();
	delay_bus_mini(BusWaitTime);
	ch = BUS_Input();
	BUS_RD_H();
	return ch & 0x000000FF;
}

