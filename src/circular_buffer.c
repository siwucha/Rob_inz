/*
 * circular_buffer.c
 *
 *  Created on: 4 lut 2016
 *      Author: Darek
 */
#include "stm32f4xx.h"
#include "circular_buffer.h"

static inline bool cbIsFull(CircBuffer *cb) { return  cb->Count == BUFFER_SIZE; }
static inline bool cbIsEmpty(CircBuffer *cb) { return cb->Count == 0; }

bool cbAdd(CircBuffer *cb, char elem)
{
	uint16_t end = (cb->Beg + cb->Count) % BUFFER_SIZE;
	cb->elements[end] = elem;
	if(!cbIsFull(cb)) ++cb->Count; else cb->Beg = (cb->Beg + 1) % BUFFER_SIZE;
	return true;
}

char cbRead(CircBuffer *cb)
{
	char elem;
	if(cbIsEmpty(cb)) return 0;
	__disable_irq();
	elem = cb->elements[cb->Beg];
	cb->elements[cb->Beg] = 0;
	cb->Beg = (cb->Beg + 1 ) % BUFFER_SIZE;
	--cb->Count;
	__enable_irq();
	return elem;
}
