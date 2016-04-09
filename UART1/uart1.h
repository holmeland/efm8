/*
 * uart1.h
 *
 *  Created on: 8 apr 2016
 *      Author: John
 */

#ifndef UART1_H_
#define UART1_H_

#include "johns_defs.h"

#define UART1_RX_BUFFERSIZE        10
#define UART1_TX_BUFFERSIZE        10

extern volatile u8 uart1_rx_buffer[UART1_RX_BUFFERSIZE];
extern volatile u8 uart1_tx_buffer[UART1_TX_BUFFERSIZE];
extern volatile u8 uart1_rx_head; /* where the UART writes to */
extern volatile u8 uart1_rx_tail; /* where the application reads from */
extern volatile u8 uart1_rx_unread_bytes; /* Amount of unread bytes in the array */
extern volatile u8 uart1_tx_head; /* where the application writes to */
extern volatile u8 uart1_tx_tail; /* where the UART reads from */
extern volatile u8 uart1_tx_free_space; /* Amount of vacant elements in the array */

extern void uart1_setup(void);
extern bool uart1_transmit(u8 *buffer, u8 size);
extern bool uart1_receive(u8 *buffer, u8 size);

#endif /* UART1_H_ */
