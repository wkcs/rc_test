#include "os_kernel.h"
#include "os_define.h"
#include "os_mem.h"
#include "os_task.h"
#include "os_sys_task.h"
#include "os_config.h"
#include "os_cpu.h"
#include "os_time.h"

/*总任务列表*/
_task_tcb *task_tcb_list_head = NULL, *task_tcb_list_end = NULL;
/*就绪任务列表*/
_ready_task_list *ready_task_list_head = NULL, *ready_task_list_end = NULL;
_ready_task_list *ready_task_list_high_prio = NULL;     /*指向就绪任务列表第一个非空表*/
/*挂起任务列表*/
_hang_task_list *hang_task_list_head = NULL, *hang_task_list_end = NULL;
/*睡眠任务列表*/
_sleep_task_list *sleep_task_list_head = NULL, *sleep_task_list_end = NULL;
/*时间相关挂起任务列表*/
_wait_time_task_list *wait_time_task_list_head = NULL, *wait_time_task_list_end = NULL;

/*系统最高优先级就绪任务任务控制块和当前运行任务任务控制块*/
_task_tcb *os_high_prio_tcb = NULL, *os_run_prio_tcb = NULL;

/*挂起任务检查任务的任务控制块*/
_task_tcb *sys_hang_check_task_tcb = NULL;
/*空闲任务的任务控制块*/
_task_tcb *idle_task_tcb = NULL;
/*统计任务的任务控制块*/
_task_tcb *os_statistics_task_tcb = NULL;

/*系统运行标志*/
os_uint8 os_running = 0;

/*系统统计*/
_os_statistical_struct os_statistical_struct;

/*
 * 初始化系统内核
 */
os_int8 kernel_init(void)
{
    os_int8 status;
    mem_init();
    status = task_init();
    if (status != 0)
        return status;
    return 0;
}

/*
 * 初始化系统内核
 */
void kernel_start(void)
{
    os_high_prio_tcb -> task_status = TASK_RUNNING;
    os_start_hign_prio_task();
}

