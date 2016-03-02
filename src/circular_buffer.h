/*
 * circular_buffer.h
 *
 *  Created on: 4 lut 2016
 *      Author: Darek
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

typedef struct
{
	char elements[BUFFER_SIZE];
	uint16_t Beg;
	uint16_t Count;
} CircBuffer;

bool cbAdd(CircBuffer *cb, char elem);
char cbRead(CircBuffer *cb);
#endif /* CIRCULAR_BUFFER_H_ */
