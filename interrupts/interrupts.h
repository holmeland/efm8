/*
 * interrupts.h
 *
 *  Created on: 31 okt 2015
 *      Author: John
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

extern bool uart_transmit(u8 *buffer, u8 size);
extern bool uart_receive(u8 *buffer, u8 size);

#endif /* INTERRUPTS_H_ */
