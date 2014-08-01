#include "stm32f2xx.h"
#include "TCS1CT.h"


#define TCS_CS_0()	GPIOB->BSRRL=GPIO_Pin_8
#define TCS_CS_1()	GPIOB->BSRRH=GPIO_Pin_8
#define TCS_RST_0()	GPIOB->BSRRL=GPIO_Pin_9
#define TCS_RST_1()	GPIOB->BSRRH=GPIO_Pin_9
#define TCS_A0_0()	GPIOC->BSRRL=GPIO_Pin_11		//寄存器
#define TCS_A0_1()	GPIOC->BSRRH=GPIO_Pin_11		//数据
#define TCS_WR_0()	GPIOC->BSRRL=GPIO_Pin_12
#define TCS_WR_1()	GPIOC->BSRRH=GPIO_Pin_12
#define TCS_RD_0()	GPIOD->BSRRL=GPIO_Pin_2
#define TCS_RD_1()	GPIOD->BSRRH=GPIO_Pin_2

#define TCS_VCC_0()	GPIOC->BSRRL=GPIO_Pin_0
#define TCS_VCC_1()	GPIOC->BSRRH=GPIO_Pin_0

#define TCS_Input()		GPIOB->IDR
#define TCS_Outout(x)	GPIOB->ODR=(GPIOB->ODR&0xff00)|(x&0x00FF); //输出//GPIOB->ODR=data


/*
////////////////////////////////////////////////////////////////////////////////
//----设置总线为输入方式--------------------------------------------------------
static void TCS_DatabusSetIn(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//DATA-BUS
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	TCS_Outout(0xFF);
}
//----设置总线为输出方式--------------------------------------------------------
static void TCS_DatabusSetOut(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//DATA-BUS
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

}
*/
//----读寄存器------------------------------------------------------------------
u8 TCS_ReadReg(u8 reg)
{
	u8 d;

	TCS_CS_0();

	//写寄存器
	TCS_DatabusSetOut();		//设置总线为输出方式
	TCS_A0_0();
	TCS_Outout(reg);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	//读数据
	TCS_DatabusSetIn();			//设置总线为输入方式
	TCS_A0_1();
	TCS_RD_0();
	Delay(1);
	d = TCS_Input();
	Delay(1);
	TCS_RD_1();
	
	TCS_CS_1();
	return d;
}
//----写寄存器------------------------------------------------------------------
void TCS_WriteReg(u8 reg, u8 data)
{
	TCS_CS_0();

	//写寄存器
	TCS_DatabusSetOut();		//设置总线为输出方式
	TCS_A0_0();
	TCS_Outout(reg);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	//写数据
	TCS_A0_1();
	TCS_Outout(data);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	TCS_CS_1();
}


//----读寄存器------------------------------------------------------------------
void TCDVDR_ReadRegister(u8 reg, u32* data)
{
	TCS_CS_0();

	//写寄存器
	TCS_DatabusSetOut();		//设置总线为输出方式
	TCS_A0_0();
	TCS_Outout(reg);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	//读数据
	TCS_DatabusSetIn();			//设置总线为输入方式
	TCS_A0_1();
	TCS_RD_0();
	Delay(1);
	*data = TCS_Input();
	Delay(1);
	TCS_RD_1();
	
	TCS_CS_1();
}
//----写寄存器------------------------------------------------------------------
void TCDVDR_WriteRegister(u8 reg, u32 data)
{
	TCS_CS_0();

	//写寄存器
	TCS_DatabusSetOut();		//设置总线为输出方式
	TCS_A0_0();
	TCS_Outout(reg);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	//写数据
	TCS_A0_1();
	TCS_Outout(data);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	TCS_CS_1();
}
//------------------------------------------------------------------------------
s32 TCDVDR_GrabImage(u8 *ImgBuf, s32 BufSize)
{
	u8 datnm;
	u8 *tmpbuff = ImgBuf;
	u32 nSize = BufSize;

	TCS_CS_0();

	//写寄存器
	TCS_DatabusSetOut();		//设置总线为输出方式
	TCS_A0_0();
	TCS_Outout(0xFF);
	TCS_WR_0();
	Delay(1);
	TCS_WR_1();

	//读数据
	TCS_DatabusSetIn();			//设置总线为输入方式
	TCS_A0_1();
	while(nSize)
	{
		do
		{
			TCS_RD_0();
			Delay(1);
			datnm = TCS_Input();
			TCS_RD_1();
		}while(datnm == 0);

		*tmpbuff = datnm;
		tmpbuff++;
		nSize--;
	}	
	TCS_CS_1();
	return 0;

}

////////////////////////////////////////////////////////////////////////////////
/*
//----设置GPIO控制信号----------------------------------------------------------
void TCS_GPIO_Init(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	//CS,RESET
	TCS_CS_1();
	TCS_RST_1();
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	//A0,WR,VCC
	TCS_A0_1();
	TCS_WR_1();
	TCS_VCC_0();
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_0;				
   	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	//RD
	TCS_RD_1();
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				
   	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	//设置总线为输入方式
	TCS_DatabusSetIn();

}
//----检测寄存器----------------------------------------------------------------
static bool TCS_CheckRegisterDefaults(void)
{
    u8 d;
	u8 test1=0x5A, test2=0x99;

	//检测初始数值
	d = TCS_ReadReg(0x00);
    if(d != 0x9E)
		return FALSE;

	d = TCS_ReadReg(0x10);
    if(d != 0x00)
		return FALSE;

	//检测写入数值（无害的寄存器: 0x00, 0x07, 0x08, 0x10, 0x12, 0x14, 0x15, 0x17, 0x19）
	TCS_WriteReg(0x00, test1);
	d = TCS_ReadReg(0x00);
	if(d != test1)
		return FALSE;

	TCS_WriteReg(0x10, test2);
	d = TCS_ReadReg(0x10);
	if(d != test2)
		return FALSE;

	return TRUE;
}
//----TCS1CT芯片初始化----------------------------------------------------------
bool TCS_Init(void)
{
	TCS_GPIO_Init();
	TCS_VCC_1();
	delay_ms(500);
	TCS_RST_0();
	delay_ms(100);
	TCS_RST_1();
	return TCS_CheckRegisterDefaults();
}
  */
////////////////////////////////////////////////////////////////////////////////



