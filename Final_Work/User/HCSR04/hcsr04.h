#ifndef _HCSR04_H
#define _HCSR04_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "bsp_usart.h"

//stm32中unsigned char是8位
//unsigned int 和 unsigned long都是32位

#define HCSR04_GPIO_CLK RCC_APB2Periph_GPIOA
#define Trig_Port       GPIOA
#define Trig_Pin        GPIO_Pin_2
#define Echo_Port       GPIOA
#define Echo_Pin        GPIO_Pin_3

#define Echo_Status     PAin(3)

#define Trig_High       GPIO_SetBits(Trig_Port, Trig_Pin)
#define Trig_Low        GPIO_ResetBits(Trig_Port, Trig_Pin)
#define Echo_High       GPIO_SetBits(Echo_Port, Echo_Pin)
#define Echo_Low        GPIO_ResetBits(Echo_Port, Echo_Pin)

#define Echo_TIM                   TIM2
#define Echo_TIM_IRQ               TIM2_IRQn
#define Echo_TIM_IRQHandler        TIM2_IRQHandler
#define Echo_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define Echo_TIM_CLK               RCC_APB1Periph_TIM2

//定时器中断一次1us
#define Echo_TIM_Period            1
#define Echo_TIM_Prescaler         35


void HCSR04_Init(void);
float Hcsr04_Get_Distance(void);

#endif

