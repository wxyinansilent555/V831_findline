#include "Sensor.h"
#include "RPM.h"
#include "Delay.h"
u8 sensor_flag=0;
u8 sensor_flag_last=0;
int flag_time=0;

#define sensor_ GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
void sensor_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);// ʹ��AFIOʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);//����AFIO��ѡ����Ҫ�õ��ж�����
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;// �����ж���Ϊ3��1
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;// ʹ���ж���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;// ����Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // ����Ϊ�½��ش����ж�
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;// �����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;// ������ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;// ���������ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);

	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 1)
		{
			Delay_us(2000);
			
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 1)
			{
				if(flag_time>=150)
				{
					sensor_flag ++;
					if(sensor_flag-sensor_flag_last>1)
						sensor_flag=sensor_flag_last+1;
					OLED_ShowNum(2, 2, sensor_flag, 2);
					sensor_flag_last=sensor_flag;					
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	//}
}
