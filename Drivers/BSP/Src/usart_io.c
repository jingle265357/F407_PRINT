#include "stm32f4xx_hal.h"
#include "usart_io.h"
#include<string.h>


/* USART6 init function */
//void MX_USART6_UART_Init(com,baudrate,wor,t)
//{

//  HalUartHandle6.UartHandle.Instance = USART6;
//  HalUartHandle6.UartHandle.Init.BaudRate = 115200;
//  HalUartHandle6.UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
//  HalUartHandle6.UartHandle.Init.StopBits = UART_STOPBITS_1;
//  HalUartHandle6.UartHandle.Init.Parity = UART_PARITY_NONE;
//  HalUartHandle6.UartHandle.Init.Mode = UART_MODE_TX_RX;
//  HalUartHandle6.UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  HalUartHandle6.UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
//  HAL_UART_Init(&(HalUartHandle6.UartHandle));

//}

void USART_IO_Config(USER_UART_HandleTypeDef *pUartHandle, USART_TypeDef *uart, uint32_t baudrate,
											uint32_t worthlength, uint32_t stopbits, uint32_t parity, uint32_t mode, uint32_t hwflowctl, uint32_t oversampling )
{
				pUartHandle->UartHandle.Instance = uart;
				pUartHandle->UartHandle.Init.BaudRate = baudrate;
				pUartHandle->UartHandle.Init.WordLength = worthlength;
				pUartHandle->UartHandle.Init.StopBits = stopbits;
				pUartHandle->UartHandle.Init.Parity = parity;
				pUartHandle->UartHandle.Init.Mode = mode;
				pUartHandle->UartHandle.Init.HwFlowCtl = hwflowctl;
				pUartHandle->UartHandle.Init.OverSampling = oversampling;
}
	

/*
wrote by fan to build 2 buffer for a uart receive
*/
int USART_IO_Open(USER_UART_HandleTypeDef* UartHandle, int flag)
{

	flag = flag;

	if(UartHandle==NULL)
			return -1;
	//pBuffTable[0]
	HAL_UART_Init(&(UartHandle->UartHandle));
	UartHandle->UpBufferPtr = UartHandle->BuffB;
	UartHandle->UpBufferCount = 0;	
	UartHandle->UartHandle.State = HAL_UART_STATE_READY;
	HAL_UART_Receive_IT((UART_HandleTypeDef*) &(UartHandle->UartHandle), UartHandle->BuffA, HAL_BUFFER_SIZE);
	return 1;
}
//uint8_t* pBuff[2] = {HalBufferA, HalBufferB}; 
//the buff of low
//return :
//0:  HalBUfferA
//1:  HalBufferB
//2:  error
static FLIPBUFF_TABLE WhichLowBuffer(USER_UART_HandleTypeDef*  pUserUartHandle)
{
	if(((pUserUartHandle->UartHandle.pRxBuffPtr) >= pUserUartHandle->BuffA)&&((pUserUartHandle->UartHandle.pRxBuffPtr) <= (pUserUartHandle->BuffA+HAL_BUFFER_SIZE)))	
		return BUFFA;                                                                    
	else if((pUserUartHandle->UartHandle.pRxBuffPtr >= pUserUartHandle->BuffB)&&(pUserUartHandle->UartHandle.pRxBuffPtr <= (pUserUartHandle->BuffB+HAL_BUFFER_SIZE)))	
		return BUFFB;
	else
		return ERR;	
}


