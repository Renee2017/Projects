#include "Sensor.h"


int T1=0;
int T2=0;
int T=0;
int STEP = 0;

//����trig��echo��Ӧ��PA12��PA11
void Pin_Init()
{
	GPIO_InitTypeDef Struct1;// ��main������ͷ����һ�����˿�������Ϣ�Ľṹ�壨���߹���ѡ�C/C++��ѡC99 Mode,�� 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��ʹ��
	
	//���ýṹ��:
	
	Struct1.GPIO_Pin=GPIO_Pin_12;// ����GPIO�˿ں�Ϊ1
	Struct1.GPIO_Mode=GPIO_Mode_Out_PP;//һ������Ϊ�������ģʽ���ܹ������/�͵�ƽ��
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// �����������Ϊ50MHz
    
	GPIO_Init(GPIOA, &Struct1);//�ṹ���ʼ���� ������ǽṹ���ָ��
	

	Struct1.GPIO_Pin=GPIO_Pin_11;
	Struct1.GPIO_Mode=GPIO_Mode_IPD;//����Ϊ��������
    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// �����������Ϊ50MHz
    
	GPIO_Init(GPIOA, &Struct1);//�ṹ���ʼ���� ������ǽṹ���ָ��
	
	
}

void AFIO_Exit_NVIC_Init()
{
//	�ϸ������Ѿ���GPIO���г�ʼ�����˺����в����ظ����иò���������ע�͵�
//	GPIO_InitTypeDef Struct1;// ��main������ͷ����һ�����˿�������Ϣ�Ľṹ�壨���߹���ѡ�C/C++��ѡC99 Mode,�� 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��ʹ��
//	//���ýṹ��:
//	Struct1.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;;// ����GPIO�˿ں�Ϊ1
//    Struct1.GPIO_Speed=GPIO_Speed_50MHz;// ������GPIO������ٶȣ���Ϊ����������ģʽ������Ҳ���Բ�д
//    Struct1.GPIO_Mode=GPIO_Mode_IPU;//����ģʽ
//	GPIO_Init(GPIOA, &Struct1);//�ṹ���ʼ���� ������ǽṹ���ָ��
//	
	//��ʼ��AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //��AFIOʱ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);     //ѡ��������PA11(�ж�����echo��Ӧ��pin�Ƿ�����)������AFIO

	
	//��ʼ��EXTI�ṹ��(����Ҫ����ʱ��)
	EXTI_InitTypeDef EXTI_InitStruct;  //����EXTI�Ľṹ�����
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;               //ָ��Ҫ���õ��ж���,stm32c8t6��Ƭ���У�led�ƶ�Ӧio��ΪPA1��������Ӧ��io��ΪPA8
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;      //ָ���ⲿ�ж��ߵ�ģʽ���������¼���EVENT��ģʽ�����ж�(Interrupt)ģʽ��
												//�ж���Ӧ�����������̣����ŵ�ƽ�仯�����ж��¼���Ӧ���ᴥ���жϣ����Ǵ�����������������������֮������Ϲ���
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   //ָ�������źŵ���Ч���أ�
																//����������(Rising)�����½��ش���(Falling)���������ش�����Rising_Falling��
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;                //ָ����ѡ�ж��ߵ���״̬����ΪENABLE��DISABLE
	EXTI_Init(&EXTI_InitStruct);                          //���ṹ�崫��.��ΪEXTIֻ��һ�������Բ���Ҫ��GPIO������ָ��Ҫ�����ĸ�EXTI��

	//��ʼ��Ƕ���������ƼĴ����������ж����ȼ���NVICҲ����Ҫ����ʱ�ӣ���ΪNVIC���ں������裬rcc���ڿ����ں��⡤����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�������ȼ����飨Ƕ���������ƼĴ�����ֻ��4�����ſ������ȼ���һ����������鷽ʽ������ռ����ռ�����ȼ��ʹ�ռ����Ӧ�����ж˲�������������ѡ��ע������оƬֻ����һ�ַ��鷽ʽ������ж���ж�ģ�飬Ҫ��֤ÿ��ģ�鶼��ͬһ����鷽ʽ
	
	NVIC_InitTypeDef NVIC_InitStruct;  //����NVIC�Ľṹ�����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//ָ���ж�ͨ����������رգ������ж�Դ��IRQn�������������stm32f10x.h��170���ң���ͬIO�ڵ��ж�Դ������ͬ
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //  ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//������ѡͨ������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    //������ѡͨ������Ӧ���ȼ������ȼ����ڶ���ж�Դͬʱ���룬����ӵ��ʱ����Ч��ֻ��һ���ж�ʱ����������
	NVIC_Init(&NVIC_InitStruct);	
}

