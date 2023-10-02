#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Led.h"



//void SystemInit();//Systemlnit初始化函数，用于配置时钟树，必须写在main函数里才会被执行

int main(void)
{
	SystemInit();
	Timer_init();
	Led_Init();
while(1)
	{
	}
}
