#ifndef __KEY_H
#define __KEY_H

#define KEY_add    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define KEY_sub    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define KEY_x10    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY_mode   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) 
#define KEY_ok     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) 
#define KEY_start  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

#define KEY_add_res    1
#define KEY_sub_res    2 
#define KEY_x10_res    3
#define KEY_mode_res   4
#define KEY_ok_res     5
#define KEY_start_res  6

void Key_Init(void);
uint8_t Key_Scan(u8 mode);
u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
void Flash_change(int key);
extern u8 key_flag;
#endif
