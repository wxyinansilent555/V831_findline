#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
//存储计数值
int16_t Count_L=0, Count_R=0;
int16_t Encoder_Left_Count=0,Encoder_Right_Count=0;
void Encoder_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);// 使能AFIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//配置AFIO，选择想要用的中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line6;// 配置中断线为3和1
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;// 使能中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;// 配置为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 配置为下降沿触发中断
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 配置中断优先级分组

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;// 配置中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;// 配置抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 配置子优先级为0
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;// 配置中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;// 配置抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 配置子优先级为1
	NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Left_Get(void)
{
	int16_t Temp;
	Temp = Count_L;
	Count_L = 0;
	return Temp;
}

int16_t Encoder_Right_Get(void)
{
	int16_t Temp;
	Temp = Count_R;
	Count_R = 0;
	return Temp;
}

//中断函数的名字是固定的，本函数对应PIN1脚
void EXTI9_5_IRQHandler(void)
{
		//if (EXTI_GetITStatus(EXTI_Line1) == SET)
	//{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
			{
				Count_L ++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	//}
}

//中断函数的名字是固定的，本函数对应PIN3脚
void EXTI1_IRQHandler(void)
{
		//if (EXTI_GetITStatus(EXTI_Line3) == SET)
	//{
			/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
			{
				Count_R ++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
		
	//}
}

