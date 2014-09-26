// Includes ---------------------------------------------------------------------
#include <string.h>
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




//#define PRT_DOTLINE 1
//#define PRT_DRYRUN 2
//指令
#define CMD_Dotline 0xFFA0
#define CMD_FF      0xFFA1


//发送-接收 返回标志字定义
#define BACK_ACK            0x00    //正确应答
#define BACK_DATA_ERR       0x01    //发送数据错误
#define BACK_PRT_BUSY       0x02    //控制器忙
#define BACK_PRT_NOPAPER    0X03    //打印机缺纸
#define BACK_PRT_OVERHEAT   0x04    //打印机温度过高

//UART_HandleTypeDef huart6;
USER_UART_HandleTypeDef	huart1;
TIM_HandleTypeDef    	TimHandle;
int readflag;


int Data_Gather(unsigned char *buff);
void Data_ACK(u8 cmd);

//-------------------------------------------------------------------------------
static void Error_Handler(void)
{
  while(1);
}


//----初始化---------------------------------------------------------------------
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
  //设置time0中断时间为1ms
  TimHandle.Init.Period = 25000 - 1;
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


//-------------------------------------------------------------------------------
int cnt=0;
u8 buffer[100];




	int gather_state;
	u16 cmd;
	unsigned char buff[48];

int main(void)
{	
//	int i;
	main_init();	
	port_init();
	prt_init();
	USART_IO_Open( &huart1, 0 );

//#ifdef M_DEBUG	
	printf("#start......\r\n");
//#endif

//time0中断时间精度测试
//#ifdef TEST_CASE_TIME0		
//	timer0_start();
//	while(1);
//#endif



//---------------------------发送一行dotline测例（仅32个dot有效）----------------
#ifdef TEST_CASE_DOTLINE
	memset(buff, 0xF8, 48);
	while(1)
	{
		for(i=0;i<50;i++)
		{
			PRT_DotLine(buff,48);
			prt_stm.data_ready=1;
			prt_stm.motor_busy = 1;
			while(prt_stm.motor_busy == 1);
		}

		delay_ms(1000);
	}
#endif

//-------------------------MOTOR步进测试-----------------------------------------
#ifdef TEST_CASE_MOTOR	
	while(1)
	{
		prt_stm.data_ready=1;
		delay_ms(500);
	}

#endif


//-----------------------------快进测试------------------------------------------
#ifdef TEST_CASE_FF
	while(1)
	{
		prt_paper_ff();
		delay_ms(1000);
	}
#endif

//----------------------------校验函数测试---------------------------------------
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
			delay_ms(2);
		}
		if(c2 == 223)
		{
			DI=1;
			delay(4);
			DI=0;
			delay_ms(4);	
		}
	
	}
#endif
  
//#ifdef TEST_CASE_COMAGR
			
//#endif



//---------------------数据接收检测	---------------------------------------------
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

//-------------------------进纸检测----------------------------------------------
#ifdef TEST_CASE_PAPERFF
	while(1)
	{
		prt_paper_ff();
	}
#endif


//-----------------------------------STB  test-----------------------------------
#ifdef TEST_CASE_STB
	prt_test_STB();
	while(1);

#endif
//-------------------------------------主循环------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
	while(1)
	{			
		gather_state = Data_Gather(buff);		//数据接收处理，返回接收状态	   
		
		if(gather_state < 1)				   //
			continue;
		
		//读取指令
		cmd = *((short int*)&buff[0]);			//从接收数组中读取操作命令
			
		if(cmd == (u16)CMD_Dotline)				//打印命令则执行打印
		{
			Data_ACK(BACK_ACK);
			PRT_DotLine(&buff[2],48);			//发送打印数据 
			prt_stm.data_ready=1;
			prt_stm.motor_busy = 1;
			while(prt_stm.motor_busy == 1);
		}
		
		else 
		if(cmd == (u16)CMD_FF)// && NO_PAPER == 0)				//空走纸命令
		{											
				prt_paper_run(*((short int*)&buff[2]));			//打印机进纸	
				Data_ACK(BACK_ACK);								//返回成功状态给上层
		}			
	}
}

//-----------------数据接收处理函数--------------------------------------------------------------
int Data_Gather(unsigned char *buff)
{
	unsigned char c1,c2;
	static int rev;
	int rx_count;

	if(!prt_stm.tim_flag)
		return 0;

	prt_stm.tim_flag = 0;
	rx_count=USART_IO_ReadCount(&huart1);		 //串口接收数据数
		
	if(rx_count == 0)							 //未接收到数据
		return 0;

	if(rev != rx_count)							 //数据未接收完
	{
		rev = rx_count;
		return 0;	
	}
	rev = 0;			
	
	USART_IO_Read(&huart1, buff, rx_count);		 //将接收到的数据读出
	//校验
	if(rx_count < 4)							 //标准数据不少于4个字节
		return 0;

	checkout (buff, rx_count-2, &c1, &c2);		 //校验接收数据
	if(c1 != buff[rx_count-2] || c2 != buff[rx_count-1])	   
		return 0;											   //返回校验错误

	//数据接收无误，返回
	return 1;

}

//-------------------------------------------------------------------------------
void Data_ACK(u8 cmd)
{

	USART_IO_Write(&huart1, &cmd, 1);		
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#ifdef TEST_CASE_RX
	sprintf(debug_buff,"error %d\r\n",gather_state);
	printf(debug_buff);
#endif

