#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "rc_message.h"

static void cmd_main(uint8_t len, uint8_t *buf)
{
    uint8_t tx_buf[4];

    tx_buf[0] = VERSION_1;
    tx_buf[1] = VERSION_2;
    tx_buf[2] = VERSION_3;
    tx_buf[3] = 0;        

    rc_send_message(tx_buf, 4, FIRMWARE_MES);
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_firmware = {
    CMD_FIRMWARE,
    cmd_main
};


