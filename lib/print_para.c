#include "print_para.h"
#include "usart.h"
#include "test_type.h"
#include "rc_err.h"
#include "rc_debug.h"

void print_test_para(void)
{
    uint32_t i;

    rc_printf("test_signal_para\r\n");
    rc_printf("|---clk_delay_num:%d\r\n", test_para.test_signal_para.clk_delay_num);

    rc_printf("power_para\r\n");
    rc_printf("|---restart_time:%dus\r\n", test_para.power_para.restart_time);
    rc_printf("|---open_time:%dus\r\n", test_para.power_para.open_time);
    rc_printf("|---clos_time:%dus\r\n", test_para.power_para.clos_time);

    rc_printf("freq_test_para\r\n");
    rc_printf("|---target_freq:%dHz\r\n", test_para.freq_test_para.target_freq);

    rc_printf("cal_para\r\n");
    rc_printf("|---auto_cal_mode:%d\r\n", test_para.cal_para.auto_cal_mode);
    rc_printf("|---freq_cal_test_num\r\n");
    for (i = 0; i < 10; i++)
    {
        rc_printf("|   |---%d:%d\r\n", i, test_para.cal_para.freq_cal_test_num[i]);
    }
    rc_printf("|---start_cal_test_num:%d\r\n", test_para.cal_para.start_cal_test_num);
    rc_printf("|---bin_cal_test_num:%d\r\n", test_para.cal_para.bin_cal_test_num);

    rc_printf("efuse_para\r\n");
    rc_printf("|---efuse_mode:%d\r\n", test_para.efuse_para.efuse_mode);
    rc_printf("|---efuse_test_num:%d\r\n", test_para.efuse_para.efuse_test_num);
    rc_printf("|---efuse_time:%dms\r\n", test_para.efuse_para.efuse_time);

    rc_printf("sub_bin_para\r\n");
    rc_printf("|---bin_en:0x%02x\r\n", test_para.sub_bin_para.bin_en);
    rc_printf("|---bin_freq_err_max\r\n");
    for (i = 0; i < 8; i++)
    {
        rc_printf("|   |---Bin%d:%dHz\r\n", i + 1, test_para.sub_bin_para.bin_freq_err_max[i]);
    }
    rc_printf("|---err_bin\r\n");
    for (i = 0; i < RC_ERR_NO_MAX; i++)
    {
        rc_printf("|   |---%s:Bin%d\r\n", get_err_name(i + 1), test_para.sub_bin_para.err_bin[i]);
    }
    rc_printf("|---default_bin:Bin%d\r\n", test_para.sub_bin_para.default_bin);
    rc_printf("|---sub_bin_cal_num:%d\r\n", test_para.sub_bin_para.sub_bin_cal_num);

    rc_printf("chip_para\r\n");
    rc_printf("|---chip_type:%s\r\n", get_chip_type_name(test_para.chip_para.chip_type));
    if (test_para.chip_para.chip_type < 3)
        rc_printf("|---code_width_type:%s\r\n", get_code_width_433_type_name(test_para.chip_para.code_width_type));
    else
        rc_printf("|---code_width_type:%s\r\n", get_code_width_315_type_name(test_para.chip_para.code_width_type));
    
    rc_printf("test_en_para\r\n");
    rc_printf("|---os_test_en:%d\r\n", test_para.test_en_para.os_test_en);
    rc_printf("|---code_width_test_en:%d\r\n", test_para.test_en_para.code_width_test_en);
    rc_printf("|---standby_current_test_en:%d\r\n", test_para.test_en_para.standby_current_test_en);
    rc_printf("|---work_current_test_en:%d\r\n", test_para.test_en_para.work_current_test_en);
    rc_printf("|---auto_cal_test_en:%d\r\n", test_para.test_en_para.auto_cal_test_en);
    rc_printf("|---efuse_test_en:%d\r\n", test_para.test_en_para.efuse_test_en);
    rc_printf("|---sub_bin_test_en:%d\r\n", test_para.test_en_para.sub_bin_test_en);

    rc_printf("voltage_para\r\n");
    rc_printf("|---default_voltage:%d.%03dV\r\n", test_para.voltage_para.default_voltage / 1000, test_para.voltage_para.default_voltage % 1000);
    rc_printf("|---efuse_voltage:%d.%03dV\r\n", test_para.voltage_para.efuse_voltage / 1000, test_para.voltage_para.efuse_voltage % 1000);

    rc_printf("current_para\r\n");
    rc_printf("|---standby_current_max:%duA\r\n", test_para.current_para.standby_current_max);
    rc_printf("|---work_current_max:%d.%03dmA\r\n", test_para.current_para.work_current_max / 1000, test_para.current_para.work_current_max % 1000);
    rc_printf("|---work_current_min:%d.%03dmA\r\n", test_para.current_para.work_current_min / 1000, test_para.current_para.work_current_min % 1000);

    rc_printf("debug_para\r\n");
    rc_printf("|---debug_info_en:%d\r\n", test_para.debug_para.debug_info_en);
}

void print_test_data(void)
{
    rc_printf("This has not yet been achieved\r\n");
}