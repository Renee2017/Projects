#include "Timer.h"


void Timer_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启APB1时钟，因为只有TIM1是APB2总线的外设，TIM1-8都是APB1总线的外设
	
	TIM_InternalClockConfig(TIM2);//选择内部时钟作为时基单元的时钟。此行代码可以省略，因为定时器默认时钟为内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//配置时基单元的结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//定时器的分频系数，确保定时器收到稳定信号
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//确定计数器的计数模式
	TIM_TimeBaseInitStructure.TIM_Period=9999;//确定ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;//预分频器的值，参照公式：定时频率=72MHZ/(PSC+1)/(ARR+1)，设置定时频率
	//为1s，则psc为7200-1，ARR为10000-1，注意它们的值都不能超过65535，因为是16位定时器
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器的值，用于高级计数器，此项目用不到，取值为0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//时基单元初始化
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能更新中断,此时开启了更新中断到NVIC的通路
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置优先级分组（嵌套向量控制寄存器中只有4个引脚控制优先级，一共有五组分组方式）：先占（抢占）优先级和从占（响应）。中端不多的情况下任意选，注意整个芯片只能有一种分组方式，如果有多个中断模块，要保证每个模块都是同一组分组方式
	
	NVIC_InitTypeDef NVIC_InitStructure;  //定义NVIC的结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//指定中断通道来开启或关闭，配置中断源（IRQn），具体参数在stm32f10x.h的170行找，不同IO口的中断源参数不同
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //  打开中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//设置所选通道的抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //设置所选通道的响应优先级。优先级是在多个中断源同时申请，产生拥挤时才有效，只有一个中断时则随意设置
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM2, ENABLE);//启动定时器
}
int i=0;
void TIM2_IRQHandler(void)
{
	i++;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		if (i%2==0)
			Led_off();
		else
			Led_on();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}

//void TIM_DeInit(TIM_TypeDef* TIMx);恢复缺省配置
//void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);时基单元初始化
//void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
//void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);使能记数器，对应“定时中断基本结构图”中的运行控制
//void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);使能中断输出信号，对应“定时中断基本结构图”中的中断输出控制

//以下对应时基单元的选择部分
//void TIM_InternalClockConfig(TIM_TypeDef* TIMx);选择内部时钟
//void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);选择ITRx其他定时器的时钟
//void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,选择TIX捕获通道的时钟
//                                uint16_t TIM_ICPolarity, uint16_t ICFilter);
//void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
//                             uint16_t ExtTRGFilter);选择ETR通过外部时钟模式一输入的时钟
//void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
//                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);选择ETR通过外部时钟模式一输入的时钟
//void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
//                   uint16_t ExtTRGFilter);
//void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);用于单独写预分频值
//void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode);用于改变计数器的计数模式
//void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);自动重装器预装功能的配置
//void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);给计数器手动写一个值
//void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload);给自动重装器手动写一个值
//uint16_t TIM_GetCounter(TIM_TypeDef* TIMx);获取当前计数器的值
//uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx);