void Timer_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����APB1ʱ�ӣ���Ϊֻ��TIM1��8���߼���ʱ������APB2���ߵ����裬������ʱ������APB1���ߵ�����
	
	TIM_InternalClockConfig(TIM2);//ѡ���ڲ�ʱ����Ϊʱ����Ԫ��ʱ�ӡ����д������ʡ�ԣ���Ϊ��ʱ��Ĭ��ʱ��Ϊ�ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����ʱ����Ԫ�Ľṹ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ʱ���ķ�Ƶϵ����ȷ����ʱ���յ��ȶ��ź�
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//ȷ���������ļ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=50000-1;//ȷ��ARR�Զ���װ����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//Ԥ��Ƶ����ֵ�����չ�ʽ����ʱƵ��=72MHZ/(PSC+1)/(ARR+1)�����ö�ʱƵ��
	//Ϊ2hz/s,Ҳ����0.5s��Ҳ����500msΪһ�����ڣ���pscΪ7200-1��ARRΪ10000-1��ע�����ǵ�ֵ�����ܳ���65535����Ϊ��16λ��ʱ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ���������ֵ�����ڸ߼�������������Ŀ�ò�����ȡֵΪ0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ�ܸ����ж�,��ʱ�����˸����жϵ�NVIC��ͨ·
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�������ȼ����飨Ƕ���������ƼĴ�����ֻ��4�����ſ������ȼ���һ����������鷽ʽ������ռ����ռ�����ȼ��ʹ�ռ����Ӧ�����ж˲�������������ѡ��ע������оƬֻ����һ�ַ��鷽ʽ������ж���ж�ģ�飬Ҫ��֤ÿ��ģ�鶼��ͬһ����鷽ʽ
	
	NVIC_InitTypeDef NVIC_InitStructure;  //����NVIC�Ľṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//ָ���ж�ͨ����������رգ������ж�Դ��IRQn�������������stm32f10x.h��170���ң���ͬIO�ڵ��ж�Դ������ͬ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //  ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//������ѡͨ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //������ѡͨ������Ӧ���ȼ������ȼ����ڶ���ж�Դͬʱ���룬����ӵ��ʱ����Ч��ֻ��һ���ж�ʱ����������
	NVIC_Init(&NVIC_InitStructure);	
	
	//TIM_Cmd(TIM2, ENABLE);//������ʱ��
}


void EXTI15_10_IRQHandler(void)//�жϺ��������޲��޷���ֵ�ġ��жϺ���������������Ϊ������Ҫ���ã��Զ�ִ��
{
	if (EXTI_GetITStatus(EXTI_Line11)==SET)//�ж��жϱ�־λ��ȷ����������Ҫ���ж�Դ�����������������ΪEXTI5-EXTI9ʹ��ͬһ���жϷ���������ΪEXTI10-EXTI15ʹ��ͬһ���жϷ����������pin����1-4������Ҫ��һ��
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==1)
		{

			TIM_SetCounter(TIM2,0);//���ü�������ʼֵΪ0�������ж����������T1=499ms��T2=25ms����������ݴ���
			//�����������Է�Χ��450cm�����չ�����������13ms����������·����Ҫ26ms����ʱT2-T1=25-499,������Ԥ��
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
		
	EXTI_ClearFlag(EXTI_Line11);//�жϽ�����һ��Ҫ�ٵ���������жϱ�־λ�ĺ�������ΪֻҪ�жϱ�־λ��1�����жϳɹ����ˣ�����ͻ���ת���жϺ��������������жϱ�־λ�������ͻ�һֱ�����жϣ��ͻἫ������ϵͳ��Դ���޷�������������ҵ��
}


void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//��ʱ�����жϱ�־λ״̬���óɹ���˵���������жϣ���Ϊÿ0.5s����һ���жϣ�
		//����������������170m��ԶԶ���������������ɲⷶΧ���������д�����̳��ִ��󣬴����޷�������ӡ������
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
	if (GPIO_SetBits(GPIOA,GPIO_Pin_10))//trig��ƽ����
	{
		void EXTI15_10_IRQHandler(void)//�жϺ��������޲��޷���ֵ�ġ��жϺ���������������Ϊ������Ҫ���ã��Զ�ִ��
		{
			if (EXTI_GetITStatus(EXTI_Line10)==SET)//�ж��жϱ�־λ��ȷ����������Ҫ���ж�Դ�����������������ΪEXTI5-EXTI9ʹ��ͬһ���жϷ���������ΪEXTI10-EXTI15ʹ��ͬһ���жϷ����������pin����1-4������Ҫ��һ��
			{
				TIM_Cmd(TIM2, ENABLE);//������ʱ��
				
				T1=TIM_GetCounter(TIM2);��ȡ��ǰ��������ֵ
				Delay(0x15);
			}
			EXTI_ClearFlag(EXTI_Line10);//�жϽ�����һ��Ҫ�ٵ���������жϱ�־λ�ĺ�������ΪֻҪ�жϱ�־λ��1�����жϳɹ����ˣ�����ͻ���ת���жϺ��������������жϱ�־λ�������ͻ�һֱ�����жϣ��ͻἫ������ϵͳ��Դ���޷�������������ҵ��
		}
	}	
	if GPIO_ResetBits(GPIOA,GPIO_Pin_11)//echo��ƽ����
		void EXTI15_10_IRQHandler(void)//�жϺ��������޲��޷���ֵ�ġ��жϺ���������������Ϊ������Ҫ���ã��Զ�ִ��
		{
			if (EXTI_GetITStatus(EXTI_Line11)==SET)//�ж��жϱ�־λ��ȷ����������Ҫ���ж�Դ�����������������ΪEXTI5-EXTI9ʹ��ͬһ���жϷ���������ΪEXTI10-EXTI15ʹ��ͬһ���жϷ����������pin����1-4������Ҫ��һ��
				
				TIM_Cmd(TIM2, ENABLE);//������ʱ��
				
				T2=TIM_GetCounter(TIM2);��ȡ��ǰ��������ֵ
			EXTI_ClearFlag(EXTI_Line10);//�жϽ�����һ��Ҫ�ٵ���������жϱ�־λ�ĺ�������ΪֻҪ�жϱ�־λ��1�����жϳɹ����ˣ�����ͻ���ת���жϺ��������������жϱ�־λ�������ͻ�һֱ�����жϣ��ͻἫ������ϵͳ��Դ���޷�������������ҵ��
		}
	T=T2-T1;
}

*/