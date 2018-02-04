#include "test_type.h"
#include "board.h"

#define TEST_PARA_HEAD 203044575

test_para_t test_para;
test_data_t test_data;
test_save_t test_save;

void test_para_init(void)
{
    uint32_t buf;
    W25QXX_Read((uint8_t *)(&buf), 0, 4);
    if (buf == TEST_PARA_HEAD)
        test_para_load();
    else
        test_para_restore();
}

void test_para_restore(void)
{
    uint32_t i;

    test_para.test_para_head = TEST_PARA_HEAD;

    test_para.test_signal_para.clk_delay_num = 5;

    test_para.power_para.restart_time = 2000;
    test_para.power_para.open_time = 2000;
    test_para.power_para.clos_time = 2000;

    test_para.freq_test_para.target_freq = 423750;

    test_para.cal_para.auto_cal_mode = 0;
    test_para.cal_para.freq_cal_test_num[0] = 100;
    test_para.cal_para.freq_cal_test_num[1] = 100;
    test_para.cal_para.freq_cal_test_num[2] = 100;
    test_para.cal_para.freq_cal_test_num[3] = 200;
    test_para.cal_para.freq_cal_test_num[4] = 300;
    test_para.cal_para.freq_cal_test_num[5] = 600;
    test_para.cal_para.freq_cal_test_num[6] = 900;
    test_para.cal_para.freq_cal_test_num[7] = 1200;
    test_para.cal_para.freq_cal_test_num[8] = 1500;
    test_para.cal_para.freq_cal_test_num[9] = 10000;
    test_para.cal_para.start_cal_test_num = 2000;
    test_para.cal_para.bin_cal_test_num = 20000;

    test_para.efuse_para.efuse_mode = 0;
    test_para.efuse_para.efuse_test_num = 10000;
    test_para.efuse_para.efuse_time = 5;

    test_para.sub_bin_para.bin_en = 0x0f;
    test_para.sub_bin_para.bin_freq_err_max[0] = 100;
    test_para.sub_bin_para.bin_freq_err_max[1] = 300;
    test_para.sub_bin_para.bin_freq_err_max[2] = 0;
    test_para.sub_bin_para.bin_freq_err_max[3] = 0;
    test_para.sub_bin_para.bin_freq_err_max[4] = 0;
    test_para.sub_bin_para.bin_freq_err_max[5] = 0;
    test_para.sub_bin_para.bin_freq_err_max[6] = 0;
    test_para.sub_bin_para.bin_freq_err_max[7] = 0;
    for(i = 0; i < RC_ERR_NO_MAX; i++)
        test_para.sub_bin_para.err_bin[i] = 4;
    test_para.sub_bin_para.default_bin = 1;
    test_para.sub_bin_para.sub_bin_cal_num = 3;

    test_para.chip_para.chip_type = RC118_433;
    test_para.chip_para.code_width_type = RC_433_1_2;

    test_para.test_en_para.os_test_en = ENABLE;
    test_para.test_en_para.code_width_test_en = ENABLE;
    test_para.test_en_para.standby_current_test_en = ENABLE;
    test_para.test_en_para.work_current_test_en = ENABLE;
    test_para.test_en_para.auto_cal_test_en = ENABLE;
    test_para.test_en_para.efuse_test_en = ENABLE;
    test_para.test_en_para.sub_bin_test_en = ENABLE;

    test_para.voltage_para.default_voltage = 3300;
    test_para.voltage_para.efuse_voltage = 3600;

    test_para.current_para.standby_current_max = 5;
    test_para.current_para.work_current_max = 15000;
    test_para.current_para.work_current_min = 5000;

    test_para.debug_para.debug_info_en = 1;

    W25QXX_Write((uint8_t *)(&test_para), 0, sizeof(test_para));
}

void test_para_load(void)
{
    W25QXX_Read((uint8_t *)(&test_para), 0, sizeof(test_para));
}

void test_para_save(void)
{
    W25QXX_Write((uint8_t *)(&test_para), 0, sizeof(test_para));
}


void test_save_init(void)
{
    test_save.chip_type_save.chip_type = test_para.chip_para.chip_type;
    test_save.chip_type_save.code_width_type = test_para.chip_para.code_width_type;

    test_save.freq_save.code = 0;
    test_save.freq_save.target_freq = test_para.freq_test_para.target_freq;
    test_save.freq_save.end_freq = 0;
    test_save.freq_save.freq_err = 0;
    test_save.freq_save.start_freq = 0;

    test_save.err_save = 0;

    test_save.voltage_save.default_voltage = test_para.voltage_para.default_voltage;
    test_save.voltage_save.efuse_voltage = test_para.voltage_para.efuse_voltage;

    test_save.current_save.standby_current = 0;
    test_save.current_save.work_current_k0 = 0;
    test_save.current_save.work_current_k1 = 0;
    test_save.current_save.work_current_k2 = 0;
    test_save.current_save.work_current_k3 = 0;
    test_save.current_save.work_current_k4 = 0;
    test_save.current_save.work_current_k5 = 0;
    test_save.current_save.work_current_k6 = 0;
    test_save.current_save.work_current_k7 = 0;
    test_save.current_save.work_current_k8 = 0;
    test_save.current_save.work_current_k9 = 0;

    test_save.os_save.os_voltage_2 = 0;
    test_save.os_save.os_voltage_3 = 0;
    test_save.os_save.os_voltage_4 = 0;
    test_save.os_save.os_voltage_5 = 0;
    test_save.os_save.os_voltage_6 = 0;
    test_save.os_save.os_voltage_7 = 0;
    test_save.os_save.os_voltage_8 = 0;
    test_save.os_save.os_voltage_9 = 0;
    test_save.os_save.os_voltage_10 = 0;
    test_save.os_save.os_voltage_11 = 0;
    test_save.os_save.os_voltage_12 = 0;
    test_save.os_save.os_voltage_13 = 0;
    test_save.os_save.os_voltage_14 = 0;

    test_save.bin_save = 0;
}