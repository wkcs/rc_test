#include "os_statistic.h"

/*
 * 根据任务控制块获取任务栈使用率
 */
os_uint16 get_task_stack_usage_for_tcb(_task_tcb *task_tcb)
{
    os_stk stack_start_addr, stack_now_addr;
    os_uint16 stack_usage;
    stack_start_addr = (os_stk)task_tcb -> task_stack_start_addr;
    stack_now_addr = (os_stk)task_tcb -> task_stack_pointer;
    stack_usage = (stack_start_addr - stack_now_addr) * 10000 / task_tcb -> task_stack_size;
    return stack_usage;
}

