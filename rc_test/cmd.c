#include "cmd.h"
#include "cmd_list.h"

extern uint32_t _cmd_list_start;
extern uint32_t _cmd_list_end;


int8_t cmd_run(uint8_t cmd_num, uint8_t *cmd_data)
{
	uint32_t cmd_list_addr;
	cmd_list_addr = &_cmd_list_start;
	while(cmd_list_addr < &_cmd_list_end) {
		if (((cmd_list_t *) cmd_list_addr) -> cmd_num == cmd_num) {
			((cmd_list_t *) cmd_list_addr) -> cmd_main_function(cmd_data);
			return 0;
		}
		cmd_list_addr += 8;
	}
	return CMD_NULL;
}


