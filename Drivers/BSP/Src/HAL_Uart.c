#include <string.h>
#include "HAL_Uart.h"
/////////////////////////////////////////////////////////////////////////////////////////////////

//UART配置
USART_TypeDef* const UART_TAB[]={USART1, USART2, USART3};
GPIO_TypeDef* const UART_GPIO_TAB[]={GPIOA, GPIOD, GPIOD};
const u16 UART_TxPin_TAB[]={GPIO_PIN_9, GPIO_PIN_5, GPIO_PIN_8};
const u16 UART_RxPin_TAB[]={GPIO_PIN_10, GPIO_PIN_6, GPIO_PIN_9};

#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 
#define USART2_RX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE() 
#define USART3_RX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE()
#define USART3_TX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE() 

const u8 UART_GPIO_AF_TAB[]={GPIO_AF7_USART1, GPIO_AF7_USART2, GPIO_AF7_USART3};
DMA_Stream_TypeDef* const DMA_RX_Stream_TAB[]={DMA1_Stream5, DMA1_Stream6, DMA1_Stream3 };

//const u8 UART_TxSource_TAB[]={GPIO_PinSource9, GPIO_PinSource5, GPIO_PinSource8};
//const u8 UART_RxSource_TAB[]={GPIO_PinSource10, GPIO_PinSource6, GPIO_PinSource9};
//const u8 UART_IRQn_TAB[]={USART1_IRQn, USART2_IRQn, USART3_IRQn};
//const u32 UART_ReMap_TAB[]={0, 0, GPIO_PartialRemap_USART3};

#define USARTx_TX_AF                     GPIO_AF7_USART2



//UART中断接收
#define UART_RX_MAX 272				//接收缓冲区字节空间
typedef struct _uart_type
{
	u8 buff[UART_RX_MAX];	//接收数据缓冲区
	u8 *ptr;				//接收数据缓冲区指针
	u16 cnt;				//接收计数器
}UART_RECV_TYPE;

static UART_RECV_TYPE 	UartRecv[3];



////////////////////////////////////////////////////////////////////////////////
//----初始化UART----------------------------------------------------------------
bool ComOpen(u8 com, u32 baud, u8 width, u8 stop, u8 par)
{
	UART_HandleTypeDef UartHandle;
  	GPIO_InitTypeDef  GPIO_InitStruct;
	static DMA_HandleTypeDef hdma_rx;

	//UART配置
	if(com==COM1)
	{
		UartHandle.Instance = USART1;
		USART1_TX_GPIO_CLK_ENABLE();
  		USART1_RX_GPIO_CLK_ENABLE();
	}
	else
	if(com==COM2)
	{
		UartHandle.Instance = USART2;
		USART2_TX_GPIO_CLK_ENABLE();
  		USART2_RX_GPIO_CLK_ENABLE();
	}
	else
	if(com==COM3)
	{
		UartHandle.Instance = USART3;
		USART3_TX_GPIO_CLK_ENABLE();
  		USART3_RX_GPIO_CLK_ENABLE();
	}

	UartHandle.Init.BaudRate   = baud;

	if(width==8)
		UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	else
		UartHandle.Init.WordLength = UART_WORDLENGTH_9B;

	if(stop==1)
	  	UartHandle.Init.StopBits = UART_STOPBITS_1;
	else
	  	UartHandle.Init.StopBits = UART_STOPBITS_2;

	if(par==0)
	  	UartHandle.Init.Parity = UART_PARITY_NONE;
	else
	if(par==1)
	  	UartHandle.Init.Parity = UART_PARITY_EVEN;
	else
	if(par==2)
	  	UartHandle.Init.Parity = UART_PARITY_ODD;

  	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  	UartHandle.Init.Mode       = UART_MODE_TX_RX;
  
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
		return FALSE;

	//GPIO配置
	GPIO_InitStruct.Pin       = UART_TxPin_TAB[com];
  	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  	GPIO_InitStruct.Pull      = GPIO_NOPULL;
  	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  	GPIO_InitStruct.Alternate = UART_GPIO_AF_TAB[com];
  	HAL_GPIO_Init(UART_GPIO_TAB[com], &GPIO_InitStruct);
    
  	GPIO_InitStruct.Pin 	  = UART_RxPin_TAB[com];
  	GPIO_InitStruct.Alternate = UART_GPIO_AF_TAB[com];
    HAL_GPIO_Init(UART_GPIO_TAB[com], &GPIO_InitStruct);

/*
	//DMA配置
  	hdma_rx.Instance   				= DMA_RX_Stream_TAB[com];
  
  hdma_rx.Init.Channel             = USARTx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4; 

  HAL_DMA_Init(&hdma_rx);
    
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);

*/
	return TRUE;


/*
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

	u16 WordLenth = USART_WordLength_8b;
	u16 StopBit = USART_StopBits_1;
	u16 Parity = USART_Parity_No;

	//配置UART设备时钟，配置UART接口时钟
	if(com==COM1)
	{
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_USART1, ENABLE);
	  	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	}
	else
	if(com==COM2)
	{
  		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_USART2, ENABLE);
	  	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	}
	else
	if(com==COM3)
	{
  		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_USART3, ENABLE);
	  	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	}

	//配置复用开关
  	GPIO_PinAFConfig( UART_GPIO_TAB[com], UART_TxSource_TAB[com], UART_GPIO_AF_TAB[com]);
  	GPIO_PinAFConfig( UART_GPIO_TAB[com], UART_RxSource_TAB[com], UART_GPIO_AF_TAB[com]);

	//配置GPIO TX RX
	GPIO_InitStructure.GPIO_Pin = UART_TxPin_TAB[com];
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)UART_GPIO_TAB[com], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART_RxPin_TAB[com];
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init((GPIO_TypeDef *)UART_GPIO_TAB[com], &GPIO_InitStructure);

	//配置USART参数
	if(width==9)
		WordLenth=USART_WordLength_9b;
	if(stop==2)
		StopBit=USART_StopBits_2;
	if(par==1)
		Parity=USART_Parity_Even;
	else
	if(par==2)
		Parity=USART_Parity_Odd;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = WordLenth;
	USART_InitStructure.USART_StopBits = StopBit;
	USART_InitStructure.USART_Parity = Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init((USART_TypeDef *)UART_TAB[com], &USART_InitStructure);
  // Enable the Port_USART Transmit interrupt: this interrupt is generated when the 
  //   Port_USART transmit data register is empty 
  	USART_ITConfig(UART_TAB[com], USART_IT_RXNE,ENABLE);

	USART_Cmd((USART_TypeDef *)UART_TAB[com], ENABLE);


	//配置RX中断
  	NVIC_InitStructure.NVIC_IRQChannel = UART_IRQn_TAB[com];
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = com;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	//CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去, 如下语句解决第1个字节无法正确发送出去的问题
	USART_ClearFlag((USART_TypeDef *)UART_TAB[com], USART_FLAG_TC);     //清发送外城标志，Transmission Complete flag
	while(USART_GetFlagStatus((USART_TypeDef*)UART_TAB[com], USART_FLAG_TC)==RESET);	//Loop until the end of transmission

	//清除接收区
	UartRecv[com].ptr = UartRecv[com].buff;		
	UartRecv[com].cnt = 0;
*/

/*
	//配置RST2
  	RCC_AHB1PeriphClockCmd( RTS2_Periph, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = RTS2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(RTS2_GPIO, &GPIO_InitStructure);

	GPIO_SetBits(RTS2_GPIO , RTS2_PIN);
*/
}

