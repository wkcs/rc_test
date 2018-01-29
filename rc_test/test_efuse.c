#include "test_efuse.h"
#include "rc_err.h"
#include "delay.h"

int32_t efuse_chip(uint8_t mode, uint16_t data, uint16_t test_num)
{
    int32_t freq = 0;

    power_restart(test_para.power_para.restart_time);
    start_sig();
    efuse_sig(data);

    power_restart(test_para.power_para.restart_time);
    start_sig();
    efuse_sig(data);
    if (mode)
    {
        freq = get_freq(test_num);
        test_save.freq_save.freq_err = freq - test_save.freq_save.target_freq;
        test_save.freq_save.end_freq = freq;
    }
    delay_ms(test_para.efuse_para.efuse_time);
    exit_test();
    test_save.freq_save.code = data;
    return freq;
}

