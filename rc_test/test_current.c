#include "test_current.h"
#include "board.h"
#include "test_type.h"
#include "test_signal.h"
#include "rc_debug.h"

int32_t standby_current_test(void)
{
    uint16_t standby_current;
    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();
    standby_current = get_socket_current(100);
    test_save.current_save.standby_current = standby_current;
    if (test_para.debug_para.debug_info_en)
        rc_printf("standby_current:%d\r\n", standby_current);
    if (standby_current > test_para.current_para.standby_current_max)
        return -STANDBY_CURRENT_ERR;
    return 0;
}

int32_t work_current_test(void)
{
    uint16_t work_current;
    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();

    K0 = 1;
    work_current = get_socket_current(20);
    if (test_para.debug_para.debug_info_en)
        rc_printf("work_current_k0:%d\r\n", work_current);
    test_save.current_save.work_current_k0 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K0 = 0;
        return -WORK_CURRENT_K0_ERR;
    }
    K0 = 0;

    K1 = 1;
    work_current = get_socket_current(20);
    if (test_para.debug_para.debug_info_en)
        rc_printf("work_current_k1:%d\r\n", work_current);
    test_save.current_save.work_current_k1 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K1 = 0;
        return -WORK_CURRENT_K1_ERR;
    }
    K1 = 0;

    K2 = 1;
    work_current = get_socket_current(20);
    if (test_para.debug_para.debug_info_en)
        rc_printf("work_current_k2:%d\r\n", work_current);
    test_save.current_save.work_current_k2 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K2 = 0;
        return -WORK_CURRENT_K2_ERR;
    }
    K2 = 0;

    K3 = 1;
    work_current = get_socket_current(20);
    if (test_para.debug_para.debug_info_en)
        rc_printf("work_current_k3:%d\r\n", work_current);
    test_save.current_save.work_current_k3 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K3 = 0;
        return -WORK_CURRENT_K3_ERR;
    }
    K3 = 0;

    if (test_para.chip_para.chip_type == RC119_433 || test_para.chip_para.chip_type == RC119_315)
    {
        K4 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k4:%d\r\n", work_current);
        test_save.current_save.work_current_k4 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K4 = 0;
            return -WORK_CURRENT_K4_ERR;
        }
        K4 = 0;

        K5 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k5:%d\r\n", work_current);
        test_save.current_save.work_current_k5 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K5 = 0;
            return -WORK_CURRENT_K5_ERR;
        }
        K5 = 0;

        K6 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k6:%d\r\n", work_current);
        test_save.current_save.work_current_k6 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K6 = 0;
            return -WORK_CURRENT_K6_ERR;
        }
        K6 = 0;

        K7 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k7:%d\r\n", work_current);
        test_save.current_save.work_current_k7 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K7 = 0;
            return -WORK_CURRENT_K7_ERR;
        }
        K7 = 0;

        K8 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k8:%d\r\n", work_current);
        test_save.current_save.work_current_k8 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K8 = 0;
            return -WORK_CURRENT_K8_ERR;
        }
        K8 = 0;

        K9 = 1;
        work_current = get_socket_current(20);
        if (test_para.debug_para.debug_info_en)
            rc_printf("work_current_k9:%d\r\n", work_current);
        test_save.current_save.work_current_k9 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K9 = 0;
            return -WORK_CURRENT_K7_ERR;
        }
        K9 = 0;
    }

    return 0;
}