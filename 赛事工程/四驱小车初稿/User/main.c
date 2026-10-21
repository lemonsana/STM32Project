#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Motor.h"

uint8_t Speed = 60;			// 默认速度 0~100，第一次测试建议先用 40

/* 第一行显示当前动作，用来对照现象 */
static void ShowState(char *State)
{
	OLED_ShowString(1, 1, State);
}

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	
	OLED_ShowString(2, 1, "KEY1 Demo ");
	OLED_ShowString(3, 1, "KEY2 Stop ");
	OLED_ShowString(4, 1, "Speed:");
	OLED_ShowNum(4, 7, Speed, 3);
	ShowState("Ready   ");
	
	while (1)
	{
		uint8_t KeyNum = Key_GetNum();
		
		if (KeyNum == 1)
		{
			/* ---- 第22集：GPIO 控制电机正反转 ---- */
			ShowState("Forward ");
			Motor_Forward(Speed);
			Delay_ms(1500);
			
			ShowState("Backward");
			Motor_Backward(Speed);
			Delay_ms(1500);
			
			/* ---- 第23集：PWM 调速，看占空比和转速的关系 ---- */
			ShowState("Speed30 ");
			Motor_Forward(30);
			Delay_ms(1000);
			ShowState("Speed60 ");
			Motor_Forward(60);
			Delay_ms(1000);
			ShowState("Speed100");
			Motor_Forward(100);
			Delay_ms(1000);
			
			/* ---- 差速转弯：内侧轮减速，车会画弧 ---- */
			ShowState("TurnL   ");
			Motor_TurnLeft(Speed);
			Delay_ms(1500);
			
			ShowState("TurnR   ");
			Motor_TurnRight(Speed);
			Delay_ms(1500);
			
			Motor_Stop();
			ShowState("Ready   ");
		}
		
		if (KeyNum == 2)
		{
			Motor_Stop();
			ShowState("Stop    ");
		}
	}
}
