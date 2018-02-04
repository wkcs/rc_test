#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"

void adc_init(void);
u16 get_adc(u8 ch);
u16 get_adc_average(u8 ch, u8 times);
uint16_t get_socket_voltage(uint8_t num);
uint16_t get_socket_voltage_one(void);
uint16_t get_socket_current(uint8_t num);
uint16_t get_socket_current_one(void);

#endif

