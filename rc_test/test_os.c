#include "test_os.h"
#include "test_type.h"
#include "rc_debug.h"
#include "test_signal.h"

#define OS_VOL_LOW 500      /*OS最低电压*/
#define OS_VOL_HIGH 1000     /*OS最高电压*/

/*将模拟开关切换至os测试*/
static inline void entry_os_test()
{
	OS_TEST_1 = ENABLE;
	OS_TEST_3 = ENABLE;
	OS_TEST_4 = ENABLE;
	OS_TEST_5 = ENABLE;
	OS_TEST_6 = ENABLE;
	OS_TEST_7 = ENABLE;
	OS_TEST_8 = ENABLE;
	OS_TEST_9 = ENABLE;
	OS_TEST_10 = ENABLE;
	OS_TEST_11 = ENABLE;
	OS_TEST_12 = ENABLE;
	OS_TEST_13 = ENABLE;
}

/*将模拟开关切换至正常工作状态*/
static inline void exit_os_test()
{
	OS_TEST_1 = DISABLE;
	OS_TEST_3 = DISABLE;
	OS_TEST_4 = DISABLE;
	OS_TEST_5 = DISABLE;
	OS_TEST_6 = DISABLE;
	OS_TEST_7 = DISABLE;
	OS_TEST_8 = DISABLE;
	OS_TEST_9 = DISABLE;
	OS_TEST_10 = DISABLE;
	OS_TEST_11 = DISABLE;
	OS_TEST_12 = DISABLE;
	OS_TEST_13 = DISABLE;
}

int32_t os_test(void)
{
	int32_t err = 0;
	uint16_t *buf;
	entry_os_test();
	/*开始进行测试*/
	buf = get_os_adc();
	if (buf == 0)
		err = -OS_ADC_ERR;
	else {
		test_save.os_save.os_voltage_1 = (uint16_t)((float)(buf[8] + buf[20] + buf[32]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
		if (test_para.debug_para.debug_info_en)
			rc_printf("os_voltage_1:%d mV\r\n", test_save.os_save.os_voltage_1);
		if (test_save.os_save.os_voltage_1 < OS_VOL_LOW || test_save.os_save.os_voltage_1 > OS_VOL_HIGH) {
			exit_os_test();
			return -OS_ERR_1;
		}

		test_save.os_save.os_voltage_3 = (uint16_t)((float)(buf[11] + buf[23] + buf[35]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
		if (test_para.debug_para.debug_info_en)
			rc_printf("os_voltage_3:%d mV\r\n", test_save.os_save.os_voltage_3);
		if (test_save.os_save.os_voltage_3 < OS_VOL_LOW || test_save.os_save.os_voltage_3 > OS_VOL_HIGH) {
			exit_os_test();
			return -OS_ERR_3;
		}

		test_save.os_save.os_voltage_4 = (uint16_t)((float)(buf[0] + buf[12] + buf[24]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
		if (test_para.debug_para.debug_info_en)
			rc_printf("os_voltage_4:%d mV\r\n", test_save.os_save.os_voltage_4);
		if (test_save.os_save.os_voltage_4 < OS_VOL_LOW || test_save.os_save.os_voltage_4 > OS_VOL_HIGH) {
			exit_os_test();
			return -OS_ERR_4;
		}
		if (test_para.chip_para.chip_type == RC119_433 || test_para.chip_para.chip_type == RC119_315) {
			test_save.os_save.os_voltage_5 = (uint16_t)((float)(buf[7] + buf[19] + buf[31]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_5:%d mV\r\n", test_save.os_save.os_voltage_5);
			if (test_save.os_save.os_voltage_5 < OS_VOL_LOW || test_save.os_save.os_voltage_5 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_5;
			}

			test_save.os_save.os_voltage_6 = (uint16_t)((float)(buf[5] + buf[17] + buf[29]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_6:%d mV\r\n", test_save.os_save.os_voltage_6);
			if (test_save.os_save.os_voltage_6 < OS_VOL_LOW || test_save.os_save.os_voltage_6 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_6;
			}

			test_save.os_save.os_voltage_7 = (uint16_t)((float)(buf[6] + buf[18] + buf[30]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_7:%d mV\r\n", test_save.os_save.os_voltage_7);
			if (test_save.os_save.os_voltage_7 < OS_VOL_LOW || test_save.os_save.os_voltage_7 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_7;
			}

			test_save.os_save.os_voltage_8 = (uint16_t)((float)(buf[4] + buf[16] + buf[28]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_8:%d mV\r\n", test_save.os_save.os_voltage_8);
			if (test_save.os_save.os_voltage_8 < OS_VOL_LOW || test_save.os_save.os_voltage_8 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_8;
			}

			test_save.os_save.os_voltage_9 = (uint16_t)((float)(buf[10] + buf[12] + buf[24]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_9:%d mV\r\n", test_save.os_save.os_voltage_9);
			if (test_save.os_save.os_voltage_9 < OS_VOL_LOW || test_save.os_save.os_voltage_9 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_9;
			}

			test_save.os_save.os_voltage_10 = (uint16_t)((float)(buf[3] + buf[15] + buf[27]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_10:%d mV\r\n", test_save.os_save.os_voltage_10);
			if (test_save.os_save.os_voltage_10 < OS_VOL_LOW || test_save.os_save.os_voltage_10 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_10;
			}

			test_save.os_save.os_voltage_11 = (uint16_t)((float)(buf[2] + buf[14] + buf[26]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_11:%d mV\r\n", test_save.os_save.os_voltage_11);
			if (test_save.os_save.os_voltage_11 < OS_VOL_LOW || test_save.os_save.os_voltage_11 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_11;
			}

			test_save.os_save.os_voltage_12 = (uint16_t)((float)(buf[10] + buf[22] + buf[34]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_12:%d mV\r\n", test_save.os_save.os_voltage_12);
			if (test_save.os_save.os_voltage_12 < OS_VOL_LOW || test_save.os_save.os_voltage_12 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_12;
			}

			test_save.os_save.os_voltage_13 = (uint16_t)((float)(buf[3] + buf[15] + buf[27]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_13:%d mV\r\n", test_save.os_save.os_voltage_13);
			if (test_save.os_save.os_voltage_13 < OS_VOL_LOW || test_save.os_save.os_voltage_13 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_13;
			}
		} else {
			test_save.os_save.os_voltage_5 = (uint16_t)((float)(buf[2] + buf[14] + buf[26]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_5:%d mV\r\n", test_save.os_save.os_voltage_5);
			if (test_save.os_save.os_voltage_5 < OS_VOL_LOW || test_save.os_save.os_voltage_5 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_5;
			}
			test_save.os_save.os_voltage_6 = (uint16_t)((float)(buf[10] + buf[22] + buf[34]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_6:%d mV\r\n", test_save.os_save.os_voltage_6);
			if (test_save.os_save.os_voltage_6 < OS_VOL_LOW || test_save.os_save.os_voltage_6 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_6;
			}
			test_save.os_save.os_voltage_7 = (uint16_t)((float)(buf[3] + buf[15] + buf[27]) * 0.2711159);   //0.2711159 = (3332 / 4096 / 3)
			if (test_para.debug_para.debug_info_en)
				rc_printf("os_voltage_7:%d mV\r\n", test_save.os_save.os_voltage_7);
			if (test_save.os_save.os_voltage_7 < OS_VOL_LOW || test_save.os_save.os_voltage_7 > OS_VOL_HIGH) {
				exit_os_test();
				return -OS_ERR_7;
			}
		}
	}
	exit_os_test();
	return err;
}

