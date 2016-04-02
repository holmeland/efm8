#include "main.h"

void main (void)
{
   setup();
   IE_EA = 1;

	while(1)
	{
	   u8 rx_buf[4];

	   if(uart_receive(rx_buf, 4)) {
         u32 target = 0;
         target = (rx_buf[0] << 24) | (rx_buf[1] << 16) | (rx_buf[2] << 8) | (rx_buf[3]);
         stepper_setTarget(target);
      }

	   if(stepper_isTargetReached() == true) {
	      if(stepper_getActualSteps() >  99) {
	         stepper_setTarget(0);
	      }
	      else {
	         stepper_setTarget(4096);
	      }
	   }
	}
}
