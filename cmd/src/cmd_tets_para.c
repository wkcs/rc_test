#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "rc_debug.h"
#include "print_para.h"
#include "rc_message.h"

void save_para(uint8_t *buf)
{
    uint32_t i;

    test_para.test_signal_para.clk_delay_num = (uint16_t)((buf[0] << 8) + buf[1]);

    test_para.power_para.restart_time = (uint32_t)((buf[2] << 24) + (buf[3] << 16) + (buf[4] << 8) + buf[5]);
    test_para.power_para.open_time = (uint32_t)((buf[6] << 24) + (buf[7] << 16) + (buf[8] << 8) + buf[9]);
    test_para.power_para.clos_time = (uint32_t)((buf[10] << 24) + (buf[11] << 16) + (buf[12] << 8) + buf[13]);

    test_para.freq_test_para.target_freq = (int32_t)((buf[14] << 24) + (buf[15] << 16) + (buf[16] << 8) + buf[17]);

    test_para.cal_para.auto_cal_mode = buf[18];
    test_para.cal_para.freq_cal_test_num[0] = (uint16_t)((buf[19] << 8) + buf[20]);
    test_para.cal_para.freq_cal_test_num[1] = (uint16_t)((buf[21] << 8) + buf[22]);
    test_para.cal_para.freq_cal_test_num[2] = (uint16_t)((buf[23] << 8) + buf[24]);
    test_para.cal_para.freq_cal_test_num[3] = (uint16_t)((buf[25] << 8) + buf[26]);
    test_para.cal_para.freq_cal_test_num[4] = (uint16_t)((buf[27] << 8) + buf[28]);
    test_para.cal_para.freq_cal_test_num[5] = (uint16_t)((buf[29] << 8) + buf[30]);
    test_para.cal_para.freq_cal_test_num[6] = (uint16_t)((buf[31] << 8) + buf[32]);
    test_para.cal_para.freq_cal_test_num[7] = (uint16_t)((buf[33] << 8) + buf[34]);
    test_para.cal_para.freq_cal_test_num[8] = (uint16_t)((buf[35] << 8) + buf[36]);
    test_para.cal_para.freq_cal_test_num[9] = (uint16_t)((buf[37] << 8) + buf[38]);
    test_para.cal_para.start_cal_test_num = (uint16_t)((buf[39] << 8) + buf[40]);
    test_para.cal_para.bin_cal_test_num = (uint16_t)((buf[41] << 8) + buf[42]);

    test_para.efuse_para.efuse_mode = buf[43];
    test_para.efuse_para.efuse_test_num = (uint16_t)((buf[44] << 8) + buf[45]);
    test_para.efuse_para.efuse_time = (uint16_t)((buf[46] << 8) + buf[47]);

    test_para.sub_bin_para.bin_en = buf[48];
    for (i = 0; i < RC_ERR_NO_MAX; i++)
        test_para.sub_bin_para.err_bin[i] = buf[49 + i];
    for (i = 0; i < 8; i++)
        test_para.sub_bin_para.bin_freq_err_max[i] = (int32_t)((buf[78 + 4 * i] << 24) +
                                                               (buf[79 + 4 * i] << 16) + (buf[80 + 4 * i] << 8) + buf[81 + 4 * i]);
    test_para.sub_bin_para.default_bin = buf[110];
    test_para.sub_bin_para.sub_bin_cal_num = buf[111];

    test_para.chip_para.chip_type = buf[112];
    test_para.chip_para.code_width_type = buf[113];

    test_para.test_en_para.os_test_en = buf[114];
    test_para.test_en_para.code_width_test_en = buf[115];
    test_para.test_en_para.standby_current_test_en = buf[116];
    test_para.test_en_para.work_current_test_en = buf[117];
    test_para.test_en_para.auto_cal_test_en = buf[118];
    test_para.test_en_para.efuse_test_en = buf[119];
    test_para.test_en_para.sub_bin_test_en = buf[120];

    test_para.voltage_para.default_voltage = (uint16_t)((buf[121] << 8) + buf[122]);
    test_para.voltage_para.efuse_voltage = (uint16_t)((buf[123] << 8) + buf[124]);
    test_para.current_para.standby_current_max = (uint16_t)((buf[125] << 8) + buf[126]);
    test_para.current_para.work_current_max = (uint16_t)((buf[127] << 8) + buf[128]);
    test_para.current_para.work_current_min = (uint16_t)((buf[129] << 8) + buf[130]);

    test_para.debug_para.debug_info_en = buf[131];
}

