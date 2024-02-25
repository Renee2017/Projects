#include "Sensor.h"


int T1=0;
int T2=0;
int T=0;
int STEP = 0;

//配置trig和echo对应的PA12和PA11
void Pin_Init()
{
	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	
	//配置结构体:
	
	Struct1.GPIO_Pin=GPIO_Pin_12;// 设置GPIO端口号为1
	Struct1.GPIO_Mode=GPIO_Mode_Out_PP;//一般设置为推挽输出模式就能够输出高/低电平了
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 设置输出速率为50MHz
    
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
	

	Struct1.GPIO_Pin=GPIO_Pin_11;
	Struct1.GPIO_Mode=GPIO_Mode_IPD;//设置为下拉输入
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 设置输出速率为50MHz
    
	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
	
	
}

void AFIO_Exit_NVIC_Init()
{
//	上个函数已经对GPIO进行初始化，此函数中不用重复进行该操作，所以注释掉
//	GPIO_InitTypeDef Struct1;// 在main函数开头定义一个含端口配置信息的结构体（或者工程选项，C/C++勾选C99 Mode,） 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
//	//配置结构体:
//	Struct1.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;;// 设置GPIO端口号为1
//    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// 这里是GPIO的输出速度，因为工程是输入模式，所以也可以不写
//    Struct1.GPIO_Mode=GPIO_Mode_IPU;//输入模式
//	GPIO_Init(GPIOA, &Struct1);//结构体初始化， 传入的是结构体的指针
//	
	//初始化AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //打开AFIO时钟总线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);     //选择输入线PA11(中断是由echo对应的pin角发生的)，操作AFIO

	
	//初始化EXTI结构体(不需要开启时钟)
	EXTI_InitTypeDef EXTI_InitStruct;  //定义EXTI的结构体变量
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;               //指定要配置的中断线,stm32c8t6单片机中，led灯对应io口为PA1，按键对应的io口为PA8
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;      //指定外部中断线的模式，设置是事件（EVENT）模式还是中断(Interrupt)模式，
												//中断响应是正常的流程，引脚电平变化触发中断事件响应不会触发中断，而是触发别的外设操作，属于外设之间的联合工作
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   //指定触发信号的有效边沿，
																//设置上升沿(Rising)还是下降沿触发(Falling)或者跳变沿触发（Rising_Falling）
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;                //指定所选中断线的新状态，分为ENABLE和DISABLE
	EXTI_Init(&EXTI_InitStruct);                          //将结构体传参.因为EXTI只有一个，所以不需要像GPIO那样先指定要配置哪个EXTI了

	//初始化嵌套向量控制寄存器，配置中断优先级。NVIC也不需要开启时钟，因为NVIC是内核内外设，rcc用于开启内核外·外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//配置优先级分组（嵌套向量控制寄存器中只有4个引脚控制优先级，一共有五组分组方式）：先占（抢占）优先级和从占（响应）。中端不多的情况下任意选，注意整个芯片只能有一种分组方式，如果有多个中断模块，要保证每个模块都是同一组分组方式
	
	NVIC_InitTypeDef NVIC_InitStruct;  //定义NVIC的结构体变量
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//指定中断通道来开启或关闭，配置中断源（IRQn），具体参数在stm32f10x.h的170行找，不同IO口的中断源参数不同
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //  打开中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//设置所选通道的抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    //设置所选通道的响应优先级。优先级是在多个中断源同时申请，产生拥挤时才有效，只有一个中断时则随意设置
	NVIC_Init(&NVIC_InitStruct);	
}

void Timer_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启APB1时钟，因为只有TIM1、8（高级定时器）是APB2总线的外设，其他定时器都是APB1总线的外设
	
	TIM_InternalClockConfig(TIM2);//选择内部时钟作为时基单元的时钟。此行代码可以省略，因为定时器默认时钟为内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//配置时基单元的结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//定时器的分频系数，确保定时器收到稳定信号
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//确定计数器的计数模式
	TIM_TimeBaseInitStructure.TIM_Period=50000-1;//确定ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//预分频器的值，参照公式：定时频率=72MHZ/(PSC+1)/(ARR+1)，设置定时频率
	//为2hz/s,也就是0.5s，也就是500ms为一个周期，则psc为7200-1，ARR为10000-1，注意它们的值都不能超过65535，因为是16位定时器
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
	
	//TIM_Cmd(TIM2, ENABLE);//启动定时器
}


