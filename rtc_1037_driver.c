
/*
*------------------------------------------------------------------------------
* rtc_driver.h
*
* Include file for rtc_driver module.
*
* 
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: rtc_1307_driver.c
* Created by		: 
* Last changed by	: 
* Last changed		: 
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "rtc_1307_driver.h"
#include "i2c_driver.h"

/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/
UINT16 BCD2HEX(UINT8 );
UINT8 HEX2BCD(UINT8 );


typedef struct _RTC_DATA
{
	UINT8 seconds;
	UINT8 minutes;
	UINT8 hours;
	
	UINT8 day;
	UINT8 date;
	UINT8 month;
	UINT8 year;

	UINT8 am_pm;
	UINT8 hours_format;

//	BOOL ackBuffer[RTC_ACK];
}RTC_DATA;

RTC_DATA rtc_data = {0};


/*
*------------------------------------------------------------------------------
* void rtc_init(void)

* Summary	: Setup rtc
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/

void rtc_init (void)
{

	UINT8 tempVar;

	InitializeI2C();
	
	// Read seconds register contents
	tempVar = ReadByteI2C(RTC_SLAVE_READ,RTC_ADD_SECONDS);
	// Enable oscillator (bit 7=0)
	tempVar = tempVar & 0x7F;
	WriteByteI2C(RTC_SLAVE_WRITE,RTC_ADD_SECONDS,tempVar);
	WriteByteI2C(RTC_SLAVE_WRITE,0X07,0x90);

}	

/*
*------------------------------------------------------------------------------
* UINT8 rtc_getMinutes(void)

* Summary	: Setup rtc
*
* Input		: None
*
* Output	: Minutes 
*
*------------------------------------------------------------------------------
*/

UINT8 rtc_getMinutes(void)
{
	StartI2C();
	WriteI2C(RTC_SLAVE_READ);  //Send RTC address and Read to set I2C master to read mode
	WriteI2C(RTC_ADD_MINUTES); //Send address of Minutes Register to read the minutes
	 
	rtc_data.minutes = ReadI2C(ACK); 
	StopI2C();

	rtc_data.minutes = BCD2HEX(rtc_data.minutes); //Convert BCD data of RTC into HEX
	
	return rtc_data.minutes;
}


UINT8 rtc_setMinutes(UINT8 minutes)
{
	rtc_data.minutes = HEX2BCD(minutes);  //Convert HEX data into BCD to write into the RTC
	
	StartI2C();
	WriteI2C(RTC_SLAVE_WRITE); //Send RTC address and Read to set I2C master to write mode
	WriteI2C(RTC_ADD_MINUTES);
	
	WriteI2C(rtc_data.minutes);
	StopI2C();
	
	
	
	
}

/*
*------------------------------------------------------------------------------
* UINT8 rtc_getSeconds(void)

* Summary	: Setup rtc
*
* Input		: None
*
* Output	: Seconds
*
*------------------------------------------------------------------------------
*/

UINT8 rtc_getSeconds(void)
{

	ReadByteI2C(RTC_SLAVE_READ, RTC_ADD_SECONDS);

	rtc_data.seconds = BCD2HEX(rtc_data.seconds); //Convert BCD data of RTC into HEX
	
	return rtc_data.seconds;
}

/*
*------------------------------------------------------------------------------
* UINT16 BCD2HEX(UINT8 bcd)
*
* Summary	: Convert BCD 1 byte to HEX 1 byte
*
* Input		: UINT8 bcd - bcd value
*
* Output	: UINT8 - converted HEX value
*
*------------------------------------------------------------------------------
*/
UINT16 BCD2HEX(UINT8 bcd)
{
	UINT8 hex;	
   	hex = ((bcd >> 4) * 10) + (bcd & 0X0F) ;
   	return hex;
}

/*
*------------------------------------------------------------------------------
* UINT8 HEX2BCD(UINT8 hexIn)
*
* Summary	: Convert 1 b yte HEX to 1 byte BCD
*
* Input		: UINT8 bcd - bcd value
*
* Output	: UINT8 - converted HEX value
*
*------------------------------------------------------------------------------
*/
UINT8 HEX2BCD(UINT8 hexIn)
{
	UINT8 temp1,temp2;
	temp1 = (hexIn / 10);
	temp2 = (hexIn % 10);
	return (((temp1 & 0x0F) << 4 ) | (temp2 & 0x0F));
}
