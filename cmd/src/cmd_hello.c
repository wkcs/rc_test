#include "cmd_list.h"
#include "cmd.h"
#include "rc_debug.h"

static void cmd_main(uint8_t len, uint8_t *buf)
{
    rc_printf("Hello World\r\n");
    return;
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_hello = {
    CMD_HELLO,
    cmd_main
};

