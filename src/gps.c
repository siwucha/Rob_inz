/*
 * gps.c
 *
 *  Created on: 3 lut 2016
 *      Author: Darek
 */
#include "stm32f4xx.h"
#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include "console.h"	//potrzebene do debugowania

CircBuffer GPS_BUFFER = {{0}};

void gpsInit()
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;
	NVIC_InitTypeDef nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_25MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 9600;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_Mode = USART_Mode_Rx;
	USART_Init(UART4, &uart);

	USART_Cmd(UART4, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	nvic.NVIC_IRQChannel = UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void UART4_IRQHandler()
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		char gps_letter = USART_ReceiveData(UART4);
		if(gps_letter != 0) cbAdd(&GPS_BUFFER, gps_letter);
		send_char(gps_letter);	//potrzebne do debugowania
		send_char('a');
	}
	USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}

int gpsHeaderCheck(char *header, CircBuffer *gps_buffer)
{
	char ch = cbRead(gps_buffer);
	int checksum = 0;
	uint8_t i = 0;

	while((ch != '\0') && (ch != '$')) ch = cbRead(gps_buffer);
	if  (ch == '\0') return 0;

	while(i < 5)
	{
		ch = cbRead(gps_buffer);
		if(header[i++] != ch) return 0;
		checksum = checksum ^ ch;
	}
	return checksum;
}

bool fieldSearch(int *frame_field_position, int number_frame, char *frame_buffer, CircBuffer *gps_buffer)
{
	int i_field = 0;
	int i_character = 0;
	char ch = cbRead(gps_buffer);
	while((ch != '\0') && (ch != '*'))
	{
		if(ch == ',') frame_field_position[++i_field] = i_character + 1;
		frame_buffer[i_character++] = ch;
		ch = cbRead(gps_buffer);
	}
	return i_field == number_frame - 1;
}

void rmcUpdate(GpsDate *gps, int *frame_field_position, char *frame_buffer)
{
	char time[10];
	char status;
	char latitude[9];
	char hemisphere_lati;
	char longitude[10];
	char hemisphere_long;
	char speed[4];
	char direction[4];
	char date[6];
	//char magnetic_declination;
	//char declination_direction;

	int i;

	for(i=0; i<10; i++) time[i] = frame_buffer[frame_field_position[0] + i];
	gps->time = atof(time);

	status = frame_buffer[frame_field_position[1]];
	gps->status = status;

	for(i=0; i<9; i++) latitude[i] = frame_buffer[frame_field_position[2] + i];
	gps->latitude = atof(latitude);

	hemisphere_lati = frame_buffer[frame_field_position[3]];
	gps->hemisphere_lati = hemisphere_lati;

	for(i=0; i<10; i++) longitude[i] = frame_buffer[frame_field_position[4] + i];
	gps->longitude = atof(longitude);

	hemisphere_long = frame_buffer[frame_field_position[5]];
	gps->hemisphere_long = hemisphere_long;

	for(i=0; i<4; i++) speed[i] = frame_buffer[frame_field_position[6] + i];
	gps->speed = atof(speed);

	for(i=0; i<4; i++) direction[i] = frame_buffer[frame_field_position[7] + i];
	gps->direction = atof(direction);

	for(i=0; i<6; i++) date[i] = frame_buffer[frame_field_position[8] + i];
	gps->date = atoi(date);
}

bool gpsUpdate(GpsDate *gps, CircBuffer *gps_buffer)
{
	int count = (*gps_buffer).Count;
	if(count < 160) return false;

	char ch = 0;									//znak odczytany z bufora gps_buffer
	int checksum = 0;								//wyliczona suma kontrolna
	char hex_checksum[3] = {0};						//wyliczona suma kontrolna przekonwertowana na liczbê heksadecymaln¹
	char receive_checksum[2] = {0};					//suma kontrolna odebrana w ramce
	char frame_buffer[80] = {0};					//bufor aktualnie przetwarzanej ramki
	int frame_field_position[MAX_RMC_FIELD] = {0}; 	//tablica z pozycjami pierwszych elementow pól w ramce z frame_buffer
	bool number_field = false;						//flaga sygnalizuj¹ca ilosc odebranych danych
	int i = 0;

	char header[5] = {"GPRMC"};						//nag³ówek ramki
	checksum = gpsHeaderCheck(header, gps_buffer);
	ch = cbRead(gps_buffer);

	if(checksum && (ch == ','))
	{
		checksum = checksum ^ ch;
		number_field = fieldSearch(frame_field_position, MAX_RMC_FIELD, frame_buffer, gps_buffer);
		if(number_field)
		{
			i = 0;
			while(i<2)
			{
				ch = cbRead(gps_buffer);
				receive_checksum[i++] = ch;
			}
			i = 0;
			while(frame_buffer[i] != 0) checksum = checksum ^ frame_buffer[i++];
			sprintf(hex_checksum, "%02X", checksum);
			if((hex_checksum[0] == receive_checksum[0]) && (hex_checksum[1] == receive_checksum[1]))
			{
				rmcUpdate(gps, frame_field_position, frame_buffer);
				printf("Czas: %.4f\n", (*gps).time);
				printf("Status: %c\n", (*gps).status);
				printf("Szerokosc geograficzna: %.4f\n", (*gps).latitude);
				printf("Polkula szerkosci: %c\n", (*gps).hemisphere_lati);
				printf("Dlugosc geograficzna: %.4f\n", (*gps).longitude);
				printf("Polkula dlugosci: %c\n", (*gps).hemisphere_long);
				printf("Szybkosc: %.4f\n", (*gps).speed);
				printf("Kierunek: %.4f\n", (*gps).direction);
				printf("Date: %i\n", (*gps).date);
				return true;
			}
		}

	}
	return false;
}

