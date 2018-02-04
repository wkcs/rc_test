#ifndef __RC_ERR_H__
#define __RC_ERR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "rc_math.h"

#define ERR_NAME_CASE(x) case x: return (#x)

#define RC_ERR_NO_MAX 29

enum FREQ_ERR
{
    NO_FREQ = 1,
    TARGET_FREQ_TOO_LOW,
    TARGET_FREQ_TOO_HIGH
};

enum OS_ERR
{
    OS_ERR_2 = TARGET_FREQ_TOO_HIGH + 1,
    OS_ERR_3,
    OS_ERR_4,
    OS_ERR_5,
    OS_ERR_6,
    OS_ERR_7,
    OS_ERR_8,
    OS_ERR_9,
    OS_ERR_10,
    OS_ERR_11,
    OS_ERR_12,
    OS_ERR_13,
    OS_ERR_14,
};

enum CHIP_TYPE_ERR
{
    CODE_WIDTH_ERR = OS_ERR_14 + 1,
};

enum CURRENT_ERR
{
    STANDBY_CURRENT_ERR = CODE_WIDTH_ERR + 1,
    WORK_CURRENT_K0_ERR,
    WORK_CURRENT_K1_ERR,
    WORK_CURRENT_K2_ERR,
    WORK_CURRENT_K3_ERR,
    WORK_CURRENT_K4_ERR,
    WORK_CURRENT_K5_ERR,
    WORK_CURRENT_K6_ERR,
    WORK_CURRENT_K7_ERR,
    WORK_CURRENT_K8_ERR,
    WORK_CURRENT_K9_ERR
};

enum BIN_ERR
{
    NOT_FIND_BIN = WORK_CURRENT_K9_ERR + 1
};

inline const char *get_err_name(int32_t err)
{
    switch (RC_ABS(err))
    {
        case 0: return "NO_ERR";
        /*FREQ_ERR*/
        ERR_NAME_CASE(NO_FREQ);
        ERR_NAME_CASE(TARGET_FREQ_TOO_LOW);
        ERR_NAME_CASE(TARGET_FREQ_TOO_HIGH);
        /*OS_ERR*/
        ERR_NAME_CASE(OS_ERR_2);
        ERR_NAME_CASE(OS_ERR_3);
        ERR_NAME_CASE(OS_ERR_4);
        ERR_NAME_CASE(OS_ERR_5);
        ERR_NAME_CASE(OS_ERR_6);
        ERR_NAME_CASE(OS_ERR_7);
        ERR_NAME_CASE(OS_ERR_8);
        ERR_NAME_CASE(OS_ERR_9);
        ERR_NAME_CASE(OS_ERR_10);
        ERR_NAME_CASE(OS_ERR_11);
        ERR_NAME_CASE(OS_ERR_12);
        ERR_NAME_CASE(OS_ERR_13);
        ERR_NAME_CASE(OS_ERR_14);
        /*CHIP_TYPE_ERR*/
        ERR_NAME_CASE(CODE_WIDTH_ERR);
        /*CURRENT_ERR*/
        ERR_NAME_CASE(STANDBY_CURRENT_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K0_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K1_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K2_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K3_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K4_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K5_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K6_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K7_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K8_ERR);
        ERR_NAME_CASE(WORK_CURRENT_K9_ERR);
        /*BIN_ERR*/
        ERR_NAME_CASE(NOT_FIND_BIN);
    }
    return "unknown err";
}

#ifdef __cplusplus
}
#endif

#endif