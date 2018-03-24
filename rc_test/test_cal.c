#include "test_cal.h"
#include "rc_err.h"
#include "rc_math.h"
#include "freq.h"
#include "rc_debug.h"

//static int32_t freq_err_temp[6];      //频率误差小于100时的误差值
//static uint16_t code_temp[6];         //频率误差小于100时的code
//static uint8_t freq_ok_num ;           //频率误差小于100个数

/*
void get_string_binary(uint32_t data, char *addr, uint8_t len)
{
	uint8_t num;
	for(num = 0; num < len; num++) {
		if ((data >> (len - 1 - num)) & 0x1)
			addr[num] = '1';
		else
			addr[num] = '0';
	}
}*/

/*进行一次修调*/
int32_t cal_one(uint8_t mode, uint16_t data, uint16_t test_num)
{
	int32_t freq;
	if (test_data.power_data.status == POWER_OFF) {
		power_on(test_para.power_para.open_time);
	}
	if (test_data.test_mod != NORMAL_MODE)
		exit_test();
	//power_restart(2000);
	start_sig();
	//delay_us(40);
	cal_sig(data);
	freq = get_freq(test_num);
	if(freq <= 0) {
		if (mode) {
			power_restart(test_para.power_para.restart_time);
			start_sig();
			cal_sig(data);
			freq = get_freq(test_num);
		}
	}
	//power_off(test_para.power_para.clos_time);
	exit_test();
	//delay_ms(1);
	return freq;
}

/*自动修调*/
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
				//rc_printf("num:%d data:0x%x freq:%d\r\n", i, *data, freq);
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
			//power_off(50);
			//power_on(1000);
			start_sig();
    		cal_sig(*data);
			for (i = 0; i < test_para.sub_bin_para.sub_bin_cal_num; i++) {
				freq = get_freq(test_para.cal_para.bin_cal_test_num);
				if(freq > 0) {
					test_save.freq_save.end_freq = freq / (i + 1) + test_save.freq_save.end_freq * i / (i + 1);
				} 
				else
					return freq;
			}
			exit_test();
			//rc_printf("data:0x%x freq:%d\r\n",*data, test_save.freq_save.end_freq);
			freq_err_old = test_save.freq_save.end_freq - test_para.freq_test_para.target_freq;
			power_off(50);
			power_on(1000);
			(*data)++;
			start_sig();
    		cal_sig(*data);
			for (i = 0; i < test_para.sub_bin_para.sub_bin_cal_num; i++) {
				freq = get_freq(test_para.cal_para.bin_cal_test_num);
				if(freq > 0) {
					test_save.freq_save.end_freq = freq / (i + 1) + test_save.freq_save.end_freq * i / (i + 1);
				} 
				else
					return freq;
			}
			exit_test();
			//rc_printf("data:0x%x freq:%d\r\n",*data, test_save.freq_save.end_freq);
			freq_err = test_save.freq_save.end_freq - test_para.freq_test_para.target_freq;

			if (RC_ABS(freq_err) > RC_ABS(freq_err_old))
				(*data)--;
			//test_save.freq_save.end_freq = freq;
			test_save.freq_save.code = *data;
            return 0;
		}
	} else
		return freq;
}

/*
int32_t auto_cal_test(void)
{
	uint8_t i;
	uint16_t data, data_save;
	int32_t freq;
	char data_b[11];
	data = 0;
	for (i = 0; i < 10; i++) {
		data_save = data;
		data |= 0x0001 << (9 - i);
		freq = cal_one(0, data, test_para.cal_para.freq_cal_test_num[i]);
		get_string_binary(data, data_b, 10);
		data_b[10] = '\0';
		rc_printf("data:%s,%d freq:%d\r\n", data_b, data, freq);
		if (freq > 0) {
			if (freq > test_para.freq_test_para.target_freq)
				data = data_save;
			if (i == 9) {
				if (freq < test_para.freq_test_para.target_freq && (data & 0x3ff) == 0x3ff) {
					return -TARGET_FREQ_TOO_HIGH;
				}
			}
		} else
			return freq;
	}
	return 0;
}*/
