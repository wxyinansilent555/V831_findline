#ifndef __SENSOR_H
#define __SENSOR_H
//#include "usart.h"
#include "stm32f10x.h"
extern u8 sensor_flag;
extern int flag_time;
void sensor_Init(void);//��ʼ��
void sensor_move(void);
//��ȡ���ݣ�����һ��8λ�޷�����
//�����С��ǰ������,���λ��������ߣ����Ϊ�������ұ�
//����Ϊ0 ����Ϊ1
//u8 Read_data(void);


#endif
