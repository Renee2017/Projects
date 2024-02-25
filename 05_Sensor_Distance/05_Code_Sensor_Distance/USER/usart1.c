/************************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>//�ڱ�д�� printf �� scanf �����ĺ���ʱ��Ҫ


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART1 ʱ�Ӻ����Ӧ�˿�GPIOAʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 ʹ��IO�˿�����,�� STM32F103 ϵ��΢�������У�
				UART1 ��Ӧ�������� ��������PA9��USART1_TX���� ��������PA10��USART1_RX���� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
				/* ���� USART1 �Ľ������� PA10����Ĭ������±�����Ϊ��������ģʽ��
				������Ϊ USART1 �Ľ��չ�����Ҫ�ܹ������ⲿ�豸���͵����ݣ�
				��˽�������Ϊ���������Ա�����ⲿ�豸���͵������źš�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
				//�����ٶ�һ��ֻ��Ҫ����һ��,ǰ���TX�Ѿ����������ʡ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	  
	/* USART1 ����ģʽ���� */
	
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200(����С���������TX��RX��
	                                  //û��CLK������֧�ֵ����첽ͨѶ����Ҫ���ò�����,������˽��첽ͨѶ��clk���ã����Բ���spiЭ��)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ�����������У��λ��Ҫ����Ϊ9
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
	
	USART_Cmd(USART1, ENABLE);// USART1ʹ��

}


 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}



