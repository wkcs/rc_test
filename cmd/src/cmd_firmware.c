#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"

static void send_firmware(void)
{
    uint8_t tx_buf[32];

    tx_buf[0] = 255;
    tx_buf[1] = 30;
    tx_buf[2] = VERSION_1;
    tx_buf[3] = VERSION_2;
    tx_buf[4] = VERSION_3;

    send_data_to_uart1(tx_buf, tx_buf[1] + 2);
}

static void cmd_main(uint8_t *buf)
{
    send_firmware();
}

char cmd_firmware_init(void)
{
    return add_cmd(CMD_FIRMWARE, cmd_main);
}

