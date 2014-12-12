#include "board.h"
#include "config.h"
#include "typedefs.h"
#include "rtc_driver.h"
#include "digitdisplay.h"
#include "uart_driver.h"


typedef enum
{

	CMD_PING = 0xA0,
	CMD_GET_STATUS = 0x80
};


UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket);
void APP_init(void);
void APP_task(void);

