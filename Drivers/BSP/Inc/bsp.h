#ifndef __bsp_h__
#define __bsp_h__

#include "MyTypedef.h"
#include "STM32F4xx_HAL_CONF.h"


#define PRT_CLK_ENABLE()		__GPIOB_CLK_ENABLE()
#define DI						GPIO_PIN_0
#define CLK						GPIO_PIN_1
#define STEP_A				GPIO_PIN_2//步进电机A相//
#define STEP_B				GPIO_PIN_3//步进电机B相//
#define STEP_E				GPIO_PIN_4//步进电机控制有效//
#define STB1					GPIO_PIN_5//加热头1//
#define STB2					GPIO_PIN_6//加热头2//
#define POWER_OPEN		GPIO_PIN_7
#define LAT						GPIO_PIN_8
#define NO_PAPER			GPIO_PIN_9
#define PRT_GPIO						GPIOB



#define DI_L()					PRT_GPIO->BSRRH=DI
#define DI_H()					PRT_GPIO->BSRRL=DI
#define CLK_L()					PRT_GPIO->BSRRH=CLK
#define CLK_H()					PRT_GPIO->BSRRL=CLK
#define STEP_A_L()			PRT_GPIO->BSRRH=STEP_A
#define STEP_A_H()			PRT_GPIO->BSRRL=STEP_A
#define STEP_B_L()			PRT_GPIO->BSRRH=STEP_B
#define STEP_B_H()			PRT_GPIO->BSRRL=STEP_B
#define STEP_E_L()			PRT_GPIO->BSRRH=STEP_E
#define STEP_E_H()			PRT_GPIO->BSRRL=STEP_E
#define STB1_L()				PRT_GPIO->BSRRH=STB1
#define STB1_H()				PRT_GPIO->BSRRL=STB1
#define STB2_L()				PRT_GPIO->BSRRH=STB2
#define STB2_H()				PRT_GPIO->BSRRL=STB2
#define POWER_OPEN_L()	PRT_GPIO->BSRRH=POWER_OPEN
#define POWER_OPEN_H()	PRT_GPIO->BSRRL=POWER_OPEN
#define LAT_L()					PRT_GPIO->BSRRH=LAT
#define LAT_H()					PRT_GPIO->BSRRL=LAT
#define NO_PAPER_L()		PRT_GPIO->BSRRH=NO_PAPER
#define NO_PAPER_H()		PRT_GPIO->BSRRL=NO_PAPER


void port_init(void);

#endif
