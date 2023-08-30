#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"                  // Device header

void Key_Init();
void Key_KeepKeyForLED();
void Key_tapKeyForLED();
void Key_tapKeyForLED_ReadLED();

#endif