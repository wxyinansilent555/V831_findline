#ifndef __RPM_H
#define __RPM_H

#include <stm32f10x.h>
#include "Time_interrupt.h"
#include <Motor.h>
#include <Encoder.h>

static float last_error_L;
static float last_error_R;
static float next_error_L;
static float next_error_R;
	
//需要调整的参数
static float KP_L=0.1,KP_R=0.1;
static float KI_L=0,KI_R=0;
static float KD_L=0.05,KD_R=0.05;

extern  float wheel_length;
extern float inter_time;
extern const float speed_L_bis,speed_R_bis;
extern float rad;
extern float speed_L,speed_R;
extern float Encoder_num;
float RPID_realize(float target,float actural);
float LPID_realize(float target,float actural);
//void set_rad(float rad);
//void stay_rad();
#endif
