#ifndef __DAC_H__
#define __DAC_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif 

void dac1_init(void);

/*
 *设置rc_power_con的电压值，以控制rc_power电压
 */
void rc_power_con_dac(uint16_t value);

#ifdef __cplusplus
}
#endif 

#endif