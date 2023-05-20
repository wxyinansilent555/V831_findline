#include "openmv.h"
#include "led.h"
u8 theta_temp,distance_temp;
int theta;
int distance;
int flag;
int flag_total=0;
extern int angle;
void USART2_Init(u32 bound)
{ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//USART1,GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART1,GPIOA
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO
	
    //USART2_RX	  GPIOA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;//Usart2 NVIC 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
		
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	

    USART_Init(USART2, &USART_InitStructure); 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);                   
}
//USART2 中断函数
void USART2_IRQHandler(void)			 
{
	LED1_ON();
	u8 com_data; 
	u8 i;
	static u8 RxCounter1=0;
	static u16 RxBuffer1[10]={0};
	static u8 RxState = 0;	
	static u8 RxFlag1 = 0;
	if( USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
	{
		LED1_ON();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);  
		com_data = USART_ReceiveData(USART2);
		if(RxState==0&&com_data==0xAA)  //0xAA֡
		{
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;
		}
		else if(RxState==1&&com_data==0xAE)  //0xAE֡
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}
		else if(RxState==2)
		{
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1>=9||com_data == 0x5B)       //RxBuffer1接收满了
			{
				RxState=3;
				RxFlag1=1;
				theta_temp=(RxBuffer1[RxCounter1-8]<<8)+(RxBuffer1[RxCounter1-7]);
				distance_temp=(RxBuffer1[RxCounter1-6]<<8)+(RxBuffer1[RxCounter1-5]);
				flag=(RxBuffer1[RxCounter1-4]<<8)+(RxBuffer1[RxCounter1-3]);
			}
		}
		else if(RxState==3)		//帧尾
		{
			if(RxBuffer1[RxCounter1-1] == 0x5B)
			{
				USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//失能串口中断
				if(RxFlag1)
					{
						theta = theta_temp;
						theta = theta - 100;
						distance = distance_temp;
						distance = distance - 60;
						if(theta < 0) angle = theta + 58;
                        else if(theta > 0) angle = theta - 58;		
		                OLED_ShowSignedNum(3, 2, angle, 3);
		    			OLED_ShowSignedNum(3, 9, distance, 3);	
						}
					RxFlag1 = 0;
				    RxCounter1 = 0;
					RxState = 0;
					USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
				}
			else   
			{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<10;i++)
				{
					RxBuffer1[i]=0x00;     
				}
			}
		} 
		else  
		{
			RxState = 0;
			RxCounter1=0;
			for(i=0;i<10;i++)
			{
				RxBuffer1[i]=0x00;    
			}
		}
	}
	LED1_OFF();
}

