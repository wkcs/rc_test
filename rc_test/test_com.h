#ifndef __TEST_COM_H__
#define __TEST_COM_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void send_data(uint8_t *buf, uint8_t len);
void get_data(void);

#ifdef __cplusplus
}
#endif

#endif