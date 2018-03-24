#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "rc_message.h"

#define PRO 1

static void cmd_main(uint8_t *buf)
{
    uint8_t tx_buf[8];

    tx_buf[0] = VERSION_1;
    tx_buf[1] = VERSION_2;
    tx_buf[2] = VERSION_3;
    tx_buf[3] = PRO;

    tx_buf[4] = (uint8_t)(test_para.freq_test_para.target_freq >> 24);
    tx_buf[5] = (uint8_t)(test_para.freq_test_para.target_freq >> 16);
    tx_buf[6] = (uint8_t)(test_para.freq_test_para.target_freq >> 8);
    tx_buf[7] = (uint8_t)(test_para.freq_test_para.target_freq);

    rc_send_message(tx_buf, 8, FIRMWARE_MES);
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_firmware = {
    CMD_FIRMWARE,
    cmd_main
};


