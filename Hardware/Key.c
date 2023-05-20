#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"
#include "key.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"

//按键初始化函数 
//设置成输入
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTB时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试

	//ok
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PB3 ok
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成下拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.9
	
	//start
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PA4 start
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.4
 
    //add
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PB8 add
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.8
	
	//sub
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PA12 sub
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.12
	
	//x10
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA2 x10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB.10
	
	//mode
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PB11 mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11

}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY_add
//KEY_sub
//KEY_x10
//KEY_mode
//KEY_start
//KEY_ok
uint8_t key_mode;
u8 Key_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_mode=1;  //支持连按		  
	if(key_up && (KEY_add==0||KEY_sub==0||KEY_x10==0||KEY_mode==0||KEY_start==0||KEY_ok==0))
	{
		Delay_ms(50);//去抖动 
		key_up=0;
		if(KEY_add==0)return KEY_add_res;
		else if(KEY_sub==0)return KEY_sub_res;
		else if(KEY_x10==0)return KEY_x10_res; 
		else if(KEY_mode==0)return KEY_mode_res;
		else if(KEY_start==0)return KEY_start_res; 
		else if(KEY_ok==0)return KEY_ok_res; 
	}
    else if((KEY_add==1||KEY_sub==1||KEY_x10==1||KEY_mode==1||KEY_start==1||KEY_ok==1))key_up = 1;
	
    return 0;// 无按键按下	
	
}

/*
*函数名称：STMFLASH_ReadHalfWord
*函数功能：读出flash内半字
*输入参数：faddr
*返回参数：*(vu16*)faddr
*/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

/*
*函数名称：STMFLASH_Read
*函数功能：从指定地址开始读出指定长度的数据
*输入参数：ReadAddr:起始地址 pBuffer:数据指针 NumToWrite:半字(16位)数
*返回参数：void
*/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

u8 mode_flag = 0;
u8 x10_flag = 1;
void Flash_change(int key)
{
    if(key == KEY_mode_res)
	{
		Delay_ms(50);
		mode_flag += 1;
		if (mode_flag > 3) mode_flag = 1;		
        if(mode_flag == 1)//P
		{
            OLED_Clear();
			OLED_ShowString(1,1,"P:");
		}	
		else if (mode_flag == 2)//I
		{
		    OLED_Clear();
			OLED_ShowString(1,1,"I:");
		}
		else if (mode_flag == 3)//D
		{
			OLED_Clear();
			OLED_ShowString(1,1,"D:");
		}		
	}
    
    if(key == KEY_x10_res)
	{
		x10_flag += 1;
		if(x10_flag > 2)x10_flag =1;
	}

	if(key == KEY_add_res)//加
	{
        if(mode_flag == 1 && x10_flag == 0 )//修改P
		{
            Kp += 0.1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kp += 1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
        if(mode_flag == 2 && x10_flag == 0 )//修改I
		{
            Ki += 0.1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 2 && x10_flag == 1 )
		{
            Ki += 1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}		
        if(mode_flag == 3 && x10_flag == 0 )//修改D
		{
            Kd += 0.1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*22,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kd += 1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*22,Kd);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
	}

	if(key == KEY_sub_res)//减
	{
        if(mode_flag == 1 && x10_flag == 0 )//修改P
		{
            Kp -= 0.1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kp -= 1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
        if(mode_flag == 2 && x10_flag == 0 )//修改I
		{
            Ki -= 0.1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 2 && x10_flag == 1 )
		{
            Ki -= 1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}		
        if(mode_flag == 3 && x10_flag == 0 )//修改D
		{
            Kd -= 0.1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*22,Kp);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kd -= 1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// 解锁flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //已用地址0x800 0000+已用flash  C8T6 1KB每页  RCT6 2KB每页
			FLASH_ProgramWord(0x08000000+1*1024*22,Kd);//写入一个32位数据
			FLASH_Lock();//flash上锁
		}
	}
}

