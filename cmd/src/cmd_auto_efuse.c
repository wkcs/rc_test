#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"

static void cmd_main(uint8_t *buf)
{
    test_data.test_machine_data.test_start = ENABLE;
}

char cmd_auto_efuse_init(void)
{
    return add_cmd(CMD_AUTO_EFUSE, cmd_main);
}

