#include "LPC11xx.h"                    // Device header
#include "string.h"
#include "delay.h"
#include "LCD.h"

#define LCD_RS_LOW 		LPC_GPIO2->DATA &= ~(1<<0)	
#define LCD_RS_HIGH		LPC_GPIO2->DATA |=  (1<<0)	
#define LCD_RW_LOW		LPC_GPIO2->DATA &= ~(1<<1)	
#define LCD_RW_HIGH		LPC_GPIO2->DATA |=  (1<<1)	
#define LCD_E_LOW		  LPC_GPIO2->DATA &= ~(1<<2)	
#define LCD_E_HIGH		LPC_GPIO2->DATA |=  (1<<2)	

#define LCD_DATAPORT	LPC_GPIO2->DATA	            
#define LCD_DATA_IN		LPC_GPIO2->DIR &= ~0xFFF	
#define LCD_DATA_OUT	LPC_GPIO2->DIR |=  0xFFF	

static volatile unsigned int ticks = 0;

void Delayms(uint16_t ms)					
{
	SysTick->CTRL &= ~(1<<2);
	SysTick->LOAD = 5000*ms-1;			
	SysTick->VAL = 0;						
	SysTick->CTRL = ((1<<1)|(1<<0));       
	while(!ticks);						    
	ticks = 0;							    
	SysTick->CTRL = 0;	
}

void Delayus(uint16_t ms)					
{
	SysTick->CTRL &= ~(1<<2);
	SysTick->LOAD = 25*ms-1;			
	SysTick->VAL = 0;						
	SysTick->CTRL = ((1<<1)|(1<<0));       
	while(!ticks);						    
	ticks = 0;							    
	SysTick->CTRL = 0;	
}

void SysTick_Handler(void)
{
	ticks++;
}

uint8_t Busy_Check()
{
 	uint8_t LCD_Status;
	LCD_RS_LOW;
	LCD_RW_HIGH;
	LCD_E_HIGH;	
	Delayms(1);
	

    LPC_GPIO2->DIR &= ~(0x7F8);
    
    LCD_Status = (LPC_GPIO2->DATA & 0x7F8) >> 3;	
	
	LPC_GPIO2->DIR |= 0x7F8;
    
	LCD_E_LOW;	
	return LCD_Status;
}

void LCD_WriteCommand(uint32_t cmd)
{
 	while((Busy_Check()	&	0x80)	==	0x80);	
	LCD_RS_LOW;
	LCD_RW_LOW;
	LCD_E_LOW;	
    
	LPC_GPIO2->DATA = ((cmd << 3) & 0x7F8) | 0x00;	
	Delayms(1);	
	LCD_E_HIGH;
	Delayms(1);
	LCD_E_LOW;
}

void LCD_WriteData(uint32_t dat)
{
 	while((Busy_Check() & 0x80) == 0x80);	
	LCD_RS_HIGH;
	LCD_RW_LOW;
	LCD_E_LOW;	
	LPC_GPIO2->DATA = ((dat << 3) & 0x7F8) | 0x01;	
	Delayms(1);	
	LCD_E_HIGH;
	Delayms(1);
	LCD_E_LOW;
}

uint8_t LCD_DisplayChar(uint8_t x, uint8_t y, uint8_t ch)
{
	if ((y > 2) || (x > 16)) 
	{
	    return 1;
	}
	x |= ((y == 1) ? 0xC0 : 0x80); 
	LCD_WriteCommand(x); 
	LCD_WriteData(ch);
	return 0;
}

uint8_t LCD_DisplayStr(uint8_t x, uint8_t y, uint8_t *pStr)
{
  uint8_t i;
	uint8_t lineAddr[] = {0x80, 0xC0};

	if ((y >= 2) || (x >= 16))	
	{
	    return 1;
	}
	LCD_WriteCommand(lineAddr[y] + x);  
	for (i=x; ((i<16) && (*pStr!='\0')); i++)
	{
 		LCD_WriteData(*(pStr++)); 
	}
	if (*pStr != '\0') 
	{
 		x = 0; 
		if (++y < 2)
		{
		    LCD_DisplayStr(x, y, pStr);
		} 
	}
	return 0;
}

void LCD_IOInit(void)
{
	  LPC_GPIO2->DIR  |= 0xFFF;	
	  LPC_GPIO2->DIR  |= 0x0F;	
    LPC_GPIO0->DIR  |= 0x02;
    LPC_GPIO0->DATA |= (0x02);    
}

void LCD_Init()
{
	LCD_IOInit();	
 	LCD_WriteCommand(0x38);	
	Delayms(1);	
	LCD_WriteCommand(0x01);
	Delayms(1);	
	LCD_WriteCommand(0x06);	
	Delayms(1);	
	LCD_WriteCommand(0x0c);	
	Delayms(1);	
}
