#include "LPC11xx.h"
#include "UART.h"
#define UART_BPS  9600

void UART_Init(void)
{
	uint16_t usFdiv;
	LPC_SYSCON ->SYSAHBCLKCTRL |=(1<<16);
	LPC_IOCON ->PIO1_6 &=~0x07;
	LPC_IOCON ->PIO1_6 |=0x01;
	LPC_IOCON ->PIO1_7 &=~0x07;
	LPC_IOCON ->PIO1_7 |=0x01;
	
	LPC_SYSCON ->SYSAHBCLKCTRL |=(1<<12);
	LPC_SYSCON ->UARTCLKDIV =0x01;
	LPC_UART ->LCR =0x83;
	usFdiv = (SystemCoreClock /LPC_SYSCON ->UARTCLKDIV /16)/9600 ;
	LPC_UART ->DLM =usFdiv /256;
	LPC_UART ->DLL =usFdiv %256;
	LPC_UART ->LCR =0x03;
	LPC_UART ->FCR =0x07;
}

void UART_SendByte(uint8_t ucDat)
{
	LPC_UART -> THR = ucDat;
	while((LPC_UART -> LSR & 0x40)==0);
}

void UART_SendStr(char *pucStr)
{
	while(1)
	{
		if(*pucStr == '\0')
			break;
		UART_SendByte (*pucStr ++);	
	}
}

