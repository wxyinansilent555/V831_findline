#include "RPM.h"
//�������ֱ���,��תһȦ�������ٸ�����
float Encoder_num=234.3;
const float speed_L_bis=25;
const float	speed_R_bis=25;
float rad;
float speed_L,speed_R;
float dead_zone_p=0.5;
float dead_zone_m=-0.5;
float wheel_length=20.41;
//inter_time�Ƕ�ʱ���Ķ�ʱ���,���Ƕ�ò���һ�ζ�ʱ�ж�
float inter_time=0.05;

float LPID_realize(float target,float actural)//���ֱ�����PID
{	
	//��ʼʹ��pid�㷨
	float error_L = actural - target;
	
	float output_val_L =(target + KP_L * (error_L - last_error_L) + KI_L * error_L  );

	last_error_L = error_L;
	
	return output_val_L;
}

float RPID_realize(float target,float actural)//���ֱ�����PID
{	
	//��ʼʹ��pid�㷨
	float error_R = actural - target;
	
	float output_val_R =(target + KP_R * (error_R - last_error_R) + KI_L * error_R  );

	last_error_R= error_R;
	
	return output_val_R;
}

