#ifndef __OPENMV_H_
#define __OPENMV_H_

#include "stm32f10x.h"
#include "OLED.h"

extern int theta;
extern int distance;
extern int flag_total;
extern int flag;
void USART2_Init(u32 bound);           //串口2初始化并启动

#endif
