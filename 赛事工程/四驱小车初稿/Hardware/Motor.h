#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

/* ========================= 接线表 =========================
 * 四驱小车：左边两个电机并联接 A 通道，右边两个电机并联接 B 通道
 *
 *   功能         TB6612        L298N        STM32 引脚
 *   左轮调速     PWMA          ENA          PA6 (TIM3_CH1)
 *   左轮方向1    AIN1          IN1          PB12
 *   左轮方向2    AIN2          IN2          PB13
 *   右轮调速     PWMB          ENB          PA7 (TIM3_CH2)
 *   右轮方向1    BIN1          IN3          PB14
 *   右轮方向2    BIN2          IN4          PB15
 *   使能         STBY(接高)    不接          PB5
 *
 * 电源：电池正极 -> 驱动板 VM；驱动板 GND、单片机 GND 必须共地
 * 注意：PB5 悬空时 TB6612 不出力，这是"程序对但车不走"的头号原因
 * ========================================================= */

#define MOTOR_PORT_AIN1     GPIOB
#define MOTOR_PORT_AIN2     GPIOB
#define MOTOR_PORT_BIN1     GPIOB
#define MOTOR_PORT_BIN2     GPIOB
#define MOTOR_PORT_STBY     GPIOB

#define MOTOR_PIN_AIN1      GPIO_Pin_12
#define MOTOR_PIN_AIN2      GPIO_Pin_13
#define MOTOR_PIN_BIN1      GPIO_Pin_14
#define MOTOR_PIN_BIN2      GPIO_Pin_15
#define MOTOR_PIN_STBY      GPIO_Pin_5

/* 如果装好车后发现左右轮是反的，把对应的一行改成 1 即可，不用动接线 */
#define MOTOR_LEFT_REVERSE      0
#define MOTOR_RIGHT_REVERSE     0

/* 速度范围 -100 ~ 100，正数前进，负数后退 */
void Motor_Init(void);
void Motor_SetSpeed(int8_t LeftSpeed, int8_t RightSpeed);
void Motor_Stop(void);

/* 常用动作，Speed 取 0~100 */
void Motor_Forward(uint8_t Speed);
void Motor_Backward(uint8_t Speed);
void Motor_TurnLeft(uint8_t Speed);     /* 边走边左转，内侧轮减速 */
void Motor_TurnRight(uint8_t Speed);    /* 边走边右转，内侧轮减速 */
void Motor_SpinLeft(uint8_t Speed);     /* 原地左转，左轮反转 */
void Motor_SpinRight(uint8_t Speed);    /* 原地右转，右轮反转 */

#endif
