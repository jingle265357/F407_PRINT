#ifndef __printer_h__
#define __printer_h__

#define BACK_ACK            0x00    //��ȷӦ��
#define BACK_DATA_ERR       0x01    //�������ݴ���
#define BACK_PRT_BUSY       0x02    //������æ
#define BACK_PRT_NOPAPER    0X03    //��ӡ��ȱֽ
#define BACK_PRT_OVERHEAT   0x04    //��ӡ���¶ȹ���


typedef struct 
{
	char no_paper;
	char over_hot;
	char step_count;
	char thermal_head_count;            	 //�������ݱ���
	char data_ready;
	char time0_count;
	char motor_busy;//���������
	char FF_line_max;
	char FF_line_count;
	char FF_line_dry;
	char FF_flag;
	char tim_flag;
}PRT_STM;
extern PRT_STM prt_stm;


void PRT_DotLine(unsigned char *str, int length);
void PRT_STM_init (void);
void prt_init(void);		//�жϿ�������//
void PRT_dry_run(void);
void prt_print(void);
void prt_paper_ff(int num);
void prt_paper_run(int line_num);
void prt_test_STB(void);
#endif
