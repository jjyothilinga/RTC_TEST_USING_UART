
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/






/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "app.h"
#include "rtc_driver.h"
#include "i2c_driver.h"
#include "digitdisplay.h"
#include "communication.h"
#include "uart_driver.h"
#include "portBInterrupts.h"

/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/

#pragma config OSC      = INTIO67
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOREN    = ON
#pragma config BORV     = 3
#pragma config WDT      = OFF
#pragma config WDTPS    = 512	
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT  = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
//#pragma config WRT3   = OFF
#pragma config WRTB     = OFF//N       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF
/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/
extern UINT16 heartBeatCount;
extern UINT16 keypadUpdate_count;
extern UINT16 comUpdateCount;

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/
UINT8 message[]="IDEONICS";

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
const UINT8 LED_MAP[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
#define DIGIT_REFRESH_PERIOD	(65535 - 5000)
#define TICK_PERIOD	(65535 - 10000)




void main(void)
{
	UINT8 count=0, i, uartData;
	UINT8 blink = 0;

	BRD_init();			//board initialization

	InitializeRtc();	//RTC Initialization

	DigitDisplay_init(8); //Digit Display initialization

	TMR0_init(TICK_PERIOD,0);		//initialize timer0
	TMR1_init(DIGIT_REFRESH_PERIOD,DigitDisplay_task);	// Timer1 initialization						//initialize timer1

	APP_init();
	EnableInterrupts();		//Interrupts initialization
	portBInterruptInit(); 

	COM_init(CMD_SOP,CMD_EOP,RESP_SOP,RESP_EOP,APP_comCallBack);
	
	ENABLE_GLOBAL_INT();

	for( i = 0; i <  26; i++)
	{
		uartData = 'A'+i;
		UART_write(uartData);
		UART_transmit();
	}


	while(1)
	{

		COM_task();

	`	if(heartBeatCount >= 250 )
		{	
			APP_task();
			HB_task();
			heartBeatCount = 0;
		}

		
	}


}

/*
*  End of main.c
*/
