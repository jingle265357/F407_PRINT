/* Includes ------------------------------------------------------------------*/
#include "main.h"


#include "hal_debug.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "SystemClock.h"
#include "HAL_Bus8Bit.h"
//#include "fingerprint.h"
#include "usart_io.h"
#include "compute.h"
#include "bsp.h"
#include "printer.h"
#include "test_case.h"
#include "SystemClock.h"


#ifdef TEST_CASE_COMMAND
	const unsigned char case_buff[]={86,57,32,42,1,5,96,221};
	unsigned char c1,c2;
#endif

#ifdef TEST_CASE_COMAGR
	const unsigned char case_buff[]={6,0,86,57,32,42,1,5,97,223};
	unsigned char xdata rx_data_buff[20];
#endif

#if defined TEST_CASE_GATHER
	unsigned char gather_test_buff[]={0x55,0x55,0x0c,0x00,0x01,0x00,0x56,0x39,0x20,0x2a,0x01,0x05,0x6c,0xec};						               
#endif




#define PRT_DOTLINE 1
#define PRT_DRYRUN 2

//UART_HandleTypeDef huart6;
USER_UART_HandleTypeDef	huart1;
TIM_HandleTypeDef    	TimHandle;
USER_UART_HandleTypeDef huart1;
int readflag;


int Data_Gather(unsigned char *buff);
void Data_ACK(short int cmd);

//------------------------------------------------------------------------------
static void Error_Handler(void)
{
  while(1);
}


