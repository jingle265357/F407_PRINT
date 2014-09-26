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
//ָ��
#define CMD_Dotline 0xFFA0
#define CMD_FF      0xFFA1


//����-���� ���ر�־�ֶ���
#define BACK_ACK            0x00    //��ȷӦ��
#define BACK_DATA_ERR       0x01    //�������ݴ���
#define BACK_PRT_BUSY       0x02    //������æ
#define BACK_PRT_NOPAPER    0X03    //��ӡ��ȱֽ
#define BACK_PRT_OVERHEAT   0x04    //��ӡ���¶ȹ���

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


//----��ʼ��---------------------------------------------------------------------
void main_init(void)
{
	HAL_Init();
  	SystemClock_Config();

//	HAL_Bus8BitInit( 1 );		//8Bit���߽ӿڳ�ʼ��
//	FINT_DeviceInit();			//ָ���豸��ʼ��

	UartInit();			 //���ڳ�ʼ��printfר��
	PRT_STM_init();		//״̬���ĳ�ʼ��//
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
  //����time0�ж�ʱ��Ϊ1ms
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

//time0�ж�ʱ�侫�Ȳ���
//#ifdef TEST_CASE_TIME0		
//	timer0_start();
//	while(1);
//#endif



//---------------------------����һ��dotline��������32��dot��Ч��----------------
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

//-------------------------MOTOR��������-----------------------------------------
#ifdef TEST_CASE_MOTOR	
	while(1)
	{
		prt_stm.data_ready=1;
		delay_ms(500);
	}

#endif


//-----------------------------�������------------------------------------------
#ifdef TEST_CASE_FF
	while(1)
	{
		prt_paper_ff();
		delay_ms(1000);
	}
#endif

//----------------------------У�麯������---------------------------------------
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



//---------------------���ݽ��ռ��	---------------------------------------------
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

//-------------------------��ֽ���----------------------------------------------
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
//-------------------------------------��ѭ��------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
	while(1)
	{			
		gather_state = Data_Gather(buff);		//���ݽ��մ������ؽ���״̬	   
		
		if(gather_state < 1)				   //
			continue;
		
		//��ȡָ��
		cmd = *((short int*)&buff[0]);			//�ӽ��������ж�ȡ��������
			
		if(cmd == (u16)CMD_Dotline)				//��ӡ������ִ�д�ӡ
		{
			Data_ACK(BACK_ACK);
			PRT_DotLine(&buff[2],48);			//���ʹ�ӡ���� 
			prt_stm.data_ready=1;
			prt_stm.motor_busy = 1;
			while(prt_stm.motor_busy == 1);
		}
		
		else 
		if(cmd == (u16)CMD_FF)// && NO_PAPER == 0)				//����ֽ����
		{											
				prt_paper_run(*((short int*)&buff[2]));			//��ӡ����ֽ	
				Data_ACK(BACK_ACK);								//���سɹ�״̬���ϲ�
		}			
	}
}

//-----------------���ݽ��մ�����--------------------------------------------------------------
int Data_Gather(unsigned char *buff)
{
	unsigned char c1,c2;
	static int rev;
	int rx_count;

	if(!prt_stm.tim_flag)
		return 0;

	prt_stm.tim_flag = 0;
	rx_count=USART_IO_ReadCount(&huart1);		 //���ڽ���������
		
	if(rx_count == 0)							 //δ���յ�����
		return 0;

	if(rev != rx_count)							 //����δ������
	{
		rev = rx_count;
		return 0;	
	}
	rev = 0;			
	
	USART_IO_Read(&huart1, buff, rx_count);		 //�����յ������ݶ���
	//У��
	if(rx_count < 4)							 //��׼���ݲ�����4���ֽ�
		return 0;

	checkout (buff, rx_count-2, &c1, &c2);		 //У���������
	if(c1 != buff[rx_count-2] || c2 != buff[rx_count-1])	   
		return 0;											   //����У�����

	//���ݽ������󣬷���
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

