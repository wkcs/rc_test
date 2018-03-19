#include "cmd.h"
#include "cmd_list.h"

static _cmd_list cmd_list[256];

/*初始化命令列表*/
int8_t cmd_init(void)
{
	cmd_test_para_init();
	cmd_auto_efuse_init();
	cmd_firmware_init();
	return 0;
}

int8_t add_cmd(uint8_t cmd_num, cmd_function cmd_main_function)
{
	if (cmd_list[cmd_num].cmd_num == 0) {
		cmd_list[cmd_num].cmd_num = cmd_num;
		cmd_list[cmd_num].cmd_main_function = cmd_main_function;
		return 0;
	} else
		return CMD_REPETITION;
}

int8_t cmd_run(uint8_t cmd_num, uint8_t *cmd_data)
{
	if (cmd_num == 0) {
		cmd_list[cmd_num].cmd_main_function(cmd_data);
		return 0;
	} else {
		if (cmd_list[cmd_num].cmd_num == 0)
			return CMD_NULL;
		else {
			cmd_list[cmd_num].cmd_main_function(cmd_data);
			return 0;
		}
	}
}

