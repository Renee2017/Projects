#include "Led_Exti.h"


//初始化AFIO
void AFIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //打开AFIO时钟总线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);     //选择输入线，操作AFIO
}

//初始化EXTI结构体(不需要开启时钟)
void Exit_Init()
{	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;               //设置GPIOA的第几个IO口
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;      //设置是事件（EVENT）还是中断(Interrupt)
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;   //设置上升沿(Rising)还是下降沿触发(Falling)或者跳变沿触发（Rising_Falling）
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;                //使能中断事件屏蔽寄存器
	EXTI_Init(&EXTI_InitStruct);                          //写入设置好的参数
}
static void EXTI_NVIC_Config(void)	//初始化嵌套向量控制寄存器，配置中断优先级。NVIC也不需要开启时钟，因为NVIC是内核内外设，rcc用于开启内核外·外设
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//配置优先级组（0—5），1个位主优先级，3个位表示次优先级
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//配置中断源（IRQn），具体参数在stm32f10x.h的170行找，不同IO口的中断源参数不同
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//设置抢占优先级（主优先级）
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    //设置次优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //  打开总中断
	NVIC_Init(&NVIC_InitStruct);
}

    