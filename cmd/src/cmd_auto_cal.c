#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "test_cal.h"
#include "freq.h"
#include "rc_debug.h"
#include "rc_message.h"
#include "test_signal.h"
#include "rc_err.h"
#include "rc_math.h"

static void cmd_main(uint8_t *buf)
{
    uint16_t data;
    int32_t freq, err;
    uint8_t mes_buf[4];
	err = auto_cal(test_para.cal_para.auto_cal_mode, &data, test_save.freq_save.start_freq);
    if (err < 0) {
        switch (RC_ABS(err)) {
        case NO_FREQ: rc_send_message(0, 0, NO_FREQ_MES); break;
        case TARGET_FREQ_TOO_HIGH: mes_buf[0] = 0; rc_send_message(mes_buf, 1, TARGET_FREQ_ERR_MES); break;
        case TARGET_FREQ_TOO_LOW: mes_buf[0] = 1; rc_send_message(mes_buf, 1, TARGET_FREQ_ERR_MES); break;
        }
        return;
    }
    freq = cal_one(test_para.cal_para.auto_cal_mode, data, test_para.cal_para.start_cal_test_num);
    if (freq < 0)
        rc_send_message(0, 0, NO_FREQ_MES);
    else
    {
        mes_buf[0] = (uint8_t)(data >> 8);
        mes_buf[1] = (uint8_t)data;
        rc_send_message(mes_buf, 2, CODE_MES);

        mes_buf[0] = (uint8_t)(freq >> 24);
        mes_buf[1] = (uint8_t)(freq >> 16);
        mes_buf[2] = (uint8_t)(freq >> 8);
        mes_buf[3] = (uint8_t)freq;
        rc_send_message(mes_buf, 4, FREQ_MES);
    }
}

char cmd_auto_cal_init(void)
{
    return add_cmd(CMD_AUTO_CAL, cmd_main);
}