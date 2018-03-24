#ifndef __CMD_H__
#define __CMD_H__

#include "sys.h"

typedef void (*cmd_function) (uint8_t len, uint8_t *buf);

typedef struct {
    uint8_t cmd_id;
    cmd_function cmd_main_function;
}cmd_list_t;

#define CMD_REPETITION (int8_t)-1
#define CMD_NULL (int8_t)-2

int8_t cmd_run(uint8_t cmd_id, uint8_t cmd_len, uint8_t *cmd_data);

#endif

