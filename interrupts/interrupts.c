/*
 * interrupts.c
 *
 *  Created on: 11 sep 2015
 *      Author: John
 */
#include "interrupts.h"

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
#if 0
SI_INTERRUPT (UART0_ISR, UART0_IRQn)
{
   if(SCON0_RI == 1)    /* receive interrupt */
   {
      SCON0_RI = 0;      /* Clear interrupt flag */
      uart0_rx_buffer[uart0_rx_head] = SBUF0;
      uart0_rx_head++;
      uart0_rx_head &= UART0_RX_BUFFERSIZE;
      uart0_rx_unread_bytes++;
   }
   if(SCON0_TI == 1)     /* transmit interrupt */
   {
      SCON0_TI = 0;      /* Clear interrupt flag */
      if(uart0_tx_free_space < UART0_TX_BUFFERSIZE) { /* More bytes to transmit */
         SBUF0 = uart0_tx_buffer[uart0_tx_tail];
         uart0_tx_tail++;
         uart0_tx_tail %= UART0_TX_BUFFERSIZE;
         uart0_tx_free_space++;
      }
   }
}
#endif

SI_INTERRUPT(UART1_ISR, UART1_IRQn)
{
   u8 page_save = SFRPAGE;

   if(SCON1_RI == 1)    /* receive interrupt */
   {
      SCON1_RI = 0;      /* Clear interrupt flag */
      uart1_rx_buffer[uart1_rx_head] = SBUF1;
      uart1_rx_head++;
      uart1_rx_head &= UART1_RX_BUFFERSIZE;
      uart1_rx_unread_bytes++;
   }
   if(SCON1_TI == 1)     /* transmit interrupt */
   {
      SCON1_TI = 0;      /* Clear interrupt flag */
      if(uart1_tx_free_space < UART1_TX_BUFFERSIZE) { /* More bytes to transmit */
         SBUF1 = uart1_tx_buffer[uart1_tx_tail];
         uart1_tx_tail++;
         uart1_tx_tail %= UART1_TX_BUFFERSIZE;
         uart1_tx_free_space++;
      }
   }

   SFRPAGE = page_save;
}

