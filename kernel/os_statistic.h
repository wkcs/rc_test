#ifndef __OS_STATISTIC_H__
#define __OS_STATISTIC_H__

#include "os_cpu.h"
#include "os_kernel.h"

/*根据任务控制块获取任务栈使用率*/
os_uint16 get_task_stack_usage_for_tcb(_task_tcb *task_tcb);

#endif

