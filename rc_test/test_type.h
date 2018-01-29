#ifndef __TEST_TYPE_H__
#define __TEST_TYPE_H__

#include "sys.h"
#include "rc_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TYPE_NAME_CASE(x) case x: return (#x)

enum __CHIP_TYPE{
    RC118_433 = 0,
    RC119_433,
    RC117_433,
    RC118_315,
    RC119_315,
    RC117_315,
};

inline const char *get_chip_type_name(uint8_t chip_type)
{
    switch (chip_type)
    {
        TYPE_NAME_CASE(RC118_433);
        TYPE_NAME_CASE(RC118_315);
        TYPE_NAME_CASE(RC119_433);
        TYPE_NAME_CASE(RC119_315);
        TYPE_NAME_CASE(RC117_433);
        TYPE_NAME_CASE(RC117_315);
    }
    return "unknown type";
}

enum __HORSE_WIDE_315_TYPE{
    RC_315_0_4 = 2,
    RC_315_0_6,
    RC_315_0_8,
    RC_315_1_0,
    RC_315_1_2,
    RC_315_1_4,
    RC_315_1_7,
    RC_315_1_9,
    RC_315_2_1,
    RC_315_2_3,
    RC_315_2_5,
    RC_315_2_7,
    RC_315_2_9,
    RC_315_3_1,
    RC_315_3_3,
    RC_315_3_5,
};

inline const char *get_horse_wide_315_type_name(uint8_t horse_wide_type)
{
    switch (horse_wide_type)
    {
        TYPE_NAME_CASE(RC_315_0_4);
        TYPE_NAME_CASE(RC_315_0_6);
        TYPE_NAME_CASE(RC_315_0_8);
        TYPE_NAME_CASE(RC_315_1_0);
        TYPE_NAME_CASE(RC_315_1_2);
        TYPE_NAME_CASE(RC_315_1_4);
        TYPE_NAME_CASE(RC_315_1_7);
        TYPE_NAME_CASE(RC_315_1_9);
        TYPE_NAME_CASE(RC_315_2_1);
        TYPE_NAME_CASE(RC_315_2_3);
        TYPE_NAME_CASE(RC_315_2_5);
        TYPE_NAME_CASE(RC_315_2_7);
        TYPE_NAME_CASE(RC_315_2_9);
        TYPE_NAME_CASE(RC_315_3_1);
        TYPE_NAME_CASE(RC_315_3_3);
        TYPE_NAME_CASE(RC_315_3_5);
    }
    return "unknown type";
}

enum __HORSE_WIDE_433_TYPE{
    RC_433_0_3 = 2,
    RC_433_0_5,
    RC_433_0_6,
    RC_433_0_8,
    RC_433_0_9,
    RC_433_1_1,
    RC_433_1_2,
    RC_433_1_4,
    RC_433_1_5,
    RC_433_1_7,
    RC_433_1_8,
    RC_433_2_0,
    RC_433_2_1,
    RC_433_2_3,
    RC_433_2_4,
    RC_433_2_7,
};

inline const char *get_horse_wide_433_type_name(uint8_t horse_wide_type)
{
    switch (horse_wide_type)
    {
        TYPE_NAME_CASE(RC_433_0_3);
        TYPE_NAME_CASE(RC_433_0_5);
        TYPE_NAME_CASE(RC_433_0_6);
        TYPE_NAME_CASE(RC_433_0_8);
        TYPE_NAME_CASE(RC_433_0_9);
        TYPE_NAME_CASE(RC_433_1_1);
        TYPE_NAME_CASE(RC_433_1_2);
        TYPE_NAME_CASE(RC_433_1_4);
        TYPE_NAME_CASE(RC_433_1_5);
        TYPE_NAME_CASE(RC_433_1_7);
        TYPE_NAME_CASE(RC_433_1_8);
        TYPE_NAME_CASE(RC_433_2_0);
        TYPE_NAME_CASE(RC_433_2_1);
        TYPE_NAME_CASE(RC_433_2_3);
        TYPE_NAME_CASE(RC_433_2_4);
        TYPE_NAME_CASE(RC_433_2_7);
    }
    return "unknown type";
}

typedef struct __test_signal_para
{
    uint16_t clk_delay_num;

} test_signal_para_t;

typedef struct __power_para
{
    uint32_t restart_time;
    uint32_t open_time;
    uint32_t clos_time;
} power_para_t;

