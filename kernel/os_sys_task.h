#ifndef __OS_SYS_TASK_H__
#define __OS_SYS_TASK_H__

#include "os_cpu.h"

extern os_uint8 *idle_task_name;
extern os_stk idle_task_stack[];
extern os_uint8 *sys_hang_check_task_name;
extern os_stk sys_hang_check_task_stack[];
extern os_uint8 *os_statistics_task_name;
extern os_stk os_statistics_task_stack[];
extern os_uint16 os_statistics_task_delay_num;

void idle_task(void *pdata);
void sys_hang_check_task(void *pdata);
void os_statistics_task(void *pdata);

#endif

