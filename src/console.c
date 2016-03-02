/*
 * console.c
 *
 *  Created on: 3 lut 2016
 *      Author: Darek
 */
#include "stm32f4xx.h"
#include "console.h"

void consoleInit()
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_25MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 9600;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &uart);
	USART_Cmd(USART3, ENABLE);
}

void send_char(char ch)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, ch);
}

int __io_putchar(int ch)
{
	if (ch == '\n')
		send_char('\r');
	send_char(ch);
	return ch;
}
