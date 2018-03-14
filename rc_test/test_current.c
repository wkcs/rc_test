#include "test_current.h"
#include "board.h"
#include "test_type.h"
#include "test_signal.h"
#include "rc_debug.h"

#define SC_TEST_NUM 100        /*待机电流采样次数*/
#define WC_TEST_NUM 100        /*工作电流采样次数*/

int32_t get_standby_current(void)
{
	uint16_t i, *buf, temp;
	uint32_t adc_sum = 0;      /*ADC采样结果的总和*/
	CURRENT_TETS_H = DISABLE;
	CURRENT_TETS_L = ENABLE;
	delay_ms(10);
	buf = get_standby_current_adc(SC_TEST_NUM);
	CURRENT_TETS_L = DISABLE;
	if (buf == 0)
		return -STANDBY_CURRENT_ADC_ERR;
	for (i = 0; i < SC_TEST_NUM; i++) {
		adc_sum += buf[i];
	}
	temp = (uint16_t)((((float)adc_sum / SC_TEST_NUM) * 3250 / 4096 - 765 ) / 10 - 3);    /*结果为uA*/
	return (int32_t)temp;
}

int32_t get_work_current(void)
{
	uint16_t i, m = 0, *buf, temp;
	uint16_t adc_max[3] = {0, 0, 0};
	uint32_t adc_sum = 0;      /*ADC采样结果的总和*/
try_again:
	OS_TEST_3 = ENABLE;
	CURRENT_TETS_H = ENABLE;
	CURRENT_TETS_L = DISABLE;
    delay_us(100);
	buf = get_work_current_adc(WC_TEST_NUM);
	CURRENT_TETS_H = DISABLE;
	OS_TEST_3 = DISABLE;
	if (buf == 0)
		return -WORK_CURRENT_ADC_ERR;
	for (i = 0; i < WC_TEST_NUM; i++) {
		if (buf[i] < 4050) {
			if (buf[i] > adc_max[0])
				adc_max[0] = buf[i];
			else if (buf[i] > adc_max[1])
				adc_max[1] = buf[i];
			else if (buf[i] > adc_max[2])
				adc_max[2] = buf[i];
		}
	}
    if (adc_max[0] < 4000 && m < 20) {
        m++;
        goto try_again;
    }
	if (adc_max[1] == 0)
		adc_max[1] = adc_max[0];
	if (adc_max[2] == 0)
		adc_max[2] = adc_max[2];
	//temp = (uint16_t)((((float)adc_sum / WC_TEST_NUM) * 3250 / 4096 - 11) * 10);    /*结果为uA*/
	temp = (uint16_t)((((float)(adc_max[0] + adc_max[1] + adc_max[2]) / 3 * 0.38) * 3250 / 4096) * 10);    /*结果为uA*/
	return (int32_t)temp;
}

int32_t standby_current_test(void)
{
	int32_t err;
	uint16_t standby_current;
	/*检查芯片供电是否打开*/
	if (test_data.power_data.status == POWER_OFF) {
		power_on(test_para.power_para.open_time);
	}
	/*检查芯片是否处于测试模式，如果在测试模式则需要退出*/
	if (test_data.test_mod != NORMAL_MODE)
		exit_test();
	err = get_standby_current();
	if (err < 0)
		return err;
	standby_current = (uint16_t)err;
	test_save.current_save.standby_current = standby_current;
	if (test_para.debug_para.debug_info_en)
		rc_printf("standby_current:%d uA\r\n", standby_current);
	if (standby_current > test_para.current_para.standby_current_max)
		return -STANDBY_CURRENT_ERR;
	return 0;
}

int32_t work_current_test(void)
{
	int32_t err;
	uint16_t work_current;
	/*检查芯片供电是否打开*/
	if (test_data.power_data.status == POWER_OFF) {
		power_on(test_para.power_para.open_time);
	}
	/*检查芯片是否处于测试模式，如果在测试模式则需要退出*/
	if (test_data.test_mod != NORMAL_MODE)
		exit_test();

	/*
	 * 检查每个按键按下后的工作电流，
	 *
	 */
	K0 = 1;
	err = get_work_current();
	if (err < 0)
		return err;
	work_current = (uint16_t)err;
	if (test_para.debug_para.debug_info_en)
		rc_printf("work_current_k0:%d uA\r\n", work_current);
	test_save.current_save.work_current_k0 = work_current;
	if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
		K0 = 0;
		return -WORK_CURRENT_K0_ERR;
	}
	K0 = 0;

	K1 = 1;
	err = get_work_current();
	if (err < 0)
		return err;
	work_current = (uint16_t)err;
	if (test_para.debug_para.debug_info_en)
		rc_printf("work_current_k1:%d uA\r\n", work_current);
	test_save.current_save.work_current_k1 = work_current;
	if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
		K1 = 0;
		return -WORK_CURRENT_K1_ERR;
	}
	K1 = 0;

	K2 = 1;
	err = get_work_current();
	if (err < 0)
		return err;
	work_current = (uint16_t)err;
	if (test_para.debug_para.debug_info_en)
		rc_printf("work_current_k2:%d uA\r\n", work_current);
	test_save.current_save.work_current_k2 = work_current;
	if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
		K2 = 0;
		return -WORK_CURRENT_K2_ERR;
	}
	K2 = 0;

	K3 = 1;
	err = get_work_current();
	if (err < 0)
		return err;
	work_current = (uint16_t)err;
	if (test_para.debug_para.debug_info_en)
		rc_printf("work_current_k3:%d uA\r\n", work_current);
	test_save.current_save.work_current_k3 = work_current;
	if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
		K3 = 0;
		return -WORK_CURRENT_K3_ERR;
	}
	K3 = 0;

	if (test_para.chip_para.chip_type == RC119_433 || test_para.chip_para.chip_type == RC119_315) {
		K4 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k4:%d uA\r\n", work_current);
		test_save.current_save.work_current_k4 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K4 = 0;
			return -WORK_CURRENT_K4_ERR;
		}
		K4 = 0;

		K5 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k5:%d uA\r\n", work_current);
		test_save.current_save.work_current_k5 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K5 = 0;
			return -WORK_CURRENT_K5_ERR;
		}
		K5 = 0;

		K6 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k6:%d uA\r\n", work_current);
		test_save.current_save.work_current_k6 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K6 = 0;
			return -WORK_CURRENT_K6_ERR;
		}
		K6 = 0;

		K7 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k7:%d uA\r\n", work_current);
		test_save.current_save.work_current_k7 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K7 = 0;
			return -WORK_CURRENT_K7_ERR;
		}
		K7 = 0;

		K8 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k8:%d uA\r\n", work_current);
		test_save.current_save.work_current_k8 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K8 = 0;
			return -WORK_CURRENT_K8_ERR;
		}
		K8 = 0;

		K9 = 1;
		err = get_work_current();
		if (err < 0)
			return err;
		work_current = (uint16_t)err;
		if (test_para.debug_para.debug_info_en)
			rc_printf("work_current_k9:%d uA\r\n", work_current);
		test_save.current_save.work_current_k9 = work_current;
		if (work_current < test_para.current_para.work_current_min || work_current > test_para.current_para.work_current_max) {
			K9 = 0;
			return -WORK_CURRENT_K7_ERR;
		}
		K9 = 0;
	}

	return 0;
}