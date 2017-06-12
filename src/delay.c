#include "LPC11xx.h"                    // Device header
#include "Delay.h"
static volatile unsigned int ticks;
void Delay(uint32_t ulTime)  
{
	uint32_t i;
	i=0;
	while(ulTime--)
	{
		for(i=0;i<6000;i++);
	}
}
void delay_ms(int ms)     
{
    SysTick->CTRL &= ~(1 << 2);      
    SysTick->LOAD = 20000*ms-1;            
    SysTick->VAL = 0;          
    SysTick->CTRL = ((1<<1) | (1<<0));          
    while(!ticks);             
    ticks = 0;             
    SysTick->CTRL = 0;          
}