////////////////////////////////////////////////////////////////////////////////
/*
//----UART1中断-----------------------------------------------------------------
void USART1_IRQHandler(void)
{
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{ 
		if( UartRecv[COM1].cnt >= UART_RX_MAX )		//检测溢出
			return;
		*UartRecv[COM1].ptr = USART1->DR; //(USARTx->DR & (uint16_t)0x01FF);
		UartRecv[COM1].ptr++;
		UartRecv[COM1].cnt++;
  	}
}
//----UART2中断-----------------------------------------------------------------
void USART2_IRQHandler(void)
{
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{ 
		if( UartRecv[COM2].cnt >= UART_RX_MAX )		//检测溢出
			return;
		*UartRecv[COM2].ptr = USART2->DR; //(USARTx->DR & (uint16_t)0x01FF);
		UartRecv[COM2].ptr++;
		UartRecv[COM2].cnt++;
  	}
}
//----UART3中断-----------------------------------------------------------------
void USART3_IRQHandler(void)
{
  	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  	{ 
		if( UartRecv[COM3].cnt >= UART_RX_MAX )		//检测溢出
			return;
		*UartRecv[COM3].ptr = USART3->DR; //(USARTx->DR & (uint16_t)0x01FF);
		UartRecv[COM3].ptr++;
		UartRecv[COM3].cnt++;
  	}
}
*/

//----获取接收字节数------------------------------------------------------------
u16 ComGetRecvCnt(u8 com)
{
	return UartRecv[com].cnt;
}
//----读取数据------------------------------------------------------------------
bool ComRecvDatas(u8 com, u8 *buff, u16 lenth)
{
	if(ComGetRecvCnt(com)==0) 				//没有接收的数据
		return FALSE;
	memcpy(buff, UartRecv[com].buff, lenth);
	//自动清除
	UartRecv[com].ptr = UartRecv[com].buff;		
	UartRecv[com].cnt = 0;
	return TRUE;
}
//----清除接收缓冲区------------------------------------------------------------
void ComClearRecvBuff(u8 com)
{
	UartRecv[com].ptr = UartRecv[com].buff;		
	UartRecv[com].cnt = 0;
}

////////////////////////////////////////////////////////////////////////////////
//----发送字符串----------------------------------------------------------------
void ComSendDatas(u8 com, u8* buff, u16 lenth)
{
	while(lenth)
	{
		lenth--;
//		USART_SendData((USART_TypeDef*)UART_TAB[com], *buff++);
//		while(USART_GetFlagStatus((USART_TypeDef*)UART_TAB[com], USART_FLAG_TC)==RESET);	//Loop until the end of transmission
	}
}

