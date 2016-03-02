/*
 * position.h
 *
 *  Created on: 14 lut 2016
 *      Author: Makumba
 */

#ifndef POSITION_H_
#define POSITION_H_

#include "gps.h"
#include <math.h>

#define EARTH_R 6371000
#define WRONG_LATI (540/(M_PI*EARTH_R))
#define WRONG_LONGI (540/(M_PI*EARTH_R*cos(latitude)))

typedef struct
{
	double latitude0;
	double longitude0;
} Position0;

void getPosition0(Position0 *pos, GpsDate *gps, CircBuffer *gps_buffer);

#endif /* POSITION_H_ */
