/************************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>//在编写像 printf 或 scanf 这样的函数时需要


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟和其对应端口GPIOA时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置,在 STM32F103 系列微控制器中，
				UART1 对应的引脚是 发送引脚PA9（USART1_TX）和 接收引脚PA10（USART1_RX）。 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
				/* 对于 USART1 的接收引脚 PA10，在默认情况下被配置为浮空输入模式。
				这是因为 USART1 的接收功能需要能够接收外部设备发送的数据，
				因此将其配置为浮空输入以便接收外部设备发送的数据信号。*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
				//配置速度一般只需要进行一次,前面的TX已经配过，这里省略
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200(串口小板的引脚由TX、RX，
	                                  //没有CLK，所以支持的是异步通讯，需要设置波特率,如果想了解异步通讯的clk配置，可以参照spi协议)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位，如果有数据校验位就要设置为9
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	
	USART_Cmd(USART1, ENABLE);// USART1使能

}


 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}



