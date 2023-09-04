#include "stm32f10x.h"                  // Device header

void Delay(__IO u32 nCount)//延时函数，通过循环的方式来达到延时的效果，也可以单独写一个文件，需要用时include进main.c                               include进main.c
{
  for(; nCount != 0; )
	{
1	nCount--;
	}
} 

void SystemInit();//Systemlnit初始化函数，用于配置时钟树，会在main函数调用之前被执行

int main(void)
{
	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	
	//配置结构体:
	Struct1.GPIO_Pin=GPIO_Pin_1;// 设置GPIO端口号为1
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 设置输出速率为50MHz
    Struct1.GPIO_Mode=GPIO_Mode_Out_PP;//一般设置为推挽输出模式就能够输出高/低电平了
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
	
	int i=0;
while(1)
	{
		Delay(0x50000);//Delay()当中的数字可根据实际设备的运行频率做相应调整
		i++;
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//输出设置。由于PA1口与LED负极相连，因此仅当输出低电平时LED灯才会亮,设置为低电平：
		Delay(0x50000);
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
}
