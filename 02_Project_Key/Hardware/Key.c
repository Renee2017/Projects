#include "Key.h"
#include "Delay.h"
#include "stm32f10x.h"
#include "Led.h"

void Key_Init()
{
	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	
	//配置结构体:
	Struct1.GPIO_Pin=GPIO_Pin_8;// 设置GPIO端口号为1
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 这里是GPIO的输出速度，因为工程是输入模式，所以也可以不写
    Struct1.GPIO_Mode=GPIO_Mode_IPU;//输入模式
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
}

//长按用if，会将所有按键看作长按，点按用while，while可以将长按转换成一次点按

void Key_KeepKeyForLED()//按键一直按住灯才会亮
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)//此函数表示读取输入寄存器的某一位
	{
		Delay(0x500);//delay来消抖
		//while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)//一个=是赋值,两个==才是等于的意思。 
			//且两个==是值相等,类型可以不相同。 所以,123=="123"的结果是true。 而三个===是等同于的意思,除了要值相等,类型也要相同。
		//Delay(0x500);
		Led_on();
	}
	else Led_off();
}

static int flag=0;

void Key_tapKeyForLED()//按键每次点按，灯都会改变状态
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)
	{
		Delay(0x20);//delay来消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0);
		//if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)此方案不可行，长按时会不断进入，导致flag不断翻转
		//Delay(0x500);
		//flag +=1;
		//{
			if (flag==0)
			{
				Led_on();
				flag = 1;
			}
			else
			{
				Led_off();
				flag = 0;
			}
		//}
	}
	
	
}


void Key_tapKeyForLED_ReadLED()//按键每次点按，都会根据灯当前亮灭去改变灯的状态
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)
	{
		Delay(0x20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0);
		//Delay(0x500);
		//flag +=1;
		//{
			if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)==1) //确定PA1输出高电压，而灯是低电平点亮
			{
				Led_on();
			}
			else
			{
				Led_off();
			}
		//}
	}	
}

void Key_new_tapKeyForLED_ReadLED()//按键每点按两次灯灭，每点按一次灯亮