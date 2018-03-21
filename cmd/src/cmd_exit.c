#include "test_signal.h"
#include "cmd_list.h"
#include "cmd.h"

static void cmd_main(uint8_t *buf)
{
    exit_test();
}

char cmd_exit_init(void)
{
    return add_cmd(CMD_EXIT, cmd_main);
}