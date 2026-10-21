#include "stm32f10x.h"    // Device header
#include "Delay.h"		//延时函数
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1, 1, "Every day is");
	OLED_ShowString(2, 1, "a good day");
	
	while(1)
	{

    }
}
