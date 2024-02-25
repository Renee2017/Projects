#include "OLED.h"

void OLED_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��ʹ��gpio
	RCC_APB1PeriphClockCmd(i2C1_CLK, ENABLE);
	GPIO_InitTypeDef Struct2;	
	
	Struct2.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;// ����GPIO�˿ں�Ϊ4��5���ֱ��ӦI2C��SCL���ߺ�SDA����
	Struct2.GPIO_Mode=GPIO_Mode_Out_OD;//һ������Ϊ��©���ģʽ���ܹ������/�͵�ƽ��
    Struct2.GPIO_Speed=GPIO_Speed_50MHz;// �����������Ϊ50MHz
    
    
	GPIO_Init(GPIOA, &Struct2);//�ṹ���ʼ���� ������ǽṹ���ָ��
	
}

/*******************************************************************************
* ������         : I2cStart()
* ��������		 : ��ʼ�źţ���SCLʱ���ź��ڸߵ�ƽ�ڼ�SDA�źŲ���һ���½���
* ����           : ��
* ���         	 : ��
* ��ע           : ��ʼ֮��SDA��SCL��Ϊ0
*******************************************************************************/

void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//����ʱ����SDA����ʱ��>4.7us
	SDA=0;
	Delay10us();//����ʱ����>4us
	SCL=0;			
	Delay10us();		
}
/*******************************************************************************
* ������         : I2cStop()
* ��������		 : ��ֹ�źţ���SCLʱ���źŸߵ�ƽ�ڼ�SDA�źŲ���һ��������
* ����           : ��
* ���         	 : ��
* ��ע           : ����֮�󱣳�SDA��SCL��Ϊ1����ʾ���߿���
*******************************************************************************/

void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//����ʱ�����4.7us
	SDA=1;
	Delay10us();		
}
/*******************************************************************************
* ������         : I2cSendByte(unsigned char dat)
* ��������		 : ͨ��I2C����һ���ֽڡ���SCLʱ���źŸߵ�ƽ�ڼ䣬���ַ����ź�SDA�����ȶ�
* ����           : num
* ���         	 : 0��1�����ͳɹ�����1������ʧ�ܷ���0
* ��ע           : ������һ���ֽ�SCL=0,SDA=1
*******************************************************************************/

unsigned char I2cSendByte(unsigned char dat)
{
	unsigned char a=0,b=0;//���255��һ����������Ϊ1us�������ʱ255us��		
	for(a=0;a<8;a++)//Ҫ����8λ�������λ��ʼ
	{
		SDA=dat>>7;	 //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//����ʱ��>4.7us
		SCL=0;
		Delay10us();//ʱ�����4us		
	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
	{
		b++;
		if(b>200)	 //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
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