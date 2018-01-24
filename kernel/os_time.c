#include "os_time.h"
#include "os_task.h"
#include "os_define.h"

/*
 * 挂起当前任务，延时一段时间再恢复
 */
void os_delay(os_uint32 delay_time_slice)
{
    os_int8 i;
    os_run_prio_tcb -> task_delay_time_over = delay_time_slice;
    i = hang_task_for_tcb(os_run_prio_tcb, WAIT_DELAY, NOT_IRQ);
    if (i != 0) {
        printf("error:%d\r\n", i);
        while(1);
    }
}

