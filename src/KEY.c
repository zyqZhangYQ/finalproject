#include "LPC11xx.h"
#include "delay.h"
#include "KEY.h"


void  KEY_Init(void)
{
		LPC_GPIO3->DIR&=~(1<<0);
    LPC_GPIO3->DIR&=~(1<<1);   
}

KEY_STATUS KEY_get(void )
{
  if((LPC_GPIO3->DATA&(1<<0))==0)  	
		{
			delay_ms(5);			  	
			if((LPC_GPIO3->DATA&(1<<0))==0)
			{
				return KEY_DOWN;
			}
		}
  else return KEY_UP;		

		if((LPC_GPIO3->DATA&(1<<1))==0)	   	
		{
			delay_ms(5);
			if((LPC_GPIO3->DATA&(1<<1))==0)	
			{
			  return KEY_DOWN;
			}
		}
			else return KEY_UP;
		
		if((LPC_GPIO3->DATA&(1<<2))==0)
		{
			delay_ms(5);			  	
			if((LPC_GPIO3->DATA&(1<<2))==0)	
				{
				return KEY_DOWN;
				}
	  }
		else return KEY_UP;
		
		if((LPC_GPIO3->DATA&(1<<3))==0)
		{
			delay_ms(5);			  	
			if((LPC_GPIO3->DATA&(1<<3))==0)	
				{
				return KEY_DOWN;
				}
	  }
		else return KEY_UP;
}
