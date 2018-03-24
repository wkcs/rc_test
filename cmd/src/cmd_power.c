#include "power.h"
#include "cmd_list.h"
#include "cmd.h"
#include "test_type.h"

#define CMD_POWER_OFF 0
#define CMD_POWER_RESTART 1

static void cmd_main(uint8_t len, uint8_t *buf)
{
    if (buf[0] == CMD_POWER_OFF) 
        power_on(test_para.power_para.clos_time);
    else if (buf[0] == CMD_POWER_RESTART)
        power_restart(test_para.power_para.restart_time);
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_power = {
    CMD_POWER,
    cmd_main
};

