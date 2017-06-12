#include "LPC11xx.h"
#include "uart.h"
#include "delay.h"
#include "ADC.h"
#include "lcd.h"
#include "temp.h"
#include "stdio.h"
#include "KEY.h"

#define UART_BPS  9600
int a;
char  GcRcvBuf[20];
uint32_t y,c;

uint32_t temp(void)
{
  
	uint32_t ulADCData;
	uint32_t i;
	uint32_t ulADCBuf;
	uint32_t res_value;
	
	
    ulADCData=0;
    for(i=0;i<10;i++)
     {
			 LPC_ADC->CR |=(1<<24);
			 while((LPC_ADC ->DR[7]&0x80000000)==0);
			 LPC_ADC->CR |=(1<<24);
			 while((LPC_ADC ->DR[7]&0x80000000)==0);
			 ulADCBuf =LPC_ADC->DR[7];
			 ulADCBuf =(ulADCBuf >>6)&0x3ff;
			 ulADCData+=ulADCBuf ;
		 }	
		 
    ulADCData=ulADCData/10;
		 
    ulADCData=(ulADCData*3300)/1024;
		res_value = ((ulADCData*10000)/(3300-ulADCData));

if((res_value < 33970)&&(res_value > 20310))
	{
		c=136600;
		y=(33970-res_value )*1000/c;
	}
	else if((res_value < 20311)&&(res_value > 12570))
	{
		c=77400;
		y=((20310-res_value)*1000/c)+100;
	}
	else if((res_value<12571)&&(res_value>8034))
	{
		c=45370;
		y=((12570-res_value)*1000/c)+200;
	}
	else if((res_value<8035)&&(res_value>5298))
	{
		c=27370;
		y=((8034-res_value)*1000/c)+300;
	}
	else if((res_value<5299)&&(res_value>3586))
	{
		c=17130;
		y=((5295-res_value)*1000/c)+400;
	}
	else if((res_value<3587)&&(res_value>2484))
	{
		c=11030;
		y=((3586-res_value)*1000/y)+500;
	}
	else if((res_value<2485)&&(res_value>1756))
	{
		c=7280;
		y=((2484-res_value)*1000/c)+600;
	}
	
	       sprintf (GcRcvBuf,"temp=%fC", (float)y/10);
	    
	if((LPC_GPIO3->DATA&(1<<0))== KEY_DOWN)
	{
		   
		     LCD_DisplayStr(2,1,"*yaokecheng*");
		  
	}
	
	if((LPC_GPIO3->DATA&(1<<1))== KEY_DOWN)
	{  
		  
		     LCD_DisplayStr(2,1,"*zhangyuqing*");
		 
	}
	
       UART_SendStr(GcRcvBuf); 
            
}
	