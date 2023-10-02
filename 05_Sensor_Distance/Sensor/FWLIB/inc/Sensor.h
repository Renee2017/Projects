#ifndef _SENSOR_H_
#define _SENSOR_H_
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "misc.h"
void Pin_Init();
void AFIO_Exit_NVIC_Init();
void Timer_init(void);
#endif