/*
 * stepper.c
 *
 *  Created on: 8 sep 2015
 *      Author: John
 */
#include "stepper.h"

#define TIMER0_COUNTS_PER_PERIOD 48000 /*  */
#define TIMER0_COUNTS_TO_OVERFLOW 0xFFFFu /* 16-bit counter */
#define TIMER0_RELOAD_VALUE (TIMER0_COUNTS_TO_OVERFLOW - TIMER0_COUNTS_PER_PERIOD)
#define TIMER0_RELOAD_VALUE_MSB (TIMER0_RELOAD_VALUE >> 8)
#define TIMER0_RELOAD_VALUE_LSB (TIMER0_RELOAD_VALUE & 0xFFu)

#define A_HIGH (P0 |= BIT3)
#define A_LOW  (P0 &= ~BIT3)
#define B_HIGH (P0 |= BIT6)
#define B_LOW  (P0 &= ~BIT6)
#define C_HIGH (P0 |= BIT7)
#define C_LOW  (P0 &= ~BIT7)
#define D_HIGH (P1 |= BIT0)
#define D_LOW  (P1 &= ~BIT0)
/* Private variables */
static volatile u32 priv_actual_steps;
static volatile u32 priv_target_steps;
static volatile bool priv_target_reached = true;
static volatile u32 priv_direction;

/* In: direction = [0 = reverse, 1 = forward] */
void stepper_setDirection(u8 direction)
{
   enterCriticalSection();

   priv_direction = direction;

   exitCriticalSection();
}
/* In: target = Absolute amount of steps that the driver will try to perform from step position 0 */
void stepper_setTarget(u32 target)
{
   u8 enable_timer0_int;

   enterCriticalSection();

   if(target != priv_actual_steps) { /* Start stepper motor actions */
      enable_timer0_int = BIT1;
      /* Start timer */
      TCON |= BIT4;
      if(target > priv_actual_steps) {
         priv_direction = 1; /* forward */
      }
      else {
         priv_direction = 0; /* reverse */
      }
   }
   else {
      enable_timer0_int = 0;
      priv_target_reached = true;
   }
   priv_target_steps = target;

   priv_target_reached = false;

   exitCriticalSection();

   IE |= enable_timer0_int; /* Enable/Disable timer0 interrupt */
}
/* Out: Actual steps taken */
u32 stepper_getActualSteps(void)
{
   u32 return_value;

   enterCriticalSection();

   return_value = priv_actual_steps;

   exitCriticalSection();

   return return_value;
}
/* Out: If stepper motor has reached the target */
bool stepper_isTargetReached(void)
{
   bool return_value;

   enterCriticalSection();

   return_value = priv_target_reached;

   exitCriticalSection();

   return return_value;
}

/* Timer0:
 * - 16-bit timer
 * - T0H high 8 LSB of counter
 * - T0L low 8 LSBs of counter
 * - 48 MHz clock
 * - Interrupt on timer overflow = 0x1FFF
 * Output states are updated and the stepper will take one
 * step. The stepper motor coil is energized with low output
 *
 *          Stepper pin assignment for stepper motor model 28YBJ-48:
 *
 *          A = Blue   = P0.3
 *          B = Pink   = P0.6
 *          C = Yellow = P0.7
 *          D = Orange = P1.0
 * */
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{
   static u8 internal_phase;

   TCON &= ~BIT4;
   TL0 = TIMER0_RELOAD_VALUE_LSB; /* Determines the frequency */
   TH0 = TIMER0_RELOAD_VALUE_MSB;
   TCON |= BIT4;

   /* Update outputs depending on:
    * - remaining amount of steps
    * - direction */
   switch(internal_phase) /* 8 steps for full cycle */
   {
   case 0: /* Low = A; High = B,C,D */
      A_LOW;
      B_HIGH;
      C_HIGH;
      D_HIGH;
      break;
   case 1: /* Low = A,B; High = C,D */
      A_LOW;
      B_LOW;
      C_HIGH;
      D_HIGH;
      break;
   case 2: /* Low = B; High = A,C,D */
      A_HIGH;
      B_LOW;
      C_HIGH;
      D_HIGH;
      break;
   case 3: /* Low = B,C; High = A,D */
      A_HIGH;
      B_LOW;
      C_LOW;
      D_HIGH;
      break;
   case 4: /* Low = C; High = A,B,D */
      A_HIGH;
      B_HIGH;
      C_LOW;
      D_HIGH;
      break;
   case 5: /* Low = C,D; High = A,B */
      A_HIGH;
      B_HIGH;
      C_LOW;
      D_LOW;
      break;
   case 6: /* Low = D; High = A,B,C */
      A_HIGH;
      B_HIGH;
      C_HIGH;
      D_LOW;
      break;
   case 7: /* Low = A,D; High = B,C */
      A_LOW;
      B_HIGH;
      C_HIGH;
      D_LOW;
      break;
   default: /* Should not happen */
      break;
   }
   /* Update priv_steps_taken, priv_steps_remaining priv_target_reached */
   if(priv_direction == 0) { /* reverse */
      internal_phase--;
      /* TODO: connect steps taken to stall detection using ADC connected to P0.2 */
      priv_actual_steps--; /* decrement steps taken */
   }
   else {                    /* forward */
      internal_phase++;
      /* TODO: connect steps taken to stall detection using ADC connected to P0.2 */
      priv_actual_steps++; /* increment steps taken */
   }

   if(internal_phase == 8) {
      internal_phase = 0; /* forward wrap around */
   }
   else if (internal_phase > 8) {
      internal_phase = 7; /* reverse wrap around */
   }

   if(priv_actual_steps == priv_target_steps) { /* target has been reached */
      priv_target_reached = true;
      IE &= ~(BIT1); /* Disable timer0 interrupt */
      A_LOW;
      B_LOW;
      C_LOW;
      D_LOW;
   }
   else {
      priv_target_reached = false;
   }
}
