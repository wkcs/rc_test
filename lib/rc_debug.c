#include "rc_debug.h"
#include "board.h"
#include "usbd_cdc_vcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include "rc_message.h"

/*通过上位机的调试界面打印*/
void rc_printf(char* fmt,...)
{
	__VALIST ap;
	char buf[256];
	va_start(ap,fmt);
	vsprintf((char*)(buf),fmt,ap);
	va_end(ap);
	rc_send_message((uint8_t *)buf, strlen((const char*)buf), RC_DEBUG_MES);
}

