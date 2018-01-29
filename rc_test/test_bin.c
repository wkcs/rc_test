#include "test_bin.h"
#include "rc_math.h"
#include "test_type.h"
#include "rc_err.h"
#include "test_cal.h"

uint8_t get_bin_from_err(int32_t err)
{
    return test_para.sub_bin_para.err_bin[RC_ABS(err) - 1];
}

uint8_t get_bin_from_freq(void)
{
    uint8_t i;
    int32_t freq;

    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();

    for (i = 0; i < 3; i++) {
		freq = cal_one(0, 0, test_para.cal_para.bin_cal_test_num);
		if(freq > 0) {
			test_save.freq_save.end_freq = freq / (i + 1) + test_save.freq_save.end_freq * i / (i + 1);
		} else
			return freq;
	}

    test_save.freq_save.freq_err =test_save.freq_save.end_freq - test_save.freq_save.target_freq;

    for (i = 0; i < 8; i++)
    {
        if ((test_para.sub_bin_para.bin_en >> i) & 0x01)
        {
            if ((RC_ABS(test_save.freq_save.freq_err) < test_para.sub_bin_para.bin_freq_err_max[i]) || (test_para.sub_bin_para.bin_freq_err_max[i] == 0))
                return (i + 1);
        }
    }
    return 0;
}

