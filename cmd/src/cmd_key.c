#include "test_type.h"
#include "cmd_list.h"
#include "cmd.h"
#include "freq.h"
#include "rc_debug.h"
#include "rc_message.h"
#include "delay.h"

static void cmd_main(uint8_t *buf)
{
	int32_t freq;
	//uint8_t mes_buf[4];

	if (test_data.power_data.status == POWER_OFF)
		power_on(test_para.power_para.open_time);

	if (test_data.test_mod != NORMAL_MODE)
		exit_test();

	switch(buf[0]) {
	case 0:
		K0 = 1;
		break;
	case 1:
		K0 = 1;
		break;
	case 2:
		K0 = 1;
		break;
	case 3:
		K0 = 1;
		break;
	case 4:
		K0 = 1;
		break;
	case 5:
		K0 = 1;
		break;
	case 6:
		K0 = 1;
		break;
	case 7:
		K0 = 1;
		break;
	case 8:
		K0 = 1;
		break;
	case 9:
		K0 = 1;
		break;
	default:
		power_off(test_para.power_para.clos_time);
		return;
	}

	delay_ms(10);

	freq = get_freq(1000);

	switch(buf[0]) {
	case 0:
		K0 = 0;
		break;
	case 1:
		K0 = 0;
		break;
	case 2:
		K0 = 0;
		break;
	case 3:
		K0 = 0;
		break;
	case 4:
		K0 = 0;
		break;
	case 5:
		K0 = 0;
		break;
	case 6:
		K0 = 0;
		break;
	case 7:
		K0 = 0;
		break;
	case 8:
		K0 = 0;
		break;
	case 9:
		K0 = 0;
		break;
	default:
		power_off(test_para.power_para.clos_time);
		return;
	}
	rc_debug_mes_save_enable();
	if (freq < 0) {
		if (test_para.debug_para.debug_info_en)
			rc_printf("error:%s\r\n", get_err_name(freq));
	} else {
		if (test_para.debug_para.debug_info_en)
			rc_printf("freq:%d Hz\r\n", freq);
	}
	rc_debug_mes_save_disable();
	power_off(test_para.power_para.clos_time);
	return;
}

__attribute__((section("cmd_list"))) cmd_list_t cmd_key = {
	CMD_KEY,
	cmd_main
};