//----初始化--------------------------------------------------------------------
void main_init(void)
{
	HAL_Init();
  	SystemClock_Config();

//	HAL_Bus8BitInit( 1 );		//8Bit总线接口初始化
//	FINT_DeviceInit();			//指纹设备初始化

	UartInit();			 //串口初始化printf专用
	PRT_STM_init();		//状态机的初始化//
	HAL_UART_MspInit(&huart1.UartHandle);

	USART_IO_Config( 	&huart1, 
						USART1, 
						115200, 
						UART_WORDLENGTH_8B, 
						UART_STOPBITS_1, 
						UART_PARITY_NONE, 
						UART_MODE_TX_RX, 
						UART_HWCONTROL_NONE, 
						UART_OVERSAMPLING_16 );


  /* Set TIMx instance */
  TimHandle.Instance = PRT_TIM;
   
  /* Initialize TIM3 peripheral as follow:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock/2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = 10000 - 1;
  TimHandle.Init.Prescaler = 7;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}


/**********************************************************/
int cnt=0;
u8 buffer[100];

int main(void)
{	
	int gather_state;
	short int cmd;
	unsigned char buff[48];
	int line_num=4;


	main_init();	
	port_init();
	prt_init();
	USART_IO_Open( &huart1, 0 );

#ifdef M_DEBUG	
	printf("#start......\r\n");
#endif

//time0中断时间精度测试
#ifdef TEST_CASE_TIME0		
	timer0_start();
	while(1);
#endif



//发送一行dotline测例（仅32个dot）
#ifdef TEST_CASE_DOTLINE
	buff[0]=0x55;
	buff[1]=0x00;
	buff[2]=0xff;
	buff[3]=0x55;
	while(1)
	{
		PRT_DotLine(buff,48);
		delay_ms(40);
	}

#endif

//MOTOR测试
#ifdef TEST_CASE_MOTOR	
	while(1)
	{
		prt_stm.data_ready=1;
		delay_ms(500);
	}

#endif


//快进测试
#ifdef TEST_CASE_FF
	while(1)
	{
		PRT_dry_run(10);
		delay_ms(500);
	}
#endif

//校验函数测试
#ifdef TEST_CASE_COMMAND
	checkout ((unsigned char*)case_buff,6,&c1,&c2);
	while(1)
	{
		DI=0;
		delay_ms(10);
	
		if(c1 == 97)
		{
			DI=1;
			delay(2);
			DI=0;
			delay(2);
		}
		if(c2 == 223)
		{
			DI=1;
			delay(4);
			DI=0;
			delay(4);	
		}
	
	}
#endif
  
//#ifdef TEST_CASE_COMAGR
			
//#endif



//数据接收检测
#ifdef TEST_CASE_GATHER
		USART_IO_Write(&huart1, gather_test_buff, 14);
		delay_ms(1000);
		gather_state=Data_Gather(buff);
		if(gather_state<0)
		{
			switch(gather_state)
			{
				case -1:	printf("first head error (-1) \r\n");
							break;
				case -2:	printf("second head error (-2) \r\n");
							break;
				case -5:	printf("checkout error (-5) \r\n");
							break;
			}
			while(1);
		}
		else
		{
			cmd = buff[5];
			if(cmd == PRT_DOTLINE)
				printf("PRT_DOTLINE command \r\n");
			else
			if(cmd == PRT_DRYRUN)
				printf("PRT_DRYRUN command \r\n");
			else
				printf("other \r\n");	
			while(1);
		}
#endif



	while(1)
	{			
		gather_state=Data_Gather(buff);	   //????????
		if(gather_state < 0 )
		{

#ifdef TEST_CASE_RX
	sprintf(debug_buff,"error %d\r\n",gather_state);
	printf(debug_buff);
#endif
			continue;
		}
		else
			if(gather_state == 0)
				continue;
		//????
		cmd = *((short int*)&buff[2]);
			
		if(cmd == PRT_DOTLINE)
		{
			Data_ACK(1);
			PRT_DotLine(&buff[4],48);			  //????

		}
		
		else 
		if(cmd == PRT_DRYRUN && NO_PAPER == 0)		
		{											
			Data_ACK(2);
			PRT_dry_run(line_num);					//??N?
		}
		else
		{
			Data_ACK(0);	
		}
			
	}
}



//------------------------------------------------------------------------------
int Data_Gather(unsigned char *buff)
{
	static int step_cnt=0;
	unsigned char head;
	static short int length;
	unsigned char c1,c2;
	int rx_count;
	
loop0:	
	rx_count=USART_IO_ReadCount(&huart1);	
	if(rx_count == 0)
		return 0;		//没收到数据

	if(step_cnt == 0)
	{
		USART_IO_Read(&huart1,&head,1);
		if(head != 0x55)
			return -1;				//不是第一个有效head
		step_cnt=1;
		goto loop0;
	}
	if(step_cnt == 1)
	{
		USART_IO_Read(&huart1,&head,1);
		if(head != 0x55)
			return -2;				//不是第二个有效的head
		step_cnt=2;	
		goto loop0;
	}
	if(step_cnt == 2)
	{
		if(rx_count < 2)		
			return -3; 					//没收到length
		USART_IO_Read(&huart1,buff,2);
		length=*((short int*)&buff[0]);
		step_cnt=3;
		goto loop0;
	}
	if(step_cnt == 3)
	{
		if(length < rx_count-2)
			return -4;//数据没收完
		USART_IO_Read(&huart1,&buff[2],length-2);
		checkout (buff, length-2, &c1, &c2);
		if(c1 != buff[length-2] || c2 != buff[length-1])
			return -5;		  //校验错误						}
	}
		
	step_cnt =0;
	return 1;

}

//********************************************************************//
void Data_ACK(short int cmd)
{
	int i;
	unsigned char buff[8];
	unsigned char c1=buff[2],c2=buff[3];

	buff[0]=buff[1]=0x55;
	buff[2]=6;
	buff[3]=0;
	buff[4]=cmd;
	buff[5]=0;
	for(i=2;i<6;i++)
	{
		c1 ^= buff[i];
		c2 += buff[i];
	}
	buff[6]=c1;
	buff[7]=c2;

	USART_IO_Write(&huart1, buff, 8);
		
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

