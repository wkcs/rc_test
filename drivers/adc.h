#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif 

void adc_init(void);
void adc_test_init(void);
uint16_t *get_os_adc(void);
uint16_t *get_work_current_adc(uint16_t num);
uint16_t *get_standby_current_adc(uint16_t num);
uint16_t *get_rc_power_adc(uint16_t num);

#ifdef __cplusplus
}
#endif 

#endif

