#ifndef __HAL_Bus8Bit_H
#define __HAL_Bus8Bit_H 			   
//////////////////////////////////////////////////////////////////////////////////	 
#include "MyTypedef.h"
//#include "MidSystemClock.h"

#include "STM32F4xx_HAL_CONF.h"



//8Bit数据、地址总线
#define BUS_8Bit_CLK_ENABLE()	__GPIOB_CLK_ENABLE()
#define BUS_8Bit_PIN			(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
#define BUS_8Bit_GPIO			GPIOB

//RD WR 控制线
#define BUS_WR_CLK_ENABLE()		__GPIOB_CLK_ENABLE()
#define BUS_WR_PIN				GPIO_PIN_8
#define BUS_WR_GPIO				GPIOB
#define BUS_RD_CLK_ENABLE()		__GPIOB_CLK_ENABLE()
#define BUS_RD_PIN				GPIO_PIN_9
#define BUS_RD_GPIO				GPIOB
#define BUS_CD_CLK_ENABLE()		__GPIOE_CLK_ENABLE()
#define BUS_CD_PIN				GPIO_PIN_0
#define BUS_CD_GPIO				GPIOE

//功能控制线
#define BUS_SW1_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define BUS_SW1_PIN				GPIO_PIN_1
#define BUS_SW1_GPIO			GPIOE
#define BUS_SW2_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define BUS_SW2_PIN				GPIO_PIN_2
#define BUS_SW2_GPIO			GPIOE

//片选控制线
#define BUS_CS1_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define BUS_CS1_PIN				GPIO_PIN_3
#define BUS_CS1_GPIO			GPIOE
#define BUS_CS2_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define BUS_CS2_PIN				GPIO_PIN_4
#define BUS_CS2_GPIO			GPIOE


//------------------------------------------------------------------------------
//8Bit数据、地址总线
#define BUS_Input()		BUS_8Bit_GPIO->IDR
#define BUS_Outout(x)	(BUS_8Bit_GPIO->ODR = (  0xFF00 & BUS_8Bit_GPIO->ODR ) | ( 0x00FF & x ) ) //输出//GPIOB->ODR=data
//RD WR 控制线
#define BUS_WR_L()	BUS_WR_GPIO->BSRRH=BUS_WR_PIN
#define BUS_WR_H()	BUS_WR_GPIO->BSRRL=BUS_WR_PIN
#define BUS_RD_L()	BUS_RD_GPIO->BSRRH=BUS_RD_PIN
#define BUS_RD_H()	BUS_RD_GPIO->BSRRL=BUS_RD_PIN
#define BUS_CD_L()	BUS_CD_GPIO->BSRRH=BUS_CD_PIN		
#define BUS_CD_H()	BUS_CD_GPIO->BSRRL=BUS_CD_PIN		
//功能控制线
#define BUS_SW1_L()	BUS_SW1_GPIO->BSRRH=BUS_SW1_PIN
#define BUS_SW1_H()	BUS_SW1_GPIO->BSRRL=BUS_SW1_PIN
#define BUS_SW2_L()	BUS_SW2_GPIO->BSRRH=BUS_SW2_PIN
#define BUS_SW2_H()	BUS_SW2_GPIO->BSRRL=BUS_SW2_PIN
//片选控制线
#define BUS_CS1_L()	BUS_CS1_GPIO->BSRRH=BUS_CS1_PIN
#define BUS_CS1_H()	BUS_CS1_GPIO->BSRRL=BUS_CS1_PIN
#define BUS_CS2_L()	BUS_CS2_GPIO->BSRRH=BUS_CS2_PIN
#define BUS_CS2_H()	BUS_CS2_GPIO->BSRRL=BUS_CS2_PIN


////////////////////////////////////////////////////////////////////////////////// 
void HAL_Bus8BitInit(u16 WaitTime);		//总线接口初始化

void HAL_Bus8BitWriteCtr(u16 ch);		//总线写控制字节
void HAL_Bus8BitWriteData(u16 ch);		//总线写数据字节
u16 HAL_Bus8BitReadCtr(void);			//总线读控制字节
u16 HAL_Bus8BitReadData(void);			//总线读数据字节



#endif

