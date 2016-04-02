/*
 * setup.c
 *
 *  Created on: 30 mar 2016
 *      Author: John
 */
#include "main.h"

void setup(void);
static void disable_watchdog(void);
static void config_clocks(void);
static void config_timer0(void);
static void config_pins(void);

void setup(void)
{
   disable_watchdog();
   config_clocks();
   config_timer0();
   config_pins();
}

void disable_watchdog(void)
{
  WDTCN = 0xDE;
  WDTCN = 0xAD;
}

void config_clocks(void)
{
   SFRPAGE = 0x10;
   PFE0CN = BIT4 | BIT5; /* Enable Prefetch engine */
   HFOCN  = BIT7;  /* Enable 48 MHz */
   CLKSEL = BIT0 | BIT1 | BIT4; /* Switch to 48 MHz clock divided by 2 */
   CLKSEL = BIT0 | BIT1;      /* Now sitch to full 48 MHz */
   while((CLKSEL & BIT7) == 0);
   SFRPAGE = 0x0;
}

void config_timer0(void) /* Timer0 overflow interrupt is used for stepper motor timing */
{
   TMOD |= BIT0;
   TMOD &= ~BIT1; /* Sets timer0 in 16-bit mode */
   CKCON0 |= BIT2; /* Timer0 clock source is Sysclk */
}

void config_pins(void)
{
   P0 = 0;
   P1 = 0;

   P0MDIN = BIT3 | BIT4 | BIT5 | BIT6 | BIT7; /* Sets digital mode */
   P1MDIN = BIT0;

   P0MDOUT = BIT3 | BIT4 | BIT5 | BIT6 | BIT7; /* Sets push-pull */
   P1MDOUT = BIT0;

   P0SKIP = BIT4 | BIT5; /* P0.4 and P0.5 are connected to UART0 */

   XBR0 = BIT0; /* UART0 routed to port pins */
   XBR2 = BIT6 | BIT7; /* XBAR enabled and weak-pull ups disabled */
}
