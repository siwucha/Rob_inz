/*
 * gps.h
 *
 *  Created on: 4 lut 2016
 *      Author: Darek
 */

#ifndef GPS_H_
#define GPS_H_

#include "circular_buffer.h"
#include <stdbool.h>

#define MAX_RMC_FIELD 12

extern CircBuffer GPS_BUFFER;

typedef struct
{
	float time;
	char status;
	float latitude;
	char hemisphere_lati;
	float longitude;
	char hemisphere_long;
	float speed;
	float direction;
	int date;
} GpsDate;

void gpsInit();
bool gpsUpdate(GpsDate *gps, CircBuffer *gps_buffer);

#endif /* GPS_H_ */
