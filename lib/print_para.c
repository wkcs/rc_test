#include "print_para.h"
#include "usart.h"
#include "test_type.h"
#include "rc_err.h"

void print_test_para(void)
{
    uint32_t i;

    printf("test_signal_para\r\n");
    printf("|---clk_delay_num:%d\r\n", test_para.test_signal_para.clk_delay_num);

    printf("power_para\r\n");
    printf("|---restart_time:%dus\r\n", test_para.power_para.restart_time);
    printf("|---open_time:%dus\r\n", test_para.power_para.open_time);
    printf("|---clos_time:%dus\r\n", test_para.power_para.clos_time);

    printf("freq_test_para\r\n");
    printf("|---target_freq:%dHz\r\n", test_para.freq_test_para.target_freq);

    printf("cal_para\r\n");
    printf("|---auto_cal_mode:%d\r\n", test_para.cal_para.auto_cal_mode);
    printf("|---freq_cal_test_num\r\n");
    for (i = 0; i < 10; i++)
    {
        printf("|   |---%d:%d\r\n", i, test_para.cal_para.freq_cal_test_num[i]);
    }
    printf("|---start_cal_test_num:%d\r\n", test_para.cal_para.start_cal_test_num);
    printf("|---bin_cal_test_num:%d\r\n", test_para.cal_para.bin_cal_test_num);

    printf("efuse_para\r\n");
    printf("|---efuse_mode:%d\r\n", test_para.efuse_para.efuse_mode);
    printf("|---efuse_test_num:%d\r\n", test_para.efuse_para.efuse_test_num);
    printf("|---efuse_time:%dms\r\n", test_para.efuse_para.efuse_time);

    printf("sub_bin_para\r\n");
    printf("|---bin_en:0x%02x\r\n", test_para.sub_bin_para.bin_en);
    printf("|---bin_freq_err_max\r\n");
    for (i = 0; i < 8; i++)
    {
        printf("|   |---Bin%d:%dHz\r\n", i + 1, test_para.sub_bin_para.bin_freq_err_max[i]);
    }
    printf("|---err_bin\r\n");
    for (i = 0; i < RC_ERR_NO_MAX; i++)
    {
        printf("|   |---%s:Bin%d\r\n", get_err_name(i + 1), test_para.sub_bin_para.err_bin[i]);
    }
    printf("|---default_bin:Bin%d\r\n", test_para.sub_bin_para.default_bin);

    printf("chip_para\r\n");
    printf("|---chip_type:%s\r\n", get_chip_type_name(test_para.chip_para.chip_type));
    if (test_para.chip_para.chip_type < 3)
        printf("|---horse_wide_type:%s\r\n", get_horse_wide_433_type_name(test_para.chip_para.horse_wide_type));
    else
        printf("|---horse_wide_type:%s\r\n", get_horse_wide_315_type_name(test_para.chip_para.horse_wide_type));
    
    printf("test_en_para\r\n");
    printf("|---os_test_en:%d\r\n", test_para.test_en_para.os_test_en);
    printf("|---horse_wide_test_en:%d\r\n", test_para.test_en_para.horse_wide_test_en);
    printf("|---standby_current_test_en:%d\r\n", test_para.test_en_para.standby_current_test_en);
    printf("|---work_current_test_en:%d\r\n", test_para.test_en_para.work_current_test_en);
    printf("|---auto_cal_test_en:%d\r\n", test_para.test_en_para.auto_cal_test_en);
    printf("|---efuse_test_en:%d\r\n", test_para.test_en_para.efuse_test_en);
    printf("|---sub_bin_test_en:%d\r\n", test_para.test_en_para.sub_bin_test_en);

    printf("voltage_para\r\n");
    printf("|---default_voltage:%d.%03dV\r\n", test_para.voltage_para.default_voltage / 1000, test_para.voltage_para.default_voltage % 1000);
    printf("|---efuse_voltage:%d.%03dV\r\n", test_para.voltage_para.efuse_voltage / 1000, test_para.voltage_para.efuse_voltage % 1000);

    printf("current_para\r\n");
    printf("|---standby_current_max:%duA\r\n", test_para.current_para.standby_current_max);
    printf("|---work_current_max:%d.%03dmA\r\n", test_para.current_para.work_current_max / 1000, test_para.current_para.work_current_max % 1000);
    printf("|---work_current_min:%d.%03dmA\r\n", test_para.current_para.work_current_min / 1000, test_para.current_para.work_current_min % 1000);
}

void print_test_data(void)
{
    printf("This has not yet been achieved\r\n");
}