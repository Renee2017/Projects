#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "usart1.h"

extern int STEP;
extern int T;

void delay_us(int value)
{
	{
		int i = value * 72;
		for(;i>0;)
			i --;
	}	
}

void delay_ms(int time)
{
int j=0;
int i=0;
for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}

int dist = 0;//定义为全局变量才能debug时在watch窗口看见

int main()
{
	SystemInit();
	Pin_Init();
	AFIO_Exit_NVIC_Init();
	Timer_init();
	 USART1_Config();
	
	while(1)
	{
		//printf("dist=%d",dist);
		STEP = 0;
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		delay_us(15);//通过向trig引脚发送一个短暂的高电平信号（通常10微秒）来触发超声波脉冲的发射
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		STEP = 1;
		delay_ms(1000);//远远大于中断需要的时间（0.5s触发一次），可以包含两种情况：1.当测距小于超声波测距范围时的情况，打印成功（step==3）
		//2.当测距远于超声波测距范围时的情况，打印失败(step==-1)
		if(STEP == 3)
		{
			dist = T*340/2;
			printf("dist=%f\n",(float)dist/1000);
		}
		
		//
	}
	
}
