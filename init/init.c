#include "board.h"
#include "test_type.h"
#include "rc11x_test.h"
#include "print_para.h"
#include "test_com.h"
#include "cmd.h"
#include "rc_debug.h"
#include "power.h"
#include "usbd_cdc_vcp.h"
#include "rc_message.h"

int rc_test_loop(void)
{
	uint8_t bin;
	uint32_t num = 0;
	
	rc11x_test_v3_board_init();
	test_para_init();
	print_test_para();
	cmd_init();

	while(1) {
		if (test_data.test_machine_data.test_start) {
			if (test_para.debug_para.debug_info_en)
				rc_printf("******************RC11X_TEST*********************\r\n");
			bin = rc11x_test_start();
			if (test_save.err_save < 0) {
				if (test_para.debug_para.debug_info_en)
					rc_printf("error:%s\r\n", get_err_name(test_save.err_save));
			} else {
				if (test_para.debug_para.debug_info_en) {
					rc_printf("freq:%d Hz\r\n", test_save.freq_save.end_freq);
					rc_printf("freq_err:%d Hz\r\n", test_save.freq_save.freq_err);
					rc_printf("code:0x%03x\r\n", test_save.freq_save.code);
				}
			}
			if (test_para.debug_para.debug_info_en) {
				rc_printf("bin:Bin%d\r\n\r\n", bin);
			}
			test_mach_send_end_sig(bin);
			send_test_results();
			test_data.test_machine_data.test_start = DISABLE;
		} else if (uart1_data.rx_status == 1) {
			cmd_run(uart1_data.rx_buf[0], uart1_data.rx_buf);
			uart1_data.rx_status = 0;
			uart1_data.rx_len = 0;
		} else {
			if (num == 1000000) {
				rc_send_message(0, 0, NULL_MES);
				num = 0;
			} else
				num++;
		}
	}
}

