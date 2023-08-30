#include "Led_Exti.h"
#include "Led.h"



void AFIO_Exit_NVIC_Init()
{
	//GPIO初始化
	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	//配置结构体:
	Struct1.GPIO_Pin=GPIO_Pin_8;// 设置GPIO端口号为1
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 这里是GPIO的输出速度，因为工程是输入模式，所以也可以不写
    Struct1.GPIO_Mode=GPIO_Mode_IPU;//输入模式
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
	
	//初始化AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //打开AFIO时钟总线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);     //选择输入线，操作AFIO

	
	//初始化EXTI结构体(不需要开启时钟)
	EXTI_InitTypeDef EXTI_InitStruct;  //定义EXTI的结构体变量
	EXTI_InitStruct.EXTI_Line = EXTI_Line8;               //指定要配置的中断线,stm32c8t6单片机中，led灯对应io口为PA1，按键对应的io口为PA8
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;      //指定外部中断线的模式，设置是事件（EVENT）模式还是中断(Interrupt)模式，
												//中断响应是正常的流程，引脚电平变化触发中断事件响应不会触发中断，而是触发别的外设操作，属于外设之间的联合工作
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   //指定触发信号的有效边沿，
																//设置上升沿(Rising)还是下降沿触发(Falling)或者跳变沿触发（Rising_Falling）
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;                //指定所选中断线的新状态，分为ENABLE和DISABLE
	EXTI_Init(&EXTI_InitStruct);                          //将结构体传参.因为EXTI只有一个，所以不需要像GPIO那样先指定要配置哪个EXTI了

	//初始化嵌套向量控制寄存器，配置中断优先级。NVIC也不需要开启时钟，因为NVIC是内核内外设，rcc用于开启内核外·外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//配置优先级分组（嵌套向量控制寄存器中只有4个引脚控制优先级，一共有五组分组方式）：先占（抢占）优先级和从占（响应）。中端不多的情况下任意选，注意整个芯片只能有一种分组方式，如果有多个中断模块，要保证每个模块都是同一组分组方式
	
	NVIC_InitTypeDef NVIC_InitStruct;  //定义NVIC的结构体变量
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;//指定中断通道来开启或关闭，配置中断源（IRQn），具体参数在stm32f10x.h的170行找，不同IO口的中断源参数不同
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //  打开中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//设置所选通道的抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    //设置所选通道的响应优先级。优先级是在多个中断源同时申请，产生拥挤时才有效，只有一个中断时则随意设置
	NVIC_Init(&NVIC_InitStruct);	
}

void EXTI9_5_IRQHandler(void)//中断函数都是无参无返回值的。中断函数无需声明，因为它不需要调用，自动执行
{
	if (EXTI_GetITStatus(EXTI_Line8)==SET)//判断中断标志位，确保是我们想要的中断源触发的这个函数，因为EXTI5-EXTI9使用同一个中断服务函数，因为EXTI10-EXTI15使用同一个中断服务函数，如果pin脚在1-4，则不需要这一步
	Led_off();
	EXTI_ClearFlag(EXTI_Line8);//中断结束后，一定要再调用下清除中断标志位的函数，因为只要中断标志位置1（即中断成功）了，程序就会跳转到中断函数，如果不清除中断标志位，那它就会一直申请中断，就会极大消耗系统资源，无法正常处理其他业务
}
    