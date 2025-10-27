#ifndef __Key_H
#define __Key_H
void Key_Init(void);
uint8_t Key_GetState(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);
int16_t Get_Key(uint8_t PreverPressedKey);
void Key_Tick_PID(void);
#endif
