/*
 * interrupts.c
 *
 *  Created on: 11 sep 2015
 *      Author: John
 */

#include "johns_defs.h"
#include "interrupts.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define UART_RX_BUFFERSIZE        10
#define UART_TX_BUFFERSIZE        10

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
static volatile u8 uart_rx_buffer[UART_RX_BUFFERSIZE];
static volatile u8 uart_tx_buffer[UART_TX_BUFFERSIZE];
static volatile u8 rx_head; /* where the UART writes to */
static volatile u8 rx_tail; /* where the application reads from */
static volatile u8 rx_unread_bytes; /* Amount of unread bytes in the array */
static volatile u8 tx_head; /* where the application writes to */
static volatile u8 tx_tail; /* where the UART reads from */
static volatile u8 tx_free_space = UART_TX_BUFFERSIZE; /* Amount of vacant elements in the array */

/* Button 1 has been pressed
 * Star a polling process of the button
 * */
SI_INTERRUPT (INT0_ISR, INT0_IRQn)
{
}

/* Button 2 has been pressed
 * Start a polling process of the button
 * */
SI_INTERRUPT (INT1_ISR, INT1_IRQn)
{
}
//-----------------------------------------------------------------------------
// UART0_ISR
//-----------------------------------------------------------------------------
//
// UART0 ISR Content goes here. Remember to clear flag bits:
// SCON0::RI (Receive Interrupt Flag)
// SCON0::TI (Transmit Interrupt Flag)
//
// This routine is invoked whenever a character is received or has been transmitted.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (UART0_ISR, UART0_IRQn)
{
   if(SCON0_RI == 1)    /* receive interrupt */
   {
      SCON0_RI = 0;      /* Clear interrupt flag */
      uart_rx_buffer[rx_head] = SBUF0;
      rx_head++;
      rx_head &= UART_RX_BUFFERSIZE;
      rx_unread_bytes++;
   }
   if(SCON0_TI == 1)     /* transmit interrupt */
   {
      SCON0_TI = 0;      /* Clear interrupt flag */
      if(tx_free_space < UART_TX_BUFFERSIZE) { /* More bytes to transmit */
         SBUF0 = uart_tx_buffer[tx_tail];
         tx_tail++;
         tx_tail %= UART_TX_BUFFERSIZE;
         tx_free_space++;
      }
   }
}

/* Add byte array to the UART transmit buffer, size is amount of bytes.
 * \Return true if the number of bytes was put in the buffer, otherwise false.
 *  */
bool uart_transmit(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(tx_free_space >= size) {
      for(counter = 0; counter < size; counter++) {
         uart_tx_buffer[tx_head] = buffer[counter];
         tx_head++;
         tx_head %= UART_TX_BUFFERSIZE;
      }
      tx_free_space -= size;
      result = true;
      SCON0_TI = 1; /* force transmit interrupt */
   }

   exitCriticalSection();

   return result;
}

/* Reads size amount of bytes from the UART receive buffer and copies it to the supplied buffer
 * \Return true if bytes were put in the buffer else false.
 * */
bool uart_receive(u8 *buffer, u8 size)
{
   bool result = false;
   u8 counter;

   enterCriticalSection();

   if(rx_unread_bytes >= size) {
      for(counter = 0; counter < size; counter++) {
         buffer[counter] = uart_rx_buffer[rx_tail];
         rx_tail++;
         rx_tail %= UART_RX_BUFFERSIZE;
      }
      result = true;
      rx_unread_bytes -= size;
   }

   exitCriticalSection();

   return result;
}
