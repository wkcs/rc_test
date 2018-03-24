#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "freq.h"
#include "rc_debug.h"
#include "rc_message.h"
#include "test_signal.h"

static void cmd_main(uint8_t *buf)
{
    uint16_t data;
    int32_t freq;
    uint8_t mes_buf[4];

	power_restart(test_para.power_para.restart_time);
	data = (buf[0] << 8) + buf[1];
	start_sig();
	efuse_sig(data);
    freq = get_freq(test_para.efuse_para.efuse_test_num);
    if (freq < 0) {
		power_restart(test_para.power_para.restart_time);
	    start_sig();
	    efuse_sig(data);
        freq = get_freq(test_para.efuse_para.efuse_test_num);
		if (freq < 0)
			rc_send_message(0, 0, NO_FREQ_MES);
	}

    mes_buf[0] = (uint8_t)(freq >> 24);
    mes_buf[1] = (uint8_t)(freq >> 16);
    mes_buf[2] = (uint8_t)(freq >> 8);
    mes_buf[3] = (uint8_t)freq;
    rc_send_message(mes_buf, 4, FREQ_MES);
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_efuse = {
    CMD_EFUSE,
    cmd_main
};


