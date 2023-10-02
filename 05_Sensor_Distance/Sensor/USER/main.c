#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "usart1.h"

extern int STEP;
extern int T;

void delay_us(int value)
{
	int i = value * 72 ;
	for(;i>0;)
		i --;
}
int dist = 0;//定义为全局变量才能debug时在watch窗口看见

int main()
{
	SystemInit();
	Pin_Init();
	AFIO_Exit_NVIC_Init();
	Timer_init();
	
	while(1)
	{
		STEP = 0;
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		delay_us(15);
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		STEP = 1;
		delay_us(600000);
		if(STEP == 3)
		{
			dist = T*340/2;
			printf("dist=%d",dist);
		}
		
		//
	}
	
}
