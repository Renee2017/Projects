#include "Led.h"
#include "Delay.h"// 这里是为了方便理解，正确做法是.c只包自己的.h  ,  如果led.c需要delay.h  就加在led.h。而不是led.c
//这样main.c 通过 led.h 把led.c包含时，也能一并把led.c依赖的delay.h一起包进来。
//#include "stm32f10x.h"                  // Device header

//void Led_Init()
GPIO_InitTypeDef Led_Init()
{
	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	
	//配置结构体:
	
	Struct1.GPIO_Pin=GPIO_Pin_1;// 设置GPIO端口号为1
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 设置输出速率为50MHz
    Struct1.GPIO_Mode=GPIO_Mode_Out_PP;//一般设置为推挽输出模式就能够输出高/低电平了
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
	return Struct1;
	//return ;
}


void Led_on();
void Led_off();

void Led_flash()
{
		Delay(0x50000);//Delay()当中的数字可根据实际设备的运行频率做相应调整
		//i++;
		Led_off();
		Delay(0x50000);
		Led_on();
		
}

//void Led_flash()
//{

//		Led_on();
//		
//}

void Led_off()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

void Led_on()
{
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//输出设置。由于PA1口与LED负极相连，因此仅当输出低电平时LED灯才会亮,设置为低电平：
}

