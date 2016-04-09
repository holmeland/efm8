/*
 * uart0.c
 *
 *  Created on: 8 apr 2016
 *      Author: John
 */
#include "uart0.h"
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile u8 uart0_rx_buffer[UART0_RX_BUFFERSIZE];
volatile u8 uart0_tx_buffer[UART0_TX_BUFFERSIZE];
volatile u8 uart0_rx_head; /* where the UART writes to */
volatile u8 uart0_rx_tail; /* where the application reads from */
volatile u8 uart0_rx_unread_bytes; /* Amount of unread bytes in the array */
volatile u8 uart0_tx_head; /* where the application writes to */
volatile u8 uart0_tx_tail; /* where the UART reads from */
volatile u8 uart0_tx_free_space = UART0_TX_BUFFERSIZE; /* Amount of vacant elements in the array */

/* Add byte array to the UART transmit buffer, size is amount of bytes.
 * \Return true if the number of bytes was put in the buffer, otherwise false.
 *  */
bool uart0_transmit(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(uart0_tx_free_space >= size) {
      for(counter = 0; counter < size; counter++) {
         uart0_tx_buffer[uart0_tx_head] = buffer[counter];
         uart0_tx_head++;
         uart0_tx_head %= UART0_TX_BUFFERSIZE;
      }
      uart0_tx_free_space -= size;
      result = true;
      SCON0_TI = 1; /* force transmit interrupt */
   }

   exitCriticalSection();

   return result;
}

/* Reads size amount of bytes from the UART receive buffer and copies it to the supplied buffer
 * \Return true if bytes were put in the buffer else false.
 * */
bool uart0_receive(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(uart0_rx_unread_bytes >= size) {
      for(counter = 0; counter < size; counter++) {
         buffer[counter] = uart0_rx_buffer[uart0_rx_tail];
         uart0_rx_tail++;
         uart0_rx_tail %= UART0_RX_BUFFERSIZE;
      }
      result = true;
      uart0_rx_unread_bytes -= size;
   }

   exitCriticalSection();

   return result;
}
