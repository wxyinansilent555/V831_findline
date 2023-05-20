#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(float Speed_L,float Speed_R);
void Motor_control_1(void);
void Motor_control_2(void);
void Motor_control_3(void);
float Position_PID(float error);

static float Kp = 2.45;  //3.2
static float Ki = 0;
static float Kd = 0.05; //0.05

#endif
