#include "OLED.h"

void OLED_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能gpio
	RCC_APB1PeriphClockCmd(i2C1_CLK, ENABLE);
	GPIO_InitTypeDef Struct2;	
	
	Struct2.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;// 设置GPIO端口号为4、5，分别对应I2C的SCL总线和SDA总线
	Struct2.GPIO_Mode=GPIO_Mode_Out_OD;//一般设置为开漏输出模式就能够输出高/低电平了
    Struct2.GPIO_Speed=GPIO_Speed_50MHz;// 设置输出速率为50MHz
    
    
	GPIO_Init(GPIOA, &Struct2);//结构体初始化， 传入的是结构体的指针
	
}

/*******************************************************************************
* 函数名         : I2cStart()
* 函数功能		 : 起始信号：在SCL时钟信号在高电平期间SDA信号产生一个下降沿
* 输入           : 无
* 输出         	 : 无
* 备注           : 起始之后SDA和SCL都为0
*******************************************************************************/

void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA=0;
	Delay10us();//保持时间是>4us
	SCL=0;			
	Delay10us();		
}
/*******************************************************************************
* 函数名         : I2cStop()
* 函数功能		 : 终止信号：在SCL时钟信号高电平期间SDA信号产生一个上升沿
* 输入           : 无
* 输出         	 : 无
* 备注           : 结束之后保持SDA和SCL都为1；表示总线空闲
*******************************************************************************/

void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间大于4.7us
	SDA=1;
	Delay10us();		
}
/*******************************************************************************
* 函数名         : I2cSendByte(unsigned char dat)
* 函数功能		 : 通过I2C发送一个字节。在SCL时钟信号高电平期间，保持发送信号SDA保持稳定
* 输入           : num
* 输出         	 : 0或1。发送成功返回1，发送失败返回0
* 备注           : 发送完一个字节SCL=0,SDA=1
*******************************************************************************/

unsigned char I2cSendByte(unsigned char dat)
{
	unsigned char a=0,b=0;//最大255，一个机器周期为1us，最大延时255us。		
	for(a=0;a<8;a++)//要发送8位，从最高位开始
	{
		SDA=dat>>7;	 //起始信号之后SCL=0，所以可以直接改变SDA信号
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//建立时间>4.7us
		SCL=0;
		Delay10us();//时间大于4us		
	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//等待应答，也就是等待从设备把SDA拉低
	{
		b++;
		if(b>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
		{
			SCL=0;
			Delay10us();
			return 0;
		}
	}
	SCL=0;
	Delay10us();
 	return 1;		
}