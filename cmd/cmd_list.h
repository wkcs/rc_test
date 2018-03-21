#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

typedef enum
{
  CMD_TEST_PARA = 0,
  CMD_AUTO_EFUSE = 1,
  CMD_CAL,
  CMD_EFUSE,
  CMD_POWER = 6,
  CMD_EXIT = 8,
  CMD_CHANGE_TARGET_FREQ = 10,
  CMD_AUTO_CAL,
  CMD_KEY_TEST,
  CMD_FIRMWARE = 255
}cmd_num_list;

char cmd_test_para_init(void);
char cmd_auto_efuse_init(void);
char cmd_firmware_init(void);
char cmd_cal_init(void);
char cmd_efuse_init(void);
char cmd_power_init(void);
char cmd_exit_init(void);
char cmd_change_target_freq_init(void);
char cmd_auto_cal_init(void);

#ifdef __cplusplus
}
#endif

#endif

