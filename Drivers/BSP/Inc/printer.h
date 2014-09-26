#ifndef __printer_h__
#define __printer_h__

#define BACK_ACK            0x00    //正确应答
#define BACK_DATA_ERR       0x01    //发送数据错误
#define BACK_PRT_BUSY       0x02    //控制器忙
#define BACK_PRT_NOPAPER    0X03    //打印机缺纸
#define BACK_PRT_OVERHEAT   0x04    //打印机温度过高


typedef struct 
{
	char no_paper;
	char over_hot;
	char step_count;
	char thermal_head_count;            	 //发送数据标致
	char data_ready;
	char time0_count;
	char motor_busy;//电机工作中
	char FF_line_max;
	char FF_line_count;
	char FF_line_dry;
	char FF_flag;
	char tim_flag;
}PRT_STM;
extern PRT_STM prt_stm;


void PRT_DotLine(unsigned char *str, int length);
void PRT_STM_init (void);
void prt_init(void);		//中断开启函数//
void PRT_dry_run(void);
void prt_print(void);
void prt_paper_ff(int num);
void prt_paper_run(int line_num);
void prt_test_STB(void);
#endif
