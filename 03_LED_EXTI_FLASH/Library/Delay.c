#include "Delay.h"

void Delay(unsigned long  nCount)//延时函数，通过循环的方式来达到延时的效果，也可以单独写一个文件，需要用时include进main.c                               include进main.c
{
  for(; nCount != 0; )
	{
	nCount--;
	}
} 