static uint8_t Hal_FlipBUff(USER_UART_HandleTypeDef *pHandle)
{
			uint8_t tmp;

			tmp = WhichLowBuffer(pHandle); 	
			if(tmp==ERR)
					return 0;
			//disable interrupt
			__HAL_UART_DISABLE_IT(&(pHandle->UartHandle), UART_IT_RXNE);  
			//save low buff to upbuff
			
			if(tmp==0)
						pHandle->UpBufferPtr = pHandle->BuffA;
			else
						pHandle->UpBufferPtr = pHandle->BuffB;
			pHandle->UpBufferCount =  pHandle->UartHandle.RxXferSize - pHandle->UartHandle.RxXferCount;		
			//change the uart state
//			if(pHandle->UartHandle.State==HAL_UART_STATE_BUSY_TX_RX) 
//					pHandle->UartHandle.State = HAL_UART_STATE_BUSY_TX;
//			else
//					pHandle->UartHandle.State = HAL_UART_STATE_READY;
//			
//			//flip buffer
//			if(tmp==0)
//					HAL_UART_Receive_IT(&(pHandle->UartHandle), pHandle->BuffB, HAL_BUFFER_SIZE);
//			else
//					HAL_UART_Receive_IT(&(pHandle->UartHandle), pHandle->BuffA, HAL_BUFFER_SIZE);
			if((pHandle->UartHandle.State==HAL_UART_STATE_BUSY_TX_RX)||(pHandle->UartHandle.State==HAL_UART_STATE_BUSY_RX))
			{
				
				if(tmp==0)
						pHandle->UartHandle.pRxBuffPtr = pHandle->BuffB;
				else
						pHandle->UartHandle.pRxBuffPtr = pHandle->BuffA;
				pHandle->UartHandle.RxXferSize = HAL_BUFFER_SIZE;
				pHandle->UartHandle.RxXferCount = HAL_BUFFER_SIZE;
		  }
			else
			{
					if(tmp==0)
							HAL_UART_Receive_IT(&(pHandle->UartHandle), pHandle->BuffB, HAL_BUFFER_SIZE);
					else
							HAL_UART_Receive_IT(&(pHandle->UartHandle), pHandle->BuffA, HAL_BUFFER_SIZE);
				
			}
					
			
			__HAL_UART_ENABLE_IT(&(pHandle->UartHandle), UART_IT_RXNE);
			
			return 1;
}
static uint16_t GetDataFromUpBuffer(USER_UART_HandleTypeDef* pHandle, uint8_t* pBuff, uint16_t len)
{
	uint16_t count = 0;
	uint8_t tmp;
	
	if(pHandle->UpBufferCount > len)
	{
		memcpy(pBuff, pHandle->UpBufferPtr, len);
		pHandle->UpBufferPtr += len;
		pHandle->UpBufferCount -= len;
		return len;		
	}
	else
	{
		tmp = WhichLowBuffer(pHandle);
		memcpy(pBuff, pHandle->UpBufferPtr, pHandle->UpBufferCount);
		if(tmp==0)
				pHandle->UpBufferPtr = pHandle->BuffA;
		else if(tmp==1)
				pHandle->UpBufferPtr = pHandle->BuffB;
		else
			return 0;
//		pHandle->UpBufferPtr = pBuffTable[WhichLowBuffer(pHandle)];
		count = pHandle->UpBufferCount;
		pHandle->UpBufferCount = 0;
		/*insert flip buff*/
//		Hal_FlipBUff(pHandle);		
		return  count;
				
	}	
}

int USART_IO_Read(USER_UART_HandleTypeDef* pUserUartHandle,unsigned char *buffer, int len)
{

	if((len==0)||(len>HAL_BUFFER_SIZE))
		 return 	0;
	if(pUserUartHandle->UpBufferCount > 0)
	{	
			return GetDataFromUpBuffer(pUserUartHandle, buffer,len);

	}
	else
	{ 
		if(pUserUartHandle->UartHandle.RxXferCount < pUserUartHandle->UartHandle.RxXferSize)	// have data in buff
		{
		  /**flip buff**/
			Hal_FlipBUff(pUserUartHandle);
			return GetDataFromUpBuffer(pUserUartHandle, buffer,len);
		}
		else
			return 0;
	}		
}

int USART_IO_ReadCount(USER_UART_HandleTypeDef *pHandle)
{
			if(pHandle==NULL)
					return -1;
			if(pHandle->UpBufferCount>0)
					return pHandle->UpBufferCount;
			else
					return pHandle->UartHandle.RxXferSize - pHandle->UartHandle.RxXferCount;
			
			
}

int USART_IO_Write(USER_UART_HandleTypeDef *pHandle, unsigned char *buff, unsigned int len)
{
		if(HAL_UART_Transmit_IT(&pHandle->UartHandle, buff, len)==HAL_OK)
				return len;
		else 
				return -1;
	
}


