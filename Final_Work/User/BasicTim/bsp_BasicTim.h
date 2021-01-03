#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H

#include "stm32f10x.h"

/********************基本定时器TIM参数定义，只限TIM6、7************/
//#define BASIC_TIM6 // 如果使用TIM7，注释掉这个宏即可

//#ifdef  BASIC_TIM6 // 使用基本定时器TIM6
#define            BASIC_TIM_1_time                   TIM6
#define            BASIC_TIM_APBxClock_FUN_1_time     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK_1_time               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period_1_time            (1000-1)
#define            BASIC_TIM_Prescaler_1_time         (72-1)
#define            BASIC_TIM_IRQ_1_time               TIM6_IRQn
#define            BASIC_TIM_IRQHandler_1_time        TIM6_IRQHandler

//#else  // 使用基本定时器TIM7
#define            BASIC_TIM_1_LED                   TIM7
#define            BASIC_TIM_APBxClock_FUN_1_LED     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK_1_LED               RCC_APB1Periph_TIM7
#define            BASIC_TIM_Period_1_LED            1000-1
#define            BASIC_TIM_Prescaler_1_LED         71
#define            BASIC_TIM_IRQ_1_LED               TIM7_IRQn
#define            BASIC_TIM_IRQHandler_1_LED        TIM7_IRQHandler

#define						 ROOM_STATUS_TIM                   TIM3
#define						 ROOM_STATUS_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            ROOM_STATUS_TIM_CLK               RCC_APB1Periph_TIM3
#define            ROOM_STATUS_TIM_Period            1000-1
#define            ROOM_STATUS_TIM_Prescaler         72-1
#define            ROOM_STATUS_TIM_IRQ               TIM3_IRQn
#define            ROOM_STATUS_TIM_IRQHandler        TIM3_IRQHandler

#define						 BEEP_TIM                   TIM4
#define						 BEEP_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BEEP_TIM_CLK               RCC_APB1Periph_TIM4
#define            BEEP_TIM_Period            1000-1
#define            BEEP_TIM_Prescaler         72-1
#define            BEEP_TIM_IRQ               TIM4_IRQn
#define            BEEP_TIM_IRQHandler        TIM4_IRQHandler

#endif

void TIM_Init(void);

