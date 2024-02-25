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

int dist = 0;//����Ϊȫ�ֱ�������debugʱ��watch���ڿ���

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
		delay_us(15);//ͨ����trig���ŷ���һ�����ݵĸߵ�ƽ�źţ�ͨ��10΢�룩����������������ķ���
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		STEP = 1;
		delay_ms(1000);//ԶԶ�����ж���Ҫ��ʱ�䣨0.5s����һ�Σ������԰������������1.�����С�ڳ�������෶Χʱ���������ӡ�ɹ���step==3��
		//2.�����Զ�ڳ�������෶Χʱ���������ӡʧ��(step==-1)
		if(STEP == 3)
		{
			dist = T*340/2;
			printf("dist=%f\n",(float)dist/1000);
		}
		
		//
	}
	
}
