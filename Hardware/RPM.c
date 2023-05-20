#include "RPM.h"
//编码器分辨率,即转一圈产生多少个脉冲
float Encoder_num=234.3;
const float speed_L_bis=25;
const float	speed_R_bis=25;
float rad;
float speed_L,speed_R;
float dead_zone_p=0.5;
float dead_zone_m=-0.5;
float wheel_length=20.41;
//inter_time是定时器的定时间隔,就是多久产生一次定时中断
float inter_time=0.05;

float LPID_realize(float target,float actural)//右轮编码器PID
{	
	//开始使用pid算法
	float error_L = actural - target;
	
	float output_val_L =(target + KP_L * (error_L - last_error_L) + KI_L * error_L  );

	last_error_L = error_L;
	
	return output_val_L;
}

float RPID_realize(float target,float actural)//右轮编码器PID
{	
	//开始使用pid算法
	float error_R = actural - target;
	
	float output_val_R =(target + KP_R * (error_R - last_error_R) + KI_L * error_R  );

	last_error_R= error_R;
	
	return output_val_R;
}

