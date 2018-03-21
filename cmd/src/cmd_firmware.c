#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "rc_message.h"

static void send_firmware(void)
{
    uint8_t tx_buf[4];

    tx_buf[0] = VERSION_1;
    tx_buf[1] = VERSION_2;
    tx_buf[2] = VERSION_3;
    tx_buf[3] = 0;         //非增强版

    rc_send_message(tx_buf, 3, FIRMWARE_MES);
}

static void cmd_main(uint8_t *buf)
{
    send_firmware();
}

char cmd_firmware_init(void)
{
    return add_cmd(CMD_FIRMWARE, cmd_main);
}