void load_para(void)
{
    uint8_t tx_buf[sizeof(test_para_t) - 13];

    tx_buf[0] = 0;
    tx_buf[1] = 133;
    tx_buf[2] = 0; //0:上传
    /*test_signal_para*/
    tx_buf[3] = (uint8_t)(test_para.test_signal_para.clk_delay_num >> 8);
    tx_buf[4] = (uint8_t)test_para.test_signal_para.clk_delay_num;
    /*power_para*/
    tx_buf[5] = (uint8_t)(test_para.power_para.restart_time >> 24);
    tx_buf[6] = (uint8_t)(test_para.power_para.restart_time >> 16);
    tx_buf[7] = (uint8_t)(test_para.power_para.restart_time >> 8);
    tx_buf[8] = (uint8_t)test_para.power_para.restart_time;
    tx_buf[9] = (uint8_t)(test_para.power_para.open_time >> 24);
    tx_buf[10] = (uint8_t)(test_para.power_para.open_time >> 16);
    tx_buf[11] = (uint8_t)(test_para.power_para.open_time >> 8);
    tx_buf[12] = (uint8_t)test_para.power_para.open_time;
    tx_buf[13] = (uint8_t)(test_para.power_para.clos_time >> 24);
    tx_buf[14] = (uint8_t)(test_para.power_para.clos_time >> 16);
    tx_buf[15] = (uint8_t)(test_para.power_para.clos_time >> 8);
    tx_buf[16] = (uint8_t)test_para.power_para.clos_time;
    /*freq_test_para*/
    tx_buf[17] = (uint8_t)(test_para.freq_test_para.target_freq >> 24);
    tx_buf[18] = (uint8_t)(test_para.freq_test_para.target_freq >> 16);
    tx_buf[19] = (uint8_t)(test_para.freq_test_para.target_freq >> 8);
    tx_buf[20] = (uint8_t)test_para.freq_test_para.target_freq;
    /*cal_para*/
    tx_buf[21] = test_para.cal_para.auto_cal_mode;
    tx_buf[22] = (uint8_t)(test_para.cal_para.freq_cal_test_num[0] >> 8);
    tx_buf[23] = (uint8_t)test_para.cal_para.freq_cal_test_num[0];
    tx_buf[24] = (uint8_t)(test_para.cal_para.freq_cal_test_num[1] >> 8);
    tx_buf[25] = (uint8_t)test_para.cal_para.freq_cal_test_num[1];
    tx_buf[26] = (uint8_t)(test_para.cal_para.freq_cal_test_num[2] >> 8);
    tx_buf[27] = (uint8_t)test_para.cal_para.freq_cal_test_num[2];
    tx_buf[28] = (uint8_t)(test_para.cal_para.freq_cal_test_num[3] >> 8);
    tx_buf[29] = (uint8_t)test_para.cal_para.freq_cal_test_num[3];
    tx_buf[30] = (uint8_t)(test_para.cal_para.freq_cal_test_num[4] >> 8);
    tx_buf[31] = (uint8_t)test_para.cal_para.freq_cal_test_num[4];
    tx_buf[32] = (uint8_t)(test_para.cal_para.freq_cal_test_num[5] >> 8);
    tx_buf[33] = (uint8_t)test_para.cal_para.freq_cal_test_num[5];
    tx_buf[34] = (uint8_t)(test_para.cal_para.freq_cal_test_num[6] >> 8);
    tx_buf[35] = (uint8_t)test_para.cal_para.freq_cal_test_num[6];
    tx_buf[36] = (uint8_t)(test_para.cal_para.freq_cal_test_num[7] >> 8);
    tx_buf[37] = (uint8_t)test_para.cal_para.freq_cal_test_num[7];
    tx_buf[38] = (uint8_t)(test_para.cal_para.freq_cal_test_num[8] >> 8);
    tx_buf[39] = (uint8_t)test_para.cal_para.freq_cal_test_num[8];
    tx_buf[40] = (uint8_t)(test_para.cal_para.freq_cal_test_num[9] >> 8);
    tx_buf[41] = (uint8_t)test_para.cal_para.freq_cal_test_num[9];
    tx_buf[42] = (uint8_t)(test_para.cal_para.start_cal_test_num >> 8);
    tx_buf[43] = (uint8_t)test_para.cal_para.start_cal_test_num;
    tx_buf[44] = (uint8_t)(test_para.cal_para.bin_cal_test_num >> 8);
    tx_buf[45] = (uint8_t)test_para.cal_para.bin_cal_test_num;
    /*efuse_para*/
    tx_buf[46] = test_para.efuse_para.efuse_mode;
    tx_buf[47] = (uint8_t)(test_para.efuse_para.efuse_test_num >> 8);
    tx_buf[48] = (uint8_t)test_para.efuse_para.efuse_test_num;
    tx_buf[49] = (uint8_t)(test_para.efuse_para.efuse_time >> 8);
    tx_buf[50] = (uint8_t)test_para.efuse_para.efuse_time;
    /*sub_bin_para*/
    tx_buf[51] = test_para.sub_bin_para.bin_en;
    tx_buf[52] = test_para.sub_bin_para.err_bin[0];
    tx_buf[53] = test_para.sub_bin_para.err_bin[1];
    tx_buf[54] = test_para.sub_bin_para.err_bin[2];
    tx_buf[55] = test_para.sub_bin_para.err_bin[3];
    tx_buf[56] = test_para.sub_bin_para.err_bin[4];
    tx_buf[57] = test_para.sub_bin_para.err_bin[5];
    tx_buf[58] = test_para.sub_bin_para.err_bin[6];
    tx_buf[59] = test_para.sub_bin_para.err_bin[7];
    tx_buf[60] = test_para.sub_bin_para.err_bin[8];
    tx_buf[61] = test_para.sub_bin_para.err_bin[9];
    tx_buf[62] = test_para.sub_bin_para.err_bin[10];
    tx_buf[63] = test_para.sub_bin_para.err_bin[11];
    tx_buf[64] = test_para.sub_bin_para.err_bin[12];
    tx_buf[65] = test_para.sub_bin_para.err_bin[13];
    tx_buf[66] = test_para.sub_bin_para.err_bin[14];
    tx_buf[67] = test_para.sub_bin_para.err_bin[15];
    tx_buf[68] = test_para.sub_bin_para.err_bin[16];
    tx_buf[69] = test_para.sub_bin_para.err_bin[17];
    tx_buf[70] = test_para.sub_bin_para.err_bin[18];
    tx_buf[71] = test_para.sub_bin_para.err_bin[19];
    tx_buf[72] = test_para.sub_bin_para.err_bin[20];
    tx_buf[73] = test_para.sub_bin_para.err_bin[21];
    tx_buf[74] = test_para.sub_bin_para.err_bin[22];
    tx_buf[75] = test_para.sub_bin_para.err_bin[23];
    tx_buf[76] = test_para.sub_bin_para.err_bin[24];
    tx_buf[77] = test_para.sub_bin_para.err_bin[25];
    tx_buf[78] = test_para.sub_bin_para.err_bin[26];
    tx_buf[79] = test_para.sub_bin_para.err_bin[27];
    tx_buf[80] = test_para.sub_bin_para.err_bin[28];
    tx_buf[81] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[0] >> 24);
    tx_buf[82] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[0] >> 16);
    tx_buf[83] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[0] >> 8);
    tx_buf[84] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[0];
    tx_buf[85] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[1] >> 24);
    tx_buf[86] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[1] >> 16);
    tx_buf[87] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[1] >> 8);
    tx_buf[88] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[1];
    tx_buf[89] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[2] >> 24);
    tx_buf[90] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[2] >> 16);
    tx_buf[91] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[2] >> 8);
    tx_buf[92] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[2];
    tx_buf[93] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[3] >> 24);
    tx_buf[94] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[3] >> 16);
    tx_buf[95] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[3] >> 8);
    tx_buf[96] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[3];
    tx_buf[97] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[4] >> 24);
    tx_buf[98] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[4] >> 16);
    tx_buf[99] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[4] >> 8);
    tx_buf[100] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[4];
    tx_buf[101] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[5] >> 24);
    tx_buf[102] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[5] >> 16);
    tx_buf[103] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[5] >> 8);
    tx_buf[104] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[5];
    tx_buf[105] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[6] >> 24);
    tx_buf[106] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[6] >> 16);
    tx_buf[107] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[6] >> 8);
    tx_buf[108] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[6];
    tx_buf[109] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[7] >> 24);
    tx_buf[110] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[7] >> 16);
    tx_buf[111] = (uint8_t)(test_para.sub_bin_para.bin_freq_err_max[7] >> 8);
    tx_buf[112] = (uint8_t)test_para.sub_bin_para.bin_freq_err_max[7];
    tx_buf[113] = test_para.sub_bin_para.default_bin;
    tx_buf[114] = test_para.sub_bin_para.sub_bin_cal_num;
    /*chip_para*/
    tx_buf[115] = test_para.chip_para.chip_type;
    tx_buf[116] = test_para.chip_para.code_width_type;
    /*test_en_para*/
    tx_buf[117] = test_para.test_en_para.os_test_en;
    tx_buf[118] = test_para.test_en_para.code_width_test_en;
    tx_buf[119] = test_para.test_en_para.standby_current_test_en;
    tx_buf[120] = test_para.test_en_para.work_current_test_en;
    tx_buf[121] = test_para.test_en_para.auto_cal_test_en;
    tx_buf[122] = test_para.test_en_para.efuse_test_en;
    tx_buf[123] = test_para.test_en_para.sub_bin_test_en;
    /*voltage_para*/
    tx_buf[124] = (uint8_t)(test_para.voltage_para.default_voltage >> 8);
    tx_buf[125] = (uint8_t)test_para.voltage_para.default_voltage;
    tx_buf[126] = (uint8_t)(test_para.voltage_para.efuse_voltage >> 8);
    tx_buf[127] = (uint8_t)test_para.voltage_para.efuse_voltage;
    /*current_para*/
    tx_buf[128] = (uint8_t)(test_para.current_para.standby_current_max >> 8);
    tx_buf[129] = (uint8_t)test_para.current_para.standby_current_max;
    tx_buf[130] = (uint8_t)(test_para.current_para.work_current_max >> 8);
    tx_buf[131] = (uint8_t)test_para.current_para.work_current_max;
    tx_buf[132] = (uint8_t)(test_para.current_para.work_current_min >> 8);
    tx_buf[133] = (uint8_t)test_para.current_para.work_current_min;
    /*debug_para*/
    tx_buf[134] = test_para.debug_para.debug_info_en;

    rc_send_message(tx_buf + 2, 133, TEST_PARA_MES);
}

static void cmd_main(uint8_t *buf)
{
    switch (buf[0])
    {
    case 0:
        save_para(buf + 1);
        test_para_save();
        if (test_para.debug_para.debug_info_en)
            print_test_para();
        break;
    case 1:
        load_para();
        break;
    case 2:
        test_para_restore();
        load_para();
        break;
    }
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_test_para = {
    CMD_TEST_PARA,
    cmd_main
};


