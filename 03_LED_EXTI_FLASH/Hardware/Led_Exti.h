#ifndef _LED_EXTI_H_
#define _LED_EXTI_H_
#include "stm32f10x_exti.h"
#include "misc.h"//因为NVIC是内核外设，所以它的库函数被ST发配到杂项里来了

#include "stm32f10x.h"                  // Device header

void AFIO_Exit_NVIC_Init();
	
#endif
