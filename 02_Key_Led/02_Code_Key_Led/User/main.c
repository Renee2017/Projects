#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Delay.h"
#include "Led.h"

void SystemInit();//Systemlnit初始化函数会在main函数调用之前被执行
//void Led_flash();//声明可以有多次,定义只能有一次

int main(void)
{
	//int i=0;
	//int i=3;
	SystemInit();
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能,在led.c中已经使能过，此处可省略
	Led_Init();
	Key_Init();
	//Led_flash_2();
	
while(1)
	{
		Key_tapKeyForLED_ReadLED();
		//Key_tapKeyForLED();
		//Key_KeepKeyForLED();
		//Led_on();
		//Led_flash();
	}
}