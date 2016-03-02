/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "console.h"
#include "gps.h"
#include <stdio.h>

int main(void)
{
	consoleInit();
	gpsInit();

	GpsDate GPS = {0};

	while(1)
	{
		gpsUpdate(&GPS, &GPS_BUFFER);
	}
}
