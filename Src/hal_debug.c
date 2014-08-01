#include "stm32f4xx_hal.h"


#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE() 

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_5
#define USARTx_TX_GPIO_PORT              GPIOD  
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_6
#define USARTx_RX_GPIO_PORT              GPIOD 
#define USARTx_RX_AF                     GPIO_AF7_USART2

UART_HandleTypeDef UartHandle;


int sendchar(int ch)
{    
  /* Transmit Data */
  UartHandle.Instance ->DR = (ch & (uint16_t)0x1FF);	
  while((UartHandle.Instance ->SR & 0x0040) == (uint16_t)RESET);	

  return ch;
}


//int getKey(void)
//{
//	if(UARTX->SR&(1<<5))
//		return UARTX->DR & 0X0FF;	
//	else return -1;
//}

void UartInit(void)
{
		uint32_t tmpreg = 0x00;
		GPIO_InitTypeDef GPIO_InitStruct;

	
	
		UartHandle.Instance        = USARTx;
		UartHandle.Init.BaudRate   = 115200;
		UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
		UartHandle.Init.StopBits   = UART_STOPBITS_1;
		UartHandle.Init.Parity     = UART_PARITY_NONE;
		UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
		UartHandle.Init.Mode       = UART_MODE_TX_RX;

			/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		USARTx_TX_GPIO_CLK_ENABLE();
		USARTx_RX_GPIO_CLK_ENABLE();
		/* Enable USART2 clock */
		USARTx_CLK_ENABLE(); 
	
		  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
		
		
		__HAL_UART_DISABLE(&UartHandle);
		  /*-------------------------- USART CR2 Configuration -----------------------*/
		tmpreg = UartHandle.Instance->CR2;

		/* Clear STOP[13:12] bits */
		tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);

		/* Configure the UART Stop Bits: Set STOP[13:12] bits according to UartHandle->Init.StopBits value */
		tmpreg |= (uint32_t)UartHandle.Init.StopBits;
		
		/* Write to USART CR2 */
		UartHandle.Instance->CR2 = (uint32_t)tmpreg;

		/*-------------------------- USART CR1 Configuration -----------------------*/
		tmpreg = UartHandle.Instance->CR1;

		/* Clear M, PCE, PS, TE and RE bits */
		tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | \
																		 USART_CR1_RE | USART_CR1_OVER8));

		/* Configure the UART Word Length, Parity and mode: 
			 Set the M bits according to UartHandle.Init.WordLength value 
			 Set PCE and PS bits according to UartHandle.Init.Parity value
			 Set TE and RE bits according to UartHandle.Init.Mode value
			 Set OVER8 bit according to UartHandle.Init.OverSampling value */
		tmpreg |= (uint32_t)UartHandle.Init.WordLength | UartHandle.Init.Parity | UartHandle.Init.Mode | UartHandle.Init.OverSampling;
		
		/* Write to USART CR1 */
		UartHandle.Instance->CR1 = (uint32_t)tmpreg;
		
		/*-------------------------- USART CR3 Configuration -----------------------*/  
		tmpreg = UartHandle.Instance->CR3;
		
		/* Clear CTSE and RTSE bits */
		tmpreg &= (uint32_t)~((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE));
		
		/* Configure the UART HFC: Set CTSE and RTSE bits according to UartHandle->Init.HwFlowCtl value */
		tmpreg |= UartHandle.Init.HwFlowCtl;
		
		/* Write to USART CR3 */
		UartHandle.Instance->CR3 = (uint32_t)tmpreg;
		
		/* Check the Over Sampling */
		if(UartHandle.Init.OverSampling == UART_OVERSAMPLING_8)
		{
			/*-------------------------- USART BRR Configuration ---------------------*/
			if((UartHandle.Instance == USART1) || (UartHandle.Instance == USART6))
			{
				UartHandle.Instance->BRR = __UART_BRR_SAMPLING8(HAL_RCC_GetPCLK2Freq(), UartHandle.Init.BaudRate);
			}
			else
			{
				UartHandle.Instance->BRR = __UART_BRR_SAMPLING8(HAL_RCC_GetPCLK1Freq(), UartHandle.Init.BaudRate);
			}
		}
		else
		{
			/*-------------------------- USART BRR Configuration ---------------------*/
			if((UartHandle.Instance == USART1) || (UartHandle.Instance == USART6))
			{
				UartHandle.Instance->BRR = __UART_BRR_SAMPLING16(HAL_RCC_GetPCLK2Freq(), UartHandle.Init.BaudRate);
			}
			else
			{
				UartHandle.Instance->BRR = __UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), UartHandle.Init.BaudRate);
			}
		}
		
		
		  UartHandle.Instance->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
			UartHandle.Instance->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
		
			UartHandle.Instance->SR = (~0x0040);
			__HAL_UART_ENABLE(&UartHandle);

}	


