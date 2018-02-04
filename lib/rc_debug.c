#include "rc_debug.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

/*通过上位机的调试界面打印*/
void rc_printf(char* fmt,...)  
{  
	__VALIST ap;
	char buf[258];
	va_start(ap,fmt);
	vsprintf((char*)(buf + 2),fmt,ap);
	va_end(ap);
	buf[1] = strlen((const char*)(buf + 2));//此次发送数据的长度
	buf[0] = 254;
	send_data_to_uart1((uint8_t *)buf, buf[1] + 2);
} 

