#ifndef __ENCODER_H
#define __ENCODER_H

extern int16_t Encoder_Left_Count, Encoder_Right_Count;
void Encoder_Init(void);
int16_t Encoder_Left_Get(void);
int16_t Encoder_Right_Get(void);
#endif
