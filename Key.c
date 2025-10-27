#include "stm32f10x.h"                  // Device header
#include "Delay.h"


uint8_t Key_Num=0;

void Key_Init(void)
{
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitStruct1.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_InitStruct1.GPIO_Pin =GPIO_Pin_14;
	GPIO_InitStruct1.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct1);
}


uint8_t Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		return 1;//确认键
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		return 2;//下键
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
	{
		return 3;//上键
	}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0)
	{
		return 4;//返回键
	}
	
	return 0;
}


void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrentState, PreviousState;
	
	Count++;
	if(Count>=20)
	{
		Count=0;
		PreviousState =CurrentState;
		CurrentState=Key_GetState();
		if (CurrentState == 0 && PreviousState != 0)
		{
			Key_Num = PreviousState;
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}



/*
PID菜单的按键代码
*/
void Key_Tick_PID(void)
{
    static uint8_t Count;
    static uint8_t CurrentState, PreviousState;
    static uint8_t PreverPressedKey = 0;
    static int16_t Time_Flag = -1;
    static uint8_t LongPressTriggered = 0; // 标记长按是否已触发
    
    Count++;
    if(Count >= 20)
    {
        Count = 0;
        PreviousState = CurrentState;
        CurrentState = Key_GetState();
        
        // 改为按下时触发
        if (CurrentState != 0 && PreviousState == 0) // 按下时响应
        {
            Key_Num = CurrentState;
            PreverPressedKey = Key_Num;
            Time_Flag = 1000; // 重置长按计时器
            LongPressTriggered = 0; // 重置长按标记
        }
        // 松开时清零
        else if (CurrentState == 0 && PreviousState != 0)
        {
            Key_Num = 0;
            PreverPressedKey = 0;
            Time_Flag = -1;
            LongPressTriggered = 0;
        }
    }
    
    // 长按检测
    if(Time_Flag > 0)
    {
        Time_Flag--;
	}
        if(Time_Flag == 0 && !LongPressTriggered)
        {
            // 触发长按
            if(PreverPressedKey == 2) // 下键长按
            {
                Key_Num = 5;
                LongPressTriggered = 1;
            }
            else if(PreverPressedKey == 3) // 上键长按
            {
                Key_Num = 6;
                LongPressTriggered = 1;
            }
        }
        else if(Time_Flag == 0 && LongPressTriggered)
        {
            // 持续长按，重复触发
            Time_Flag = 100; // 设置较短的重复间隔
            if(PreverPressedKey == 2)
            {
                Key_Num = 5;
            }
            else if(PreverPressedKey == 3)
            {
                Key_Num = 6;
            }
        }

    }


int16_t Get_Key(uint8_t PreverPressedKey)
{

	if(PreverPressedKey==2)//下键长按
		return -1;
	if(PreverPressedKey==3)//上键长按
		return 1;
	
	
	return 0;
	

	
}














	

//GPIO的读取函数
//uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取输入数据寄存器某一端口的输入值
//uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);//读取整个输入数据寄存器
//uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取输出数据寄存器的某一位，一般用来看自己输出了什么
//uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);//读取整个输出寄存器

