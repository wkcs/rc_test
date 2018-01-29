#ifndef __TEST_EFUSE__
#define __TEST_EFUSE__

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t efuse_chip(uint8_t mode, uint16_t data, uint16_t test_num);

#ifdef __cplusplus
}
#endif

#endif