void EXTI15_10_IRQHandler(void)//中断函数都是无参无返回值的。中断函数无需声明，因为它不需要调用，自动执行
{
	if (EXTI_GetITStatus(EXTI_Line11)==SET)//判断中断标志位，确保是我们想要的中断源触发的这个函数，因为EXTI5-EXTI9使用同一个中断服务函数，因为EXTI10-EXTI15使用同一个中断服务函数，如果pin脚在1-4，则不需要这一步
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==1)
		{

			TIM_SetCounter(TIM2,0);//设置计数器初始值为0，避免中断溢出（例如T1=499ms，T2=25ms，会造成数据错误）
			//超声波最大测试范围是450cm，按照光速来换算是13ms，所以来回路程需要26ms，此时T2-T1=25-499,不符合预期
			TIM_Cmd(TIM2, ENABLE);
			T1=TIM_GetCounter(TIM2);
			STEP = 2;
		}
		else
		{
			TIM_Cmd(TIM2, DISABLE);
			T2=TIM_GetCounter(TIM2);
			STEP = 3;
			T = T2 -T1;
		}
	}
		
	EXTI_ClearFlag(EXTI_Line11);//中断结束后，一定要再调用下清除中断标志位的函数，因为只要中断标志位置1（即中断成功）了，程序就会跳转到中断函数，如果不清除中断标志位，那它就会一直申请中断，就会极大消耗系统资源，无法正常处理其他业务
}


void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//定时器的中断标志位状态设置成功，说明触发了中断，因为每0.5s触发一次中断，
		//按照声速来计算是170m，远远超过传感器的最大可测范围，所以运行代码过程出现错误，串口无法正常打印出距离
	{
		STEP = -1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}

/*
{
   if(0->1)
  {
    SetTime(enable)	
  }
  if(1->0)
  {
    SetTime(disable)	
    vaild_flag = 1;
  }
}


Time_handler()
{
   vaild_flag = 0;
}
void Exit_Init()
{
	if (GPIO_SetBits(GPIOA,GPIO_Pin_10))//trig电平拉高
	{
		void EXTI15_10_IRQHandler(void)//中断函数都是无参无返回值的。中断函数无需声明，因为它不需要调用，自动执行
		{
			if (EXTI_GetITStatus(EXTI_Line10)==SET)//判断中断标志位，确保是我们想要的中断源触发的这个函数，因为EXTI5-EXTI9使用同一个中断服务函数，因为EXTI10-EXTI15使用同一个中断服务函数，如果pin脚在1-4，则不需要这一步
			{
				TIM_Cmd(TIM2, ENABLE);//启动定时器
				
				T1=TIM_GetCounter(TIM2);获取当前计数器的值
				Delay(0x15);
			}
			EXTI_ClearFlag(EXTI_Line10);//中断结束后，一定要再调用下清除中断标志位的函数，因为只要中断标志位置1（即中断成功）了，程序就会跳转到中断函数，如果不清除中断标志位，那它就会一直申请中断，就会极大消耗系统资源，无法正常处理其他业务
		}
	}	
	if GPIO_ResetBits(GPIOA,GPIO_Pin_11)//echo电平拉高
		void EXTI15_10_IRQHandler(void)//中断函数都是无参无返回值的。中断函数无需声明，因为它不需要调用，自动执行
		{
			if (EXTI_GetITStatus(EXTI_Line11)==SET)//判断中断标志位，确保是我们想要的中断源触发的这个函数，因为EXTI5-EXTI9使用同一个中断服务函数，因为EXTI10-EXTI15使用同一个中断服务函数，如果pin脚在1-4，则不需要这一步
				
				TIM_Cmd(TIM2, ENABLE);//启动定时器
				
				T2=TIM_GetCounter(TIM2);获取当前计数器的值
			EXTI_ClearFlag(EXTI_Line10);//中断结束后，一定要再调用下清除中断标志位的函数，因为只要中断标志位置1（即中断成功）了，程序就会跳转到中断函数，如果不清除中断标志位，那它就会一直申请中断，就会极大消耗系统资源，无法正常处理其他业务
		}
	T=T2-T1;
}

*/