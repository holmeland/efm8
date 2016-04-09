/*
 * uart0.h
 *
 *  Created on: 8 apr 2016
 *      Author: John
 */
#ifndef UART0_H_
#define UART0_H_

#include "johns_defs.h"

#define UART0_RX_BUFFERSIZE        10
#define UART0_TX_BUFFERSIZE        10

extern volatile u8 uart0_rx_buffer[UART0_RX_BUFFERSIZE];
extern volatile u8 uart0_tx_buffer[UART0_TX_BUFFERSIZE];
extern volatile u8 uart0_rx_head; /* where the UART writes to */
extern volatile u8 uart0_rx_tail; /* where the application reads from */
extern volatile u8 uart0_rx_unread_bytes; /* Amount of unread bytes in the array */
extern volatile u8 uart0_tx_head; /* where the application writes to */
extern volatile u8 uart0_tx_tail; /* where the UART reads from */
extern volatile u8 uart0_tx_free_space; /* Amount of vacant elements in the array */

extern bool uart0_transmit(u8 *buffer, u8 size);
extern bool uart0_receive(u8 *buffer, u8 size);

#endif /* UART0_H_ */
