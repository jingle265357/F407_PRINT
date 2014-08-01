#include "hal_debug.h"

#include "stm32f4xx_hal.h"

#define UARTX   USART6

char debug_buff[100];


int sendchar(int ch)
{    
  /* Transmit Data */
  UARTX->DR = (ch & (uint16_t)0x1FF);	
  while((UARTX->SR & 0x0040) == (uint16_t)RESET);	

  return ch;
}

//int getKey(void)
//{
//	if(UARTX->SR&(1<<5))
//		return UARTX->DR & 0X0FF;	
//	else return -1;
//}


