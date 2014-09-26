#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include"bsp.h"
#include"printer.h"
#include"test_case.h"
#include "SystemClock.h"

#define STEP_MAX 3
#define NO_FF 50		     //空走指令时走纸步数
PRT_STM prt_stm;


/***************************初始化函数，状态机、步进电机****************************/


void PRT_STM_init (void)
{
	prt_stm.no_paper=1;		  //无纸//
	prt_stm.over_hot=0;		  //过热//
	prt_stm.step_count=0;
	prt_stm.thermal_head_count=0;
	prt_stm.data_ready=0;
	prt_stm.time0_count=0;
	prt_stm.motor_busy=0;  //打印头空闲
	prt_stm.FF_line_count=0;
	prt_stm.FF_flag=0;
	prt_stm.tim_flag=0;

}



/*******************************************************************************************/
/******************************有关发送*****************************************************/
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
				delay_mini(1);
				CLK_L();
				delay_mini(1);				
			}
			str++;
		}

}





/*******************************************************************************************************/


/**********************中断函数，开启、处理、关闭*************************************************/
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);


void prt_init(void)		//中断开启函数//
{

	STEP_A_H();//步进电机A相//
	STEP_B_L();//步进电机B相//
	STEP_E_H();							  //步进电机使能//
	delay_ms(10);
	STEP_E_L();							  //步进电机使能//

	prt_stm.step_count++;

}

//----168MHz延迟60nS------------------------------------------------------------
static void delay_mini_break(__IO u16 nCount)
{
	for(; nCount; nCount--);
}

//步进信号
static void prt_step(void)
{
		//步进
		switch(prt_stm.step_count)
		{
			case 0: STEP_A_H();
					STEP_B_L();
					break;
			case 1:	STEP_A_H();
					STEP_B_H();
					break;
			case 2:	STEP_A_L();
					STEP_B_H();
					break;
			default:STEP_A_L();
					STEP_B_L();
		}
		prt_stm.step_count++;
		if(prt_stm.step_count >= 4)
			prt_stm.step_count=0;

}
//每1ms调用一次，
//打印数据就绪状态下，执行一行打印操作；数据未就绪则跳出
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
	return;
#else


	//序列0，判断是否有打印数据
	if(prt_stm.time0_count == 0)
	{
		//如果没有打印数据则退出
		if(prt_stm.data_ready == 0 || NO_PAPER == 1)
		{
			STB2_L();	   
	   		STB1_L();
			STEP_E_L();

			return;	
		}
		else
		{
			prt_stm.data_ready = 0;
			LAT_L();
			delay_mini_break(20);
			LAT_H();
			delay_mini_break(40);
		}
	 }

	//序列执行开始
	prt_stm.time0_count++;
   	switch(prt_stm.time0_count)
	{
		case 1:	prt_step();	//步进
				STEP_E_H();
				STB2_L();	//加热
			   	STB1_H();
				break;
		case 2:	STB2_H();	//加热
			   	STB1_L();
				break;
		case 3:	STB2_L();	   
	   			STB1_L();
				STEP_E_L();
				break;


		case 9:	prt_step();	//步进
				STEP_E_H();
				STB2_L();	//加热
			   	STB1_H();
				break;
		case 10:	STB2_H();	//加热
			   	STB1_L();
				break;
		case 11:	STB2_L();	   
	   			STB1_L();
				STEP_E_L();
				break;

		case 16:
				prt_stm.time0_count = 0;
				prt_stm.motor_busy = 0;

	}

#endif

}



void prt_test_STB(void)
{
	STB2_L();	//加热
	STB1_H();
	delay_ms(500);
	STB2_H();	//加热
	STB1_L();
	delay_ms(500);
	STB2_L();	//加热
	STB1_L();

}



void PRT_dry_run(void)
{
	static char flag_dry=1;
	flag_dry ^= 1;			//控制走纸速度
	if(flag_dry)
		return;
	if(prt_stm.FF_line_count > prt_stm.FF_line_max)
	{
		STEP_E_L();
		prt_stm.FF_line_count = 0;
		prt_stm.motor_busy = 0; 
	}
	else
	{

		prt_step();	
		STEP_E_H();
		prt_stm.FF_line_count++;
	}

}

//-----------------------------打印机装纸----------------------------------------
void prt_paper_ff(int num)
{
	
	if(PAPER_FF_BUTTON() == 0)
	{
		delay_ms(10);
		while(PAPER_FF_BUTTON() == 0)
		{
			prt_stm.FF_flag=1;		//通知中断函数走纸
			prt_stm.FF_line_max = num;
			prt_stm.motor_busy=1;			
			while(prt_stm.motor_busy);	
			prt_stm.FF_flag=0;	
		}
	}
}


//---------------------空走纸----------------------------------------------------
void prt_paper_run(int line_num)
{
	prt_stm.FF_flag=1;               //通知中断函数走纸
	prt_stm.FF_line_max=line_num;
	prt_stm.motor_busy=1;
	while(prt_stm.motor_busy);
	prt_stm.FF_flag=0;
}
