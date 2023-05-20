#ifndef __SENSOR_H
#define __SENSOR_H
//#include "usart.h"
#include "stm32f10x.h"
extern u8 sensor_flag;
extern int flag_time;
void sensor_Init(void);//初始化
void sensor_move(void);
//读取数据，返回一个8位无符号数
//面对着小车前进方向,最高位代表最左边，最低为代表最右边
//灯亮为0 灯灭为1
//u8 Read_data(void);


#endif
