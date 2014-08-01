#ifndef __USART_IO_H_
#define __USART_IO_H_


#include "stm32f4xx_hal.h"


#define HAL_BUFFER_SIZE	  256

typedef struct
{
	UART_HandleTypeDef 	UartHandle;
	uint16_t UpBufferCount;
	unsigned char* UpBufferPtr;
	unsigned char BuffA[HAL_BUFFER_SIZE]; 
	unsigned char BuffB[HAL_BUFFER_SIZE];
}USER_UART_HandleTypeDef;    //fan


typedef enum 
{
  BUFFA       = 0x00,
  BUFFB       = 0x01,
  ERR         = 0x02,
}FLIPBUFF_TABLE;



/*************added by fan for flip buffer*******************************************/

int USART_IO_Open(USER_UART_HandleTypeDef* UartHandle, int flag);
//uint8_t WhichBuffer(USER_UART_HandleTypeDef*  pUserUartHandle);
//uint16_t GetDataFromUpBuffer(USER_UART_HandleTypeDef* pHandle, uint8_t* pBuff, uint16_t len);
int USART_IO_Read(USER_UART_HandleTypeDef* pUserUartHandle, unsigned char *buffer, int len);
void USART_IO_Config(USER_UART_HandleTypeDef *pUartHandle, USART_TypeDef *uart, uint32_t baudrate,
											uint32_t worthlength, uint32_t stopbits, uint32_t parity, uint32_t mode, 
											uint32_t hwflowctl, uint32_t oversampling );
int USART_IO_Write(USER_UART_HandleTypeDef *pHandle, unsigned char *buff, unsigned int len);
int USART_IO_ReadCount(USER_UART_HandleTypeDef *pHandle);














#endif

