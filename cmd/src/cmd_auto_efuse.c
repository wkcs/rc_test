#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"

static void cmd_main(uint8_t len, uint8_t *buf)
{
    test_data.test_machine_data.test_start = ENABLE;
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_auto_efuse = {
    CMD_AUTO_EFUSE,
    cmd_main
};



