#include "portBInterrupts.h"

#pragma udata interruptData
BOOL portB_intFlag;
UINT8 portB_currentData;
#pragma udata

void portBInterruptInit (void)
{
	OpenPORTB( PORTB_CHANGE_INT_ON & PORTB_PULLUPS_ON 
				& PORTB_INT_PRIO_HIGH ); // PortB 'change on interrupt' is enabled

	INTCONbits.RBIF = 0; // clear interrupt flag
	TRISB = 0XFF;		// Port B as input
	portB_intFlag = 0;
	portB_currentData = PORTB;
}

#pragma code 
#pragma interrupt portBInterruptHandler
void portBInterruptHandler (void)
{
	portB_intFlag = 1;
	portB_currentData = PORTB;		//End the mismatch condition by reading PORTB value
	
	Delay10us(1); 		//Give 1 Tcy delay 
	INTCONbits.RBIF = 0; // clear interrupt flag 
}