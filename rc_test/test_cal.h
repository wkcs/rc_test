#ifndef __TEST_CAL_H__
#define __TEST_CAL_H__

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t cal_one(uint8_t mode, uint16_t data, uint16_t test_num);
int32_t auto_cal(uint8_t mode, uint16_t *data, int32_t start_freq);

#ifdef __cplusplus
}
#endif

#endif