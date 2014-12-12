#include "board.h"
#include "i2c_driver.h"



#define RTC_SLAVE_ADD	(0XD0)
#define RTC_SLAVE_READ	(0XD1)
#define RTC_SLAVE_WRITE	(0XD0)



typedef enum _RTC_REG_ADD
{
	RTC_ADD_SECONDS = 0X00,
	RTC_ADD_MINUTES,
	RTC_ADD_HOURS,
	
	RTC_ADD_DAY,
	RTC_ADD_DATE,
	RTC_ADD_MONTH,
	RTC_ADD_YEAR
}RTC_REG_ADD;

/*
typedef enum _RTC_ACK
{
	ADD_ACK = 0,
	SEC_ACK,
	MIN_ACK,
	HRS_ACK,
	DAY_ACK,
	DATE_ACK,
	MONTH_ACK,
	YEAR_ACK
}RTC_ACK;


typedef enum _I2C_INIT_DATA
{
	SYNC_MODE = MASTER,
	SLEW	  = SLEW_OFF
}I2C_INIT_DATA; 
*/

void rtc_init (void);

UINT8 rtc_getSeconds (void);
UINT8 rtc_getMinutes (void);
UINT8 rtc_getHours   (void);

UINT8 rtc_getDay		(void);
UINT8 rtc_getDate	(void);
UINT8 rtc_getMonth	(void);
UINT8 rtc_getYear    (void);
