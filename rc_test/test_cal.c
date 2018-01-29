#include "test_cal.h"
#include "rc_err.h"
#include "rc_math.h"
#include "freq.h"

int32_t cal_one(uint8_t mode, uint16_t data, uint16_t test_num)
{
    int32_t freq;
    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();
    
    start_sig();
    cal_sig(data);
    freq = get_freq(test_num);
    if(freq <= 0)
    {
        if (mode)
        {
            power_restart(test_para.power_para.restart_time);
            start_sig();
            cal_sig(data);
            freq = get_freq(test_num);
        }
    }
    exit_test();
    return freq;
}

int32_t auto_cal(uint8_t mode, uint16_t *data, int32_t start_freq)
{
    uint8_t i;
	uint16_t data_save;
	int32_t freq_err, freq_err_old, freq;
	*data = 0;
    if (start_freq == 0)
	{
	    freq = cal_one(mode, *data, test_para.cal_para.start_cal_test_num);
		if (freq > 0)
			test_save.freq_save.start_freq = freq;
		else
			return freq;
	}
    else {
        freq = start_freq;
		test_save.freq_save.start_freq = start_freq;
	}
	if(freq > 0)
	{
		if (test_para.freq_test_para.target_freq < freq)
		{
			return -TARGET_FREQ_TOO_LOW;
		} else
		{
			for (i = 0; i < 10; i++) {
				data_save = *data;
				*data |= 0x0001 << (9 - i);
				freq = cal_one(mode, *data, test_para.cal_para.freq_cal_test_num[i]);
				if (freq > 0) {
					if (freq > test_para.freq_test_para.target_freq)
						*data = data_save;
					if (i == 9) {
						if (freq < test_para.freq_test_para.target_freq && (*data & 0x3ff) == 0x3ff) {
							return -TARGET_FREQ_TOO_HIGH;
						}
					}
				} else
					return freq;
			}
			freq = cal_one(mode, *data, test_para.cal_para.bin_cal_test_num);
			if(freq > 0) {
				freq_err_old = freq - test_para.freq_test_para.target_freq;
			} else
				return freq;
			(*data)++;
			freq = cal_one(mode, *data, test_para.cal_para.bin_cal_test_num);
			if(freq > 0) {
				freq_err = freq - test_para.freq_test_para.target_freq;
			} else
				return freq;

			if (RC_ABS(freq_err) > RC_ABS(freq_err_old))
				(*data)--;

			test_save.freq_save.code = *data;
            return 0;
		}
	} else
		return freq;
}