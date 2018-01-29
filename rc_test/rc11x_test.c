#include "rc11x_test.h"
#include "test_cal.h"
#include "test_efuse.h"
#include "test_bin.h"
#include "test_check.h"
#include "test_machine_com.h"
#include "test_type.h"
#include "test_current.h"
#include "rc_err.h"

uint8_t rc11x_test_start(void)
{
    uint8_t bin;
    int32_t err;
    uint16_t data;
    test_save_init();
    power_on(test_para.power_para.open_time);
    if (test_para.test_en_para.os_test_en)
    {

    }
    if (test_para.test_en_para.horse_wide_test_en)
    {
        err = check_horse_wide();
        if (err != 0)
        {
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.standby_current_test_en)
    {
        err = standby_current_test();
        if (err != 0)
        {
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.work_current_test_en)
    {
        err = work_current_test();
        if (err != 0)
        {
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.auto_cal_test_en)
    {
        err = auto_cal(test_para.cal_para.auto_cal_mode, &data, test_save.freq_save.start_freq);
        if (err != 0)
        {
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.efuse_test_en)
    {
        err = efuse_chip(test_para.efuse_para.efuse_mode, data, 0);
        if (err != 0)
        {
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.sub_bin_test_en)
    {
        bin = get_bin_from_freq();
        if(bin == 0)
        {
            test_save.err_save = -NOT_FIND_BIN;
            bin = get_bin_from_err(-NOT_FIND_BIN);
        }
        power_off(test_para.power_para.clos_time);
        test_save.bin_save = bin;
        return bin;
    }
    
    power_off(test_para.power_para.clos_time);
    test_save.bin_save = test_para.sub_bin_para.default_bin;
    return test_para.sub_bin_para.default_bin;
}