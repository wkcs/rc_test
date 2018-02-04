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
  CMD_FIRMWARE = 255
}cmd_num_list;

char cmd_test_para_init(void);
char cmd_auto_efuse_init(void);
char cmd_firmware_init(void);

#ifdef __cplusplus
}
#endif

#endif

