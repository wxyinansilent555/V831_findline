#include "Time_interrupt.h"


void TIM2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_InternalClockConfig(TIM2);
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 5*100-1;     
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
			
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

   
    TIM_Cmd(TIM2, ENABLE);
}


void TIM3_Init(void)
{
//    
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

//    TIM_InternalClockConfig(TIM3);
//	
//    
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//  
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseStructure.TIM_Period = 5*100-1;    
//    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//			
//	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//		
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_Init(&NVIC_InitStructure);


//    TIM_Cmd(TIM3, ENABLE);
}




