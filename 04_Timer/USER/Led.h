#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"      
#include "stm32f10x_gpio.h"

GPIO_InitTypeDef Led_Init();
void Led_off();
void Led_on();

#endif