typedef struct __freq_test_para
{
    int32_t target_freq;
} freq_test_para_t;

typedef struct __cal_para
{
    uint8_t auto_cal_mode;
    uint16_t freq_cal_test_num[10];
    uint16_t start_cal_test_num;
    uint16_t bin_cal_test_num;
} cal_para_t;

typedef struct __efuse_para
{
    uint8_t efuse_mode;
    uint16_t efuse_test_num;
    uint16_t efuse_time;
} efuse_para_t;

typedef struct __sub_bin_para
{
    uint8_t bin_en;
    uint8_t err_bin[RC_ERR_NO_MAX];
    int32_t bin_freq_err_max[8];
    uint8_t default_bin;
} sub_bin_para_t;

typedef struct __chip_para
{
    uint8_t chip_type;
    uint8_t horse_wide_type;
} chip_para_t;

typedef struct __test_en_para
{
    uint8_t os_test_en;
    uint8_t horse_wide_test_en;
    uint8_t standby_current_test_en;
    uint8_t work_current_test_en;
    uint8_t auto_cal_test_en;
    uint8_t efuse_test_en;
    uint8_t sub_bin_test_en;
}test_en_para_t;

typedef struct __voltage_para
{
    uint16_t default_voltage;
    uint16_t efuse_voltage;
}voltage_para_t;

typedef struct __current_para
{
    uint16_t standby_current_max;
    uint16_t work_current_max;
    uint16_t work_current_min;
}current_para_t;

typedef struct __test_para
{
    uint32_t test_para_head;
    test_signal_para_t test_signal_para;
    power_para_t power_para;
    freq_test_para_t freq_test_para;
    cal_para_t cal_para;
    efuse_para_t efuse_para;
    sub_bin_para_t sub_bin_para;
    chip_para_t chip_para;
    test_en_para_t test_en_para;
    voltage_para_t voltage_para;
    current_para_t current_para;
} test_para_t;

typedef struct __freq_data
{
    uint8_t status;
    uint8_t timer_updata_num;
    uint16_t timer_data;
    uint16_t need_num;
    uint16_t get_num;
} freq_data_t;

typedef struct __test_machine_data
{
    uint8_t test_start;
} test_machine_data_t;

typedef struct __power_data
{
    uint8_t status;
} power_data_t;

typedef struct __test_data
{
    freq_data_t freq_data;
    test_machine_data_t test_machine_data;
    power_data_t power_data;
    uint8_t test_mod;
} test_data_t;

typedef struct __chip_type_save{
    uint8_t chip_type;
    uint8_t horse_wide_type;
}chip_type_save_t;

typedef struct __freq_save{
    int32_t target_freq;
    int32_t freq_err;
    int32_t start_freq;
    int32_t end_freq;
    uint16_t code;
}freq_save_t;

typedef struct __voltage_save{
    uint16_t default_voltage;
    uint16_t efuse_voltage;
}voltage_save_t;

typedef struct __current_save{
    uint16_t standby_current;
    uint16_t work_current_k0;
    uint16_t work_current_k1;
    uint16_t work_current_k2;
    uint16_t work_current_k3;
    uint16_t work_current_k4;
    uint16_t work_current_k5;
    uint16_t work_current_k6;
    uint16_t work_current_k7;
    uint16_t work_current_k8;
    uint16_t work_current_k9;
}current_save_t;

typedef struct __os_save{
    uint16_t os_voltage_2;
    uint16_t os_voltage_3;
    uint16_t os_voltage_4;
    uint16_t os_voltage_5;
    uint16_t os_voltage_6;
    uint16_t os_voltage_7;
    uint16_t os_voltage_8;
    uint16_t os_voltage_9;
    uint16_t os_voltage_10;
    uint16_t os_voltage_11;
    uint16_t os_voltage_12;
    uint16_t os_voltage_13;
    uint16_t os_voltage_14;
}os_save_t;


typedef struct __test_save
{
    chip_type_save_t chip_type_save;
    freq_save_t freq_save;
    int32_t err_save;
    voltage_save_t voltage_save;
    current_save_t current_save;
    os_save_t os_save;
    uint8_t bin_save;
} test_save_t;

void test_para_init(void);
void test_para_restore(void);
void test_para_load(void);
void test_para_save(void);
void test_save_init(void);

extern test_para_t test_para;
extern test_data_t test_data;
extern test_save_t test_save;

#ifdef __cplusplus
}
#endif

#endif