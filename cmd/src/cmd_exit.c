#include "test_signal.h"
#include "cmd_list.h"
#include "cmd.h"

static void cmd_main(uint8_t *buf)
{
    exit_test();
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_exit = {
    CMD_EXIT,
    cmd_main
};

