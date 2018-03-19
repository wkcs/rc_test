#include "test_check.h"
#include "rc_math.h"
#include "rc_debug.h"

/*检测码宽*/
int32_t check_code_width(void)
{
    int32_t freq1, freq2;
    int8_t temp;
    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();
    
    start_sig();
    cal_sig(0);
    freq1 = get_freq(test_para.cal_para.bin_cal_test_num);
    if (freq1 <= 0)
    {
        power_restart(test_para.power_para.restart_time);
        start_sig();
        cal_sig(0);
        freq1 = get_freq(test_para.cal_para.bin_cal_test_num);
        if (freq1 <= 0)
            return freq1;
    }
    test_save.freq_save.start_freq = freq1;
    exit_test();
    start_sig();
    check_sig();
    freq2 = get_freq(1000);
    if (freq2 <= 0)
    {
        power_restart(test_para.power_para.restart_time);
        start_sig();
        check_sig();
        freq2 = get_freq(1000);
        if (freq2 <= 0)
            return freq2;
    }
    temp = (int8_t)(freq1 * 10 / freq2 / 4);
    if (RC_ABS(temp - test_para.chip_para.code_width_type * 10) < 5)
    {
        if (test_para.debug_para.debug_info_en){
            if (temp % 10 < 5)
                temp = temp / 10;
            else
                temp = temp / 10 + 1;
            if (test_para.chip_para.chip_type < 3)
                rc_printf("code_width_type:%s\r\n", get_code_width_433_type_name(temp));
            else
                rc_printf("code_width_type:%s\r\n", get_code_width_315_type_name(temp));
        }
        return 0;
    }
    else {
        if (test_para.debug_para.debug_info_en) {
            if (temp % 10 < 5)
                temp = temp / 10;
            else
                temp = temp / 10 + 1;
            if (test_para.chip_para.chip_type < 3)
                rc_printf("code_width_type:%s\r\n", get_code_width_433_type_name(temp));
            else
                rc_printf("code_width_type:%s\r\n", get_code_width_315_type_name(temp));
        }
        return -CODE_WIDTH_ERR;
    }
}

