#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"

#define BEEP_CLK		RCC_APB2Periph_GPIOC
#define BEEP_Port		GPIOC
#define BEEP_Pin		GPIO_Pin_0

#define BEEP_ON			PCout(0) = 1
#define BEEP_OFF		PCout(0) = 0
#define BEEP_TOGGLE PCout(0) ^= 1

void BEEP_GPIO_Config(void);

#endif

