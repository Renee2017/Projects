#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"                  // Device header


//void Led_Init();
GPIO_InitTypeDef Led_Init();
void Led_on();
void Led_off();
void Led_flash();
void Led_flash();
void Led_flash();
//void Led_flash_2();函数在.h中声明了，但是未在.c中定义，编译会报错

#endif