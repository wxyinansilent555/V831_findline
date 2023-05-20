#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"
#include "Delay.h"
#include "OLED.h"
#include "Time_interrupt.h"
#include "Motor.h"
#include "Key.h"
#include "PWM.h"                                    
#include "RPM.h"
#include "openMv.h"
#include "Encoder.h"
#include "LED.h"
#include "Serial.h"
#include "Sensor.h"

int  target_speed = 30;    //cm/s  小车整体预期速度
float target_L,target_R;   //      小车预期速度的脉冲数 30cm/s ==> 334.9
#define  track    17.7     //cm    后轮中轴距离
#define  wheel_R  3.248    //cm    轮子的半径
#define R        110       //cm    转向半径
int angle, ENABLE_flag = 0;
//uint32_t *Kp_address = (uint32_t *)0x08000000+1*1024*30;
//uint32_t *Kd_address = (uint32_t *)0x08000000+1*1024*31;
//uint32_t *Ki_address = (uint32_t *)0x08000000+1*1024*32;
extern uint8_t Serial_RxData;
u8 key_flag=0;
uint32_t test_num =5;

void go_line(void);

int main(void)
{
	LED_Init();
	OLED_Init();	
	OLED_ShowString(1,1,"VL:");
	OLED_ShowString(1,9,"VR:");
	
	sensor_Init();
	Key_Init();
	Motor_Init(); 
	Encoder_Init();	

	USART2_Init(57600);    //V831串口打开
	USART1_Init(57600);    //334透传模块打开
	TIM2_Init();	       //初始化定时器2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); //立即失能定时器，直到V831初始化完成

//	FLASH_Unlock();// 解锁flash
//	FLASH_ErasePage(0x08000000+1*1024*30);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
//	FLASH_ProgramWord(0x08000000+1*1024*30,test_num); //写入一个32位数据
//	FLASH_Lock();//flash上锁
//	
//    //上电读取FLASH
//    Kp = *Kp_address;
//	Kd = *Kd_address;
//	Ki = *Ki_address;

	while (1)
	{	
		int key;
		key = Key_Scan(0);
        if(key == KEY_start_res && key_flag == 0)
		{
			key_flag = 1;
			ENABLE_flag = 1 ;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		}
		/*
		else if (key == KEY_start_res && key_flag == 1)
		{
			key_flag = 0;
			ENABLE_flag = 0;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
		}
		
		else if (key == KEY_add_res || key == KEY_x10_res || key == KEY_sub_res || key == KEY_mode_res)
		{
			LED1_ON();
			Flash_change(key);
			LED1_OFF();
		}
		*/
	}
}

int test_flag = 3;//修改任务
int circle = 0;       //当前圈数
/*主控函数 0.05s触发一次*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{	
		if(test_flag == 1) //任务1 正常走一圈
		{
			if(sensor_flag <= 2)
			{
                go_line();
			}	
			else
			{
				Motor_control_1();
				sensor_flag = 0;
			}		
		}
		else if(test_flag == 2) //任务2超车
		{
			if(sensor_flag < 2)
			{
                go_line();
			}
			else
			{
				Motor_control_2();
				sensor_flag = 0;
			}
		}
		else if(test_flag == 3) //任务3超车两次
		{
			if(sensor_flag > 2)
			{
				Motor_control_3();
				sensor_flag = 0;
				circle = circle + 1;
				if(circle > 2) circle = 1;
			}
			else
			{
				go_line();
			}
		}

		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		
	}
	
}

void go_line(void)
{
	flag_time++;
    Encoder_Left_Count=Encoder_Left_Get();
	Encoder_Right_Count=Encoder_Right_Get();
		
	//回传编码器脉冲到OLED上	
	OLED_ShowFNum(1,4,Encoder_Left_Count);  //左轮编码器
	OLED_ShowFNum(1,12,Encoder_Right_Count);//右轮编码器
        
	Serial_Printf("%d,",Encoder_Left_Count);//334透传串口调试用
	Serial_Printf("\n");
	
    speed_L = 80 + Position_PID(angle);  //位置PID
	speed_R = 80 - Position_PID(angle);
		
	if(speed_L < 0)speed_L = 0;               //占空比限幅
	if(speed_R  <0)speed_R = 0;

	Motor_SetSpeed(LPID_realize(speed_L,Encoder_Left_Count),RPID_realize(speed_R,Encoder_Right_Count)); //速度PID
}