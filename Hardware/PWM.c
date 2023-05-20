#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;    
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36-1;  
																							 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);  

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; 
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure); 
	TIM_OC4Init(TIM1,&TIM_OCInitStructure); 
	TIM_Cmd(TIM1,ENABLE);   
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 
	TIM_OC1PreloadConfig(TIM1,ENABLE);  
	TIM_OC4PreloadConfig(TIM1,ENABLE); 
	TIM_ARRPreloadConfig(TIM1,ENABLE);  

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

//PWMB
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}

//PWMA
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);
}
