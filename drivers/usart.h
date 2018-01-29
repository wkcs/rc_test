#ifndef __USART_H
#define __USART_H
#include "stm32f4xx_conf.h"
#include "sys.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

typedef struct __uart_data
{
	uint8_t rx_buf[257];
	uint8_t rx_status;
	uint32_t rx_len;
}uart_data_t;

void uart1_init(u32 bound);
void uart3_init(u32 bound);

extern uart_data_t uart1_data, uart3_data;

#endif


