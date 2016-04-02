/*
 * johns_defs.h
 *
 *  Created on: 30 aug 2015
 *      Author: John
 */

#ifndef JOHNS_DEFS_H_
#define JOHNS_DEFS_H_

#include <SI_EFM8UB1_Register_Enums.h>                  // SI_SFR declarations

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define u8     uint8_t
#define u16    uint16_t
#define u32    uint32_t

#define enterCriticalSection() (IE &= ~BIT7)
#define exitCriticalSection() (IE |= BIT7)

#endif /* JOHNS_DEFS_H_ */
