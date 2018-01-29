#include "test_current.h"
#include "board.h"
#include "test_type.h"
#include "test_signal.h"

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
    printf("standby_current:%d\r\n", standby_current);
    if (standby_current > test_para.current_para.standby_current_max)
        return -STANDBY_CURRENT_ERR;
    return 0;
}

int32_t work_current_test(void)
{
    uint16_t work_current, work_current2, work_current3, work_current4, work_current5;
    uint16_t work_current6, work_current7, work_current8, work_current9, work_current10;
    uint16_t work_current11, work_current12, work_current13, work_current14, work_current15;
    uint16_t work_current16, work_current17, work_current18, work_current19, work_current20;
    if (test_data.power_data.status == POWER_OFF)
    {
        power_on(test_para.power_para.open_time);
    }
    if (test_data.test_mod != NORMAL_MODE)
        exit_test();

    K0 = 1;
    //work_current = get_socket_current(20);
    work_current = get_socket_current_one();
    work_current2 = get_socket_current_one();
    work_current3 = get_socket_current_one();
    work_current4 = get_socket_current_one();
    work_current5 = get_socket_current_one();
    work_current6 = get_socket_current_one();
    work_current7 = get_socket_current_one();
    work_current8 = get_socket_current_one();
    work_current9 = get_socket_current_one();
    work_current10 = get_socket_current_one();
    work_current11 = get_socket_current_one();
    work_current12 = get_socket_current_one();
    work_current13 = get_socket_current_one();
    work_current14 = get_socket_current_one();
    work_current15 = get_socket_current_one();
    work_current16 = get_socket_current_one();
    work_current17 = get_socket_current_one();
    work_current18 = get_socket_current_one();
    work_current19 = get_socket_current_one();
    work_current20 = get_socket_current_one();
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current6, work_current7, work_current8, work_current9, work_current10);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current11, work_current12, work_current13, work_current14, work_current15);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current16, work_current17, work_current18, work_current19, work_current20);
    test_save.current_save.work_current_k0 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K0 = 0;
        return -WORK_CURRENT_K0_ERR;
    }
    K0 = 0;

    K1 = 1;
    //work_current = get_socket_current(20);
    work_current = get_socket_current_one();
    work_current2 = get_socket_current_one();
    work_current3 = get_socket_current_one();
    work_current4 = get_socket_current_one();
    work_current5 = get_socket_current_one();
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
    test_save.current_save.work_current_k1 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K1 = 0;
        return -WORK_CURRENT_K1_ERR;
    }
    K1 = 0;

    K2 = 1;
    //work_current = get_socket_current(20);
    work_current = get_socket_current_one();
    work_current2 = get_socket_current_one();
    work_current3 = get_socket_current_one();
    work_current4 = get_socket_current_one();
    work_current5 = get_socket_current_one();
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
    test_save.current_save.work_current_k2 = work_current;
    if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
    {
        K2 = 0;
        return -WORK_CURRENT_K2_ERR;
    }
    K2 = 0;

    K3 = 1;
    //work_current = get_socket_current(20);
    work_current = get_socket_current_one();
    work_current2 = get_socket_current_one();
    work_current3 = get_socket_current_one();
    work_current4 = get_socket_current_one();
    work_current5 = get_socket_current_one();
    work_current6 = get_socket_current_one();
    work_current7 = get_socket_current_one();
    work_current8 = get_socket_current_one();
    work_current9 = get_socket_current_one();
    work_current10 = get_socket_current_one();
    work_current11 = get_socket_current_one();
    work_current12 = get_socket_current_one();
    work_current13 = get_socket_current_one();
    work_current14 = get_socket_current_one();
    work_current15 = get_socket_current_one();
    work_current16 = get_socket_current_one();
    work_current17 = get_socket_current_one();
    work_current18 = get_socket_current_one();
    work_current19 = get_socket_current_one();
    work_current20 = get_socket_current_one();
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current6, work_current7, work_current8, work_current9, work_current10);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current11, work_current12, work_current13, work_current14, work_current15);
    printf("work_current:%d, %d, %d, %d, %d\r\n", work_current16, work_current17, work_current18, work_current19, work_current20);
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
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
        test_save.current_save.work_current_k4 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K4 = 0;
            return -WORK_CURRENT_K4_ERR;
        }
        K4 = 0;

        K5 = 1;
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
        test_save.current_save.work_current_k5 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K5 = 0;
            return -WORK_CURRENT_K5_ERR;
        }
        K5 = 0;

        K6 = 1;
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
        test_save.current_save.work_current_k6 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K6 = 0;
            return -WORK_CURRENT_K6_ERR;
        }
        K6 = 0;

        K7 = 1;
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
        test_save.current_save.work_current_k7 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K7 = 0;
            return -WORK_CURRENT_K7_ERR;
        }
        K7 = 0;

        K8 = 1;
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
        test_save.current_save.work_current_k8 = work_current;
        if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max)
        {
            K8 = 0;
            return -WORK_CURRENT_K8_ERR;
        }
        K8 = 0;

        K9 = 1;
        //work_current = get_socket_current(20);
        work_current = get_socket_current_one();
        work_current2 = get_socket_current_one();
        work_current3 = get_socket_current_one();
        work_current4 = get_socket_current_one();
        work_current5 = get_socket_current_one();
        printf("work_current:%d, %d, %d, %d, %d\r\n", work_current, work_current2, work_current3, work_current4, work_current5);
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