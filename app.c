#include "app.h" 
#include "portBInterrupts.h"
#include "communication.h"


void conversion(void);
/*
*------------------------------------------------------------------------------
* Public Variables
* Buffer[0] = seconds, Buffer[1] = minutes, Buffer[2] = Hour,
* Buffer[3] = day, Buffer[4] = date, Buffer[5] = month, Buffer[6] = year
*------------------------------------------------------------------------------
*/

UINT8 displayBuffer[8] = {0};
UINT8 readTimeDateBuffer[7] = {0};
UINT8 writeTimeDateBuffer[] = {0X01, 0X39, 0X13, 0X03, 0x07, 0X01, 0X14};
//UINT8 readTimeDateBuffer[7] = {0X01, 0X42, 0X10, 0X03, 0x22, 0X01, 0X14};

extern BOOL portB_intFlag;
/*
*------------------------------------------------------------------------------
* app - the app structure. 
*------------------------------------------------------------------------------
*/

typedef struct _APP
{
	BOOL dotOn;
}APP;

#pragma idata app_data
APP app = {0};

UINT8 txBuffer[6] = {0};
#pragma idata

/*
*------------------------------------------------------------------------------
* void APP_init(void)
*
* Summary	: Initialize application
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

void APP_init(void)
{
	//writeTimeDateBuffer[2] = SetHourMode(0X06,1,1);
	//Set Date and Time

	WriteRtcTimeAndDate(writeTimeDateBuffer);	
	TEST_LAMP = 0;
}

/*
*------------------------------------------------------------------------------
* void APP_task(void)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

void APP_task(void)
{

	int i, j;

	ReadRtcTimeAndDate(readTimeDateBuffer);  //Read the data from RTC
	conversion(); // Separate the higher and lower nibble and store it into the display buffer 
	
	
	if( app.dotOn == 0 )
	{
		DigitDisplay_updateBuffer(displayBuffer);
		app.dotOn = 1;
	}
	else 
	{
		DigitDisplay_updateBuffer(displayBuffer);
		DigitDisplay_DotOn(1, 2);
		app.dotOn = 0;	
	
	}

	ENTER_CRITICAL_SECTION();  //turn OFF all interrupts
	if(portB_intFlag == 1)				// when PB is pressed
	{	
		EXIT_CRITICAL_SECTION();   //turn ON all interrupts	
			
		for( j = 0, i = 0; i < 7; i++, j++)			
		{
			if(j == 3)
				j++;

			txBuffer[i] = readTimeDateBuffer[6 - j];  //store time and date 
		}
		
		COM_txBuffer(txBuffer, 6);

		ENTER_CRITICAL_SECTION();  //turn OFF all interrupts
		portB_intFlag = 0;
	
	}
	EXIT_CRITICAL_SECTION();   //turn ON all interrupts
}


void conversion(void)
{

	displayBuffer[0] = (readTimeDateBuffer[1] & 0X0F) + '0';        //Minute LSB
	displayBuffer[1] = ((readTimeDateBuffer[1] & 0XF0) >> 4) + '0'; //Minute MSB
	displayBuffer[2] = (readTimeDateBuffer[2] & 0X0F) + '0';		//Hour LSB
	displayBuffer[3] = ((readTimeDateBuffer[2] & 0XF0) >> 4) + '0'; //Hour MSB
	displayBuffer[4] = (readTimeDateBuffer[5] & 0X0F) + '0';  		//month LSB
	displayBuffer[5] = ((readTimeDateBuffer[5] & 0XF0) >> 4) + '0';	//Month MSB
	displayBuffer[6] = (readTimeDateBuffer[4] & 0X0F) + '0';		//Date LSB
	displayBuffer[7] = ((readTimeDateBuffer[4] & 0XF0) >> 4) + '0';	//Date MSB
}


UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket)
{
	UINT8 i, j;
	UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;

	switch( rxCode)
	{
		case CMD_GET_STATUS:
		break;

		default: break;
	}
	return length;


}