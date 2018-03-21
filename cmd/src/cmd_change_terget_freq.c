#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"

static void cmd_main(uint8_t *buf)
{
    test_para.freq_test_para.target_freq = (int32_t)((buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3]);
}

char cmd_change_target_freq_init(void)
{
    return add_cmd(CMD_CHANGE_TARGET_FREQ, cmd_main);
}