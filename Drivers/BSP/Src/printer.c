#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include"bsp.h"
#include"printer.h"
#include"test_case.h"
#include "SystemClock.h"

#define STEP_MAX 3
PRT_STM prt_stm;


/***************************��ʼ��������״̬�����������****************************/


void PRT_STM_init (void)
{
	prt_stm.no_paper=1;		  //��ֽ//
	prt_stm.over_hot=0;		  //����//
	prt_stm.step_count=0;
	prt_stm.thermal_head_count=0;
	prt_stm.data_ready=0;
	prt_stm.time0_count=0;
}



/*******************************************************************************************/
/******************************�йط���*****************************************************/
void PRT_DotLine(unsigned char *str, int length)
{
		int i,j;
		char temp;
	
	
		for(i=0;i<length;i++)
		{
			temp = *str;
			for(j=0;j<8;j++)
			{
				if(temp & 0x80)
					DI_H();
				else
					DI_L();
				temp <<= 1;
	
				CLK_H();
				delay_ms(1);
				CLK_L();
				delay_ms(1);				
			}
			str++;
		}
	
	

}

void PRT_dry_run(int line_num)
{
	int i;
	for(i=0;i<line_num*2;i++)
	{
		switch(prt_stm.step_count)
		{
			case 0: STEP_A_L();
							STEP_B_L();
							break;
			case 1:	STEP_A_L();
							STEP_B_H();
							break;
			case 2:	STEP_A_H();
							STEP_B_H();
							break;
			default:	STEP_A_H();
							STEP_B_L();
							prt_stm.step_count=0;			
		}
		prt_stm.step_count++;
		delay_ms(1);
	}
}




/*******************************************************************************************************/


/**********************�жϺ����������������ر�*************************************************/
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);


void prt_init(void)		//�жϿ�������//
{

	STEP_A_L();//�������A��//
	STEP_B_L();//�������B��//
	STEP_E_H();							  //�������ʹ��//

	prt_stm.step_count++;

}


void prt_print(void)
{
#ifdef TEST_CASE_TIME0

	static int flag=0;
	if(flag == 0)
	{
		DI_L();
		flag=1;
	}
	else
	{
		DI_H();
		flag=0;
	}
#else


	prt_stm.time0_count++;
	if(prt_stm.time0_count == 1)
	{
		//�ж��Ƿ���dotline����
		if(prt_stm.data_ready == 0 || NO_PAPER == 1)
		{
			prt_stm.time0_count=0;
			STB2_L();	   
	   		STB1_L();
			return;	
		}
		prt_stm.data_ready = 0;
		//����
		STB2_L();	   
	   	STB1_H();
		//����
		switch(prt_stm.step_count)
		{
			case 0: STEP_A_L();
							STEP_B_L();
							break;
			case 1:	STEP_A_L();
							STEP_B_H();
							break;
			case 2:	STEP_A_H();
							STEP_B_H();
							break;
			default:	STEP_A_H();
							STEP_B_L();
							prt_stm.step_count=0;			
		}
		prt_stm.step_count++;
	}
	else
	if(prt_stm.time0_count == 2) 
	{
		STB1_L();
		STB2_H();
	}
	else
	if(prt_stm.time0_count == 3) 
	{
		STB2_L();
		STB1_H();
		switch(prt_stm.step_count)
		{
			case 0: STEP_A_L();
							STEP_B_L();
							break;
			case 1:	STEP_A_L();
							STEP_B_H();
							break;
			case 2:	STEP_A_H();
							STEP_B_H();
							break;
			default:	STEP_A_H();
							STEP_B_L();
							prt_stm.step_count=0;			
		}
		prt_stm.step_count++;

	}
	else
	if(prt_stm.time0_count == 4) 
	{
		STB1_L();
		STB2_H();
		prt_stm.time0_count=0;
	}
#endif

}
