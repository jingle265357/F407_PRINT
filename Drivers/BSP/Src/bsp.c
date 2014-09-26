#include"bsp.h"

void port_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	
	PRT_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = (DI | CLK | STEP_A | STEP_B | STEP_E | STB1 | STB2 | POWER_OPEN | LAT	);		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(PRT_GPIO, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = NO_PAPER;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PRT_GPIO, &GPIO_InitStruct); 


	GPIO_InitStruct.Pin = PAPER_FF;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(PRT_GPIO, &GPIO_InitStruct); 
	
	CLK_L();
	LAT_H();
}
