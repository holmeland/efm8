/*
 * uart1.c
 *
 *  Created on: 8 apr 2016
 *      Author: John
 */

#include "uart1.h"

volatile u8 uart1_rx_buffer[UART1_RX_BUFFERSIZE];
volatile u8 uart1_tx_buffer[UART1_TX_BUFFERSIZE];
volatile u8 uart1_rx_head; /* where the UART writes to */
volatile u8 uart1_rx_tail; /* where the application reads from */
volatile u8 uart1_rx_unread_bytes; /* Amount of unread bytes in the array */
volatile u8 uart1_tx_head; /* where the application writes to */
volatile u8 uart1_tx_tail; /* where the UART reads from */
volatile u8 uart1_tx_free_space; /* Amount of vacant elements in the array */

void uart1_setup(void)
{
   u8 page_save = SFRPAGE;
   SFRPAGE = 0x20;
   SBCON1 = BIT6 | 0x3; /* Prescale 1, enable baud rate generator */
   SBRL1 = 63036; /* 9600 Baud, 48 MHz clock */
   SCON1 = BIT4; /* Receiver enable */

   SFRPAGE = 0x10;
   EIE2 |= BIT2; /* UART1 interrupt enable */

   SFRPAGE = page_save;
}

bool uart1_transmit(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(uart1_tx_free_space >= size) {
      u8 page_save = SFRPAGE;
      for(counter = 0; counter < size; counter++) {
         uart1_tx_buffer[uart1_tx_head] = buffer[counter];
         uart1_tx_head++;
         uart1_tx_head %= UART1_TX_BUFFERSIZE;
      }
      uart1_tx_free_space -= size;
      result = true;
      SFRPAGE = 0x20;
      SCON1_TI = 1; /* force transmit interrupt */
      SFRPAGE = page_save;
   }

   exitCriticalSection();

   return result;
}
bool uart1_receive(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(uart1_rx_unread_bytes >= size) {
      for(counter = 0; counter < size; counter++) {
         buffer[counter] = uart1_rx_buffer[uart1_rx_tail];
         uart1_rx_tail++;
         uart1_rx_tail %= UART1_RX_BUFFERSIZE;
      }
      result = true;
      uart1_rx_unread_bytes -= size;
   }

   exitCriticalSection();

   return result;
}
