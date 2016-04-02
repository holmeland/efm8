/*
 * stepper.h
 *
 *  Created on: 31 aug 2015
 *      Author: John
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "johns_defs.h"

/* Public functions */

extern void stepper_setup(void);
extern void stepper_setDirection(uint8_t direction);
extern void stepper_setTarget(uint32_t target);
extern uint32_t stepper_getActualSteps(void);
extern bool stepper_isTargetReached(void);


#endif /* STEPPER_H_ */
