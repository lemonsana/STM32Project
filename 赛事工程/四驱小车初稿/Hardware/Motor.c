#include "stm32f10x.h"                  // Device header
#include "Motor.h"

/* PWM 周期：计 100 个数，对应占空比 0~100% */
#define MOTOR_PERIOD    100

/**
  * @brief  把 -100~100 的速度值限幅，防止参数写错导致失控
  */
static int8_t Motor_Limit(int8_t Speed)
{
	if (Speed > 100)  return 100;
	if (Speed < -100) return -100;
	return Speed;
}

/**
  * @brief  电机初始化：方向脚配置 + TIM3 双通道 PWM（10kHz）
  */
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* PA6、PA7 交给定时器输出 PWM，必须是复用推挽 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 方向脚和 STBY 是普通推挽输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_AIN1 | MOTOR_PIN_AIN2 |
	                              MOTOR_PIN_BIN1 | MOTOR_PIN_BIN2 |
	                              MOTOR_PIN_STBY;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* TB6612 的 STBY 拉高才会输出，低电平等于全车断电 */
	GPIO_SetBits(MOTOR_PORT_STBY, MOTOR_PIN_STBY);
	
	/* 时基：72MHz / 72 = 1MHz，1MHz / 100 = 10kHz */
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = MOTOR_PERIOD - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/* 输出比较：PWM1 模式，高电平有效 */
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);		// CH1 -> PA6 左轮
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);		// CH2 -> PA7 右轮
	
	TIM_Cmd(TIM3, ENABLE);
	
	Motor_Stop();
}

/**
  * @brief  设置左右轮速度
  * @param  LeftSpeed  -100~100，正数前进
  * @param  RightSpeed -100~100，正数前进
  */
void Motor_SetSpeed(int8_t LeftSpeed, int8_t RightSpeed)
{
	LeftSpeed  = Motor_Limit(LeftSpeed);
	RightSpeed = Motor_Limit(RightSpeed);
	
#if MOTOR_LEFT_REVERSE
	LeftSpeed = -LeftSpeed;
#endif
#if MOTOR_RIGHT_REVERSE
	RightSpeed = -RightSpeed;
#endif
	
	/* 左轮：正转 AIN1=1、AIN2=0，反转反过来；PWM 只认正数，方向由引脚决定 */
	if (LeftSpeed >= 0)
	{
		GPIO_SetBits(MOTOR_PORT_AIN1, MOTOR_PIN_AIN1);
		GPIO_ResetBits(MOTOR_PORT_AIN2, MOTOR_PIN_AIN2);
		TIM_SetCompare1(TIM3, LeftSpeed);
	}
	else
	{
		GPIO_ResetBits(MOTOR_PORT_AIN1, MOTOR_PIN_AIN1);
		GPIO_SetBits(MOTOR_PORT_AIN2, MOTOR_PIN_AIN2);
		TIM_SetCompare1(TIM3, -LeftSpeed);
	}
	
	/* 右轮 */
	if (RightSpeed >= 0)
	{
		GPIO_SetBits(MOTOR_PORT_BIN1, MOTOR_PIN_BIN1);
		GPIO_ResetBits(MOTOR_PORT_BIN2, MOTOR_PIN_BIN2);
		TIM_SetCompare2(TIM3, RightSpeed);
	}
	else
	{
		GPIO_ResetBits(MOTOR_PORT_BIN1, MOTOR_PIN_BIN1);
		GPIO_SetBits(MOTOR_PORT_BIN2, MOTOR_PIN_BIN2);
		TIM_SetCompare2(TIM3, -RightSpeed);
	}
}

/**
  * @brief  停车（四路方向全 0，电机两端短接，属于刹车）
  */
void Motor_Stop(void)
{
	GPIO_ResetBits(MOTOR_PORT_AIN1, MOTOR_PIN_AIN1);
	GPIO_ResetBits(MOTOR_PORT_AIN2, MOTOR_PIN_AIN2);
	GPIO_ResetBits(MOTOR_PORT_BIN1, MOTOR_PIN_BIN1);
	GPIO_ResetBits(MOTOR_PORT_BIN2, MOTOR_PIN_BIN2);
	TIM_SetCompare1(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);
}

void Motor_Forward(uint8_t Speed)
{
	Motor_SetSpeed(Speed, Speed);
}

void Motor_Backward(uint8_t Speed)
{
	Motor_SetSpeed(-Speed, -Speed);
}

/* 左转：右轮快、左轮慢 */
void Motor_TurnLeft(uint8_t Speed)
{
	Motor_SetSpeed(Speed / 2, Speed);
}

/* 右转：左轮快、右轮慢 */
void Motor_TurnRight(uint8_t Speed)
{
	Motor_SetSpeed(Speed, Speed / 2);
}

void Motor_SpinLeft(uint8_t Speed)
{
	Motor_SetSpeed(-Speed, Speed);
}

void Motor_SpinRight(uint8_t Speed)
{
	Motor_SetSpeed(Speed, -Speed);
}
