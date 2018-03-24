#include "rc11x_test.h"
#include "test_cal.h"
#include "test_efuse.h"
#include "test_bin.h"
#include "test_check.h"
#include "test_machine_com.h"
#include "test_type.h"
#include "test_current.h"
#include "test_os.h"
#include "rc_err.h"
#include "rc_debug.h"
#include "rc_message.h"

/*开始一次测试，返回分档结果*/
uint8_t rc11x_test_start(void)
{
    uint8_t bin;
    int32_t err;
    uint16_t data = 0;
    /*测试前先初始化用于保存测试结果的结构体*/
    test_save_init();
    power_on(test_para.power_para.open_time);
    if (test_para.test_en_para.os_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        err = os_test();
        //power_off(test_para.power_para.clos_time);
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"os_test\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.code_width_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        err = check_code_width();
        //power_off(test_para.power_para.clos_time);
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"check_code_width\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.standby_current_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        err = standby_current_test();
        //power_off(test_para.power_para.clos_time);
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"standby_current_test\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.work_current_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        err = work_current_test();
        //power_off(test_para.power_para.clos_time);
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"work_current_test\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.auto_cal_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        //err = auto_cal_test();
        err = auto_cal(test_para.cal_para.auto_cal_mode, &data, test_save.freq_save.start_freq);
        //power_off(test_para.power_para.clos_time);
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"auto_cal\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.efuse_test_en)
    {
        if (data != 0) {
            rc_power_con_dac(test_para.voltage_para.efuse_voltage);  
            err = efuse_chip(test_para.efuse_para.efuse_mode, data, 0);
            rc_power_con_dac(test_para.voltage_para.default_voltage);
            //power_off(test_para.power_para.clos_time);
        }
        else
            err = 0;
        if (err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"efuse_chip\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
    }
    if (test_para.test_en_para.sub_bin_test_en)
    {
        rc_power_con_dac(test_para.voltage_para.default_voltage);  
        err = get_bin_from_freq(&bin);
        if(err != 0)
        {
            if (test_para.debug_para.debug_info_en)
				rc_printf("error by \"get_bin_from_freq\"\r\n");
            bin = get_bin_from_err(err);
            test_save.err_save = err;
            power_off(test_para.power_para.clos_time);
            test_save.bin_save = bin;
            return bin;
        }
        power_off(test_para.power_para.clos_time);
        test_save.bin_save = bin;
        return bin;
    }
    
    power_off(test_para.power_para.clos_time);
    test_save.bin_save = test_para.sub_bin_para.default_bin;
    return test_para.sub_bin_para.default_bin;
}

/*向上位机发送测试结果*/
void send_test_results(void)
{
    uint8_t tx_buf[79];
    tx_buf[0] = 1;
    tx_buf[1] = 77;
    /*chip_type_save*/
    tx_buf[2] = test_save.chip_type_save.chip_type;
    tx_buf[3] = test_save.chip_type_save.code_width_type;
    /*freq_save*/
    tx_buf[4] = (uint8_t)(test_save.freq_save.target_freq >> 24);
    tx_buf[5] = (uint8_t)(test_save.freq_save.target_freq >> 16);
    tx_buf[6] = (uint8_t)(test_save.freq_save.target_freq >> 8);
    tx_buf[7] = (uint8_t)test_save.freq_save.target_freq;
    tx_buf[8] = (uint8_t)(test_save.freq_save.freq_err >> 24);
    tx_buf[9] = (uint8_t)(test_save.freq_save.freq_err >> 16);
    tx_buf[10] = (uint8_t)(test_save.freq_save.freq_err >> 8);
    tx_buf[11] = (uint8_t)test_save.freq_save.freq_err;
    tx_buf[12] = (uint8_t)(test_save.freq_save.start_freq >> 24);
    tx_buf[13] = (uint8_t)(test_save.freq_save.start_freq >> 16);
    tx_buf[14] = (uint8_t)(test_save.freq_save.start_freq >> 8);
    tx_buf[15] = (uint8_t)test_save.freq_save.start_freq;
    tx_buf[16] = (uint8_t)(test_save.freq_save.end_freq >> 24);
    tx_buf[17] = (uint8_t)(test_save.freq_save.end_freq >> 16);
    tx_buf[18] = (uint8_t)(test_save.freq_save.end_freq >> 8);
    tx_buf[19] = (uint8_t)test_save.freq_save.end_freq;
    tx_buf[20] = (uint8_t)(test_save.freq_save.code >> 8);
    tx_buf[21] = (uint8_t)test_save.freq_save.code;
    /*err_save*/
    tx_buf[22] = (uint8_t)(test_save.err_save >> 24);
    tx_buf[23] = (uint8_t)(test_save.err_save >> 16);
    tx_buf[24] = (uint8_t)(test_save.err_save >> 8);
    tx_buf[25] = (uint8_t)test_save.err_save;
    /*voltage_save*/
    tx_buf[26] = (uint8_t)(test_save.voltage_save.default_voltage >> 8);
    tx_buf[27] = (uint8_t)test_save.voltage_save.default_voltage;
    tx_buf[28] = (uint8_t)(test_save.voltage_save.efuse_voltage >> 8);
    tx_buf[29] = (uint8_t)test_save.voltage_save.efuse_voltage;
    /*current_save*/
    tx_buf[30] = (uint8_t)(test_save.current_save.standby_current >> 8);
    tx_buf[31] = (uint8_t)test_save.current_save.standby_current;
    tx_buf[32] = (uint8_t)(test_save.current_save.work_current_k0 >> 8);
    tx_buf[33] = (uint8_t)test_save.current_save.work_current_k0;
    tx_buf[34] = (uint8_t)(test_save.current_save.work_current_k1 >> 8);
    tx_buf[35] = (uint8_t)test_save.current_save.work_current_k1;
    tx_buf[36] = (uint8_t)(test_save.current_save.work_current_k2 >> 8);
    tx_buf[37] = (uint8_t)test_save.current_save.work_current_k2;
    tx_buf[38] = (uint8_t)(test_save.current_save.work_current_k3 >> 8);
    tx_buf[39] = (uint8_t)test_save.current_save.work_current_k3;
    tx_buf[40] = (uint8_t)(test_save.current_save.work_current_k4 >> 8);
    tx_buf[41] = (uint8_t)test_save.current_save.work_current_k4;
    tx_buf[42] = (uint8_t)(test_save.current_save.work_current_k5 >> 8);
    tx_buf[43] = (uint8_t)test_save.current_save.work_current_k5;
    tx_buf[44] = (uint8_t)(test_save.current_save.work_current_k6 >> 8);
    tx_buf[45] = (uint8_t)test_save.current_save.work_current_k6;
    tx_buf[46] = (uint8_t)(test_save.current_save.work_current_k7 >> 8);
    tx_buf[47] = (uint8_t)test_save.current_save.work_current_k7;
    tx_buf[48] = (uint8_t)(test_save.current_save.work_current_k8 >> 8);
    tx_buf[49] = (uint8_t)test_save.current_save.work_current_k8;
    tx_buf[50] = (uint8_t)(test_save.current_save.work_current_k9 >> 8);
    tx_buf[51] = (uint8_t)test_save.current_save.work_current_k9;
    /*os_save*/
    tx_buf[52] = (uint8_t)(test_save.os_save.os_voltage_1 >> 8);
    tx_buf[53] = (uint8_t)test_save.os_save.os_voltage_1;
    tx_buf[54] = (uint8_t)(test_save.os_save.os_voltage_2 >> 8);
    tx_buf[55] = (uint8_t)test_save.os_save.os_voltage_2;
    tx_buf[56] = (uint8_t)(test_save.os_save.os_voltage_3 >> 8);
    tx_buf[57] = (uint8_t)test_save.os_save.os_voltage_3;
    tx_buf[58] = (uint8_t)(test_save.os_save.os_voltage_4 >> 8);
    tx_buf[59] = (uint8_t)test_save.os_save.os_voltage_4;
    tx_buf[60] = (uint8_t)(test_save.os_save.os_voltage_5 >> 8);
    tx_buf[61] = (uint8_t)test_save.os_save.os_voltage_5;
    tx_buf[62] = (uint8_t)(test_save.os_save.os_voltage_6 >> 8);
    tx_buf[63] = (uint8_t)test_save.os_save.os_voltage_6;
    tx_buf[64] = (uint8_t)(test_save.os_save.os_voltage_7 >> 8);
    tx_buf[65] = (uint8_t)test_save.os_save.os_voltage_7;
    tx_buf[66] = (uint8_t)(test_save.os_save.os_voltage_8 >> 8);
    tx_buf[67] = (uint8_t)test_save.os_save.os_voltage_8;
    tx_buf[68] = (uint8_t)(test_save.os_save.os_voltage_9 >> 8);
    tx_buf[69] = (uint8_t)test_save.os_save.os_voltage_9;
    tx_buf[70] = (uint8_t)(test_save.os_save.os_voltage_10 >> 8);
    tx_buf[71] = (uint8_t)test_save.os_save.os_voltage_10;
    tx_buf[72] = (uint8_t)(test_save.os_save.os_voltage_11 >> 8);
    tx_buf[73] = (uint8_t)test_save.os_save.os_voltage_11;
    tx_buf[74] = (uint8_t)(test_save.os_save.os_voltage_12 >> 8);
    tx_buf[75] = (uint8_t)test_save.os_save.os_voltage_12;
    tx_buf[76] = (uint8_t)(test_save.os_save.os_voltage_13 >> 8);
    tx_buf[77] = (uint8_t)test_save.os_save.os_voltage_13;
    /*bin_save*/
    tx_buf[78] = (uint8_t)test_save.bin_save;
    rc_send_message(tx_buf + 2, 77, TEST_SAVE_MES);
}
