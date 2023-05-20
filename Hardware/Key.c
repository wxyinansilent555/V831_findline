#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"
#include "key.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"

//������ʼ������ 
//���ó�����
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTBʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����

	//ok
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PB3 ok
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.9
	
	//start
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PA4 start
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.4
 
    //add
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PB8 add
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.8
	
	//sub
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PA12 sub
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.12
	
	//x10
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA2 x10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB.10
	
	//mode
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PB11 mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11

}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY_add
//KEY_sub
//KEY_x10
//KEY_mode
//KEY_start
//KEY_ok
uint8_t key_mode;
u8 Key_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_mode=1;  //֧������		  
	if(key_up && (KEY_add==0||KEY_sub==0||KEY_x10==0||KEY_mode==0||KEY_start==0||KEY_ok==0))
	{
		Delay_ms(50);//ȥ���� 
		key_up=0;
		if(KEY_add==0)return KEY_add_res;
		else if(KEY_sub==0)return KEY_sub_res;
		else if(KEY_x10==0)return KEY_x10_res; 
		else if(KEY_mode==0)return KEY_mode_res;
		else if(KEY_start==0)return KEY_start_res; 
		else if(KEY_ok==0)return KEY_ok_res; 
	}
    else if((KEY_add==1||KEY_sub==1||KEY_x10==1||KEY_mode==1||KEY_start==1||KEY_ok==1))key_up = 1;
	
    return 0;// �ް�������	
	
}

/*
*�������ƣ�STMFLASH_ReadHalfWord
*�������ܣ�����flash�ڰ���
*���������faddr
*���ز�����*(vu16*)faddr
*/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

/*
*�������ƣ�STMFLASH_Read
*�������ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
*���������ReadAddr:��ʼ��ַ pBuffer:����ָ�� NumToWrite:����(16λ)��
*���ز�����void
*/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
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

	if(key == KEY_add_res)//��
	{
        if(mode_flag == 1 && x10_flag == 0 )//�޸�P
		{
            Kp += 0.1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kp += 1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
        if(mode_flag == 2 && x10_flag == 0 )//�޸�I
		{
            Ki += 0.1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 2 && x10_flag == 1 )
		{
            Ki += 1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//д��һ��32λ����
			FLASH_Lock();//flash����
		}		
        if(mode_flag == 3 && x10_flag == 0 )//�޸�D
		{
            Kd += 0.1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*22,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kd += 1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*22,Kd);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
	}

	if(key == KEY_sub_res)//��
	{
        if(mode_flag == 1 && x10_flag == 0 )//�޸�P
		{
            Kp -= 0.1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kp -= 1;
			OLED_ShowNum(1,4,Kp,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*20);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*20,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
        if(mode_flag == 2 && x10_flag == 0 )//�޸�I
		{
            Ki -= 0.1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 2 && x10_flag == 1 )
		{
            Ki -= 1;
			OLED_ShowNum(1,4,Ki,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*21);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*21,Ki);//д��һ��32λ����
			FLASH_Lock();//flash����
		}		
        if(mode_flag == 3 && x10_flag == 0 )//�޸�D
		{
            Kd -= 0.1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*22,Kp);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
		else if(mode_flag == 1 && x10_flag == 1 )
		{
            Kd -= 1;
			OLED_ShowNum(1,4,Kd,3);
			FLASH_Unlock();// ����flash
			FLASH_ErasePage(0x08000000+1*1024*22);     //���õ�ַ0x800 0000+����flash  C8T6 1KBÿҳ  RCT6 2KBÿҳ
			FLASH_ProgramWord(0x08000000+1*1024*22,Kd);//д��һ��32λ����
			FLASH_Lock();//flash����
		}
	}
}

