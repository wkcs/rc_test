#ifndef __TEST_BIN_H__
#define __TEST_BIN_H__

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t get_bin_from_err(int32_t err);
int32_t get_bin_from_freq(uint8_t *bin);

#ifdef __cplusplus
}
#endif

#endif