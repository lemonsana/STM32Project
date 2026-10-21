#include "stm32f10x.h"    // Device header
#include "Delay.h"		//延时函数
#include "LED.h"		//led功能函数
#include "Key.h"		//初始化指定GPIO引脚+检测按键状态

uint8_t KeyNum;

int main(void)
{
	LED_Init();		//对PA1 PA2口初始化
	Key_Init();		//对PB1 PB11口初始化
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)		//key.c文件内定义为：PB1按下后 KeyNum=1
		{
			LED1_Turn();		//翻转状态
		}
		if (KeyNum == 2)		//PB11按下后 KeyNum=2
		{
			LED2_Turn();		//翻转状态
		}		
		
    }
}
