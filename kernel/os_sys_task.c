#include "os_sys_task.h"
#include "os_kernel.h"
#include "os_define.h"
#include "os_task.h"
#include "os_config.h"
#include "os_time.h"

os_uint8 *idle_task_name = "idle_task";
os_stk idle_task_stack[IDLE_TASK_STACK_SIZE];
void idle_task(void *pdata)
{
    while (1)
    {
        //os_statistical_struct.os_cpu_use_num++;
    }
}

os_uint8 *sys_hang_check_task_name = "syc_task";
os_stk sys_hang_check_task_stack[SYS_HANG_CHECK_TASK_STACK_SIZE];
void sys_hang_check_task(void *pdata)
{
    _task_tcb *task_tcb;
    _hang_task_list *hang_task_list;
    while (1)
    {
        enter_critical();
        hang_task_list = hang_task_list_head;
        while (hang_task_list != NULL)
        {
            if (hang_task_list->task_wait_flag != 0)
            {
                if (hang_task_list->task_wait_flag & WAIT_DELAY)
                {
                    if (hang_task_list->task_delay_time_over > 0)
                    {
                        hang_task_list->task_delay_time_over--;
                        if (hang_task_list->task_delay_time_over == 0)
                        {
                            task_tcb = hang_task_list;
                            hang_task_list = hang_task_list -> hang_task_next;
                            restore_task_for_tcb(task_tcb, WAIT_DELAY, IS_IRQ); /*此处不进行任务切换*/
                        }
                        else
                        {
                            hang_task_list = hang_task_list->hang_task_next;
                        }
                    }
                    else
                    {
                        task_tcb = hang_task_list;
                        hang_task_list = hang_task_list->hang_task_next;
                        restore_task_for_tcb(task_tcb, WAIT_DELAY, IS_IRQ); /*此处不进行任务切换*/
                    }
                }
                else
                {
                    hang_task_list = hang_task_list->hang_task_next;
                }
            }
            else
            {
                hang_task_list = hang_task_list->hang_task_next;
            }
        }
        exit_critical();
        hang_task_for_tcb(sys_hang_check_task_tcb, NO_WAIT, NOT_IRQ); /*挂起当前任务，并切换到最高优先级就绪任务*/
    }
}

os_uint8 *os_statistics_task_name = "ost_task";
os_stk os_statistics_task_stack[OS_STATISTICS_TASK_STACK_SIZE];
os_uint16 os_statistics_task_delay_num = 0;
void os_statistics_task(void *pdata)
{
    _task_tcb *task_tcb_list;
    while (1) {
        enter_critical();
        task_tcb_list = task_tcb_list_head;
        while (task_tcb_list != NULL) {
            /*if (task_tcb_list != os_statistics_task_tcb) {
                switch (task_tcb_list -> task_list) {
                    case (ALL_TASK_LIST | READY_TASK_LIST): task_tcb_list -> task_status = TASK_READY; break;
                    case (ALL_TASK_LIST | HANG_TASK_LIST): task_tcb_list -> task_status = TASK_HANG; break;
                    case (ALL_TASK_LIST | SLEEP_TASK_LIST): task_tcb_list -> task_status = TASK_SLEEP; break;
                    case (ALL_TASK_LIST | WAIT_TIME_TASK_LIST): task_tcb_list -> task_status = TASK_HANG_TIME; break;
                }
            }*/
            task_tcb_list -> task_cpu_usage = task_tcb_list->task_cpu_use_num * 10;
            task_tcb_list->task_cpu_use_num = 0;
            task_tcb_list = task_tcb_list -> next;
        }
        /*ready_task_list_list = ready_task_list_head;
        while (ready_task_list_list != NULL)
        {
            if (ready_task_list_list->task_tcb_cycle_list_head != NULL)
            {
                task_tcb_cycle_list_head = ready_task_list_list->task_tcb_cycle_list_head;
                task_tcb_cycle_list_list = ready_task_list_list->task_tcb_cycle_list_head;
                task_tcb_cycle_list_head->task_cpu_usage = task_tcb_cycle_list_head->task_cpu_use_num * 10;
                task_tcb_cycle_list_head->task_cpu_use_num = 0;
                task_tcb_cycle_list_list = task_tcb_cycle_list_list->ready_task_next;
                while (task_tcb_cycle_list_list != task_tcb_cycle_list_head)
                {
                    task_tcb_cycle_list_list->task_cpu_usage = task_tcb_cycle_list_list->task_cpu_use_num * 10;
                    task_tcb_cycle_list_list->task_cpu_use_num = 0;
                    task_tcb_cycle_list_list = task_tcb_cycle_list_list->ready_task_next;
                }
            }
            ready_task_list_list = ready_task_list_list->next;
        }

        hang_task_list = hang_task_list_head;
        while (hang_task_list != NULL)
        {
            hang_task_list->task_cpu_usage = hang_task_list->task_cpu_use_num * 10;
            hang_task_list->task_cpu_use_num = 0;
            hang_task_list = hang_task_list->hang_task_next;
        }
        sleep_task_list = sleep_task_list_head;
        while (sleep_task_list != NULL)
        {
            sleep_task_list->task_cpu_usage = sleep_task_list->task_cpu_use_num * 10;
            sleep_task_list->task_cpu_use_num = 0;
            sleep_task_list = sleep_task_list->sleep_task_next;
        }
        wait_time_task_list = wait_time_task_list_head;
        while (wait_time_task_list != NULL)
        {
            wait_time_task_list->task_cpu_usage = wait_time_task_list->task_cpu_use_num * 10;
            wait_time_task_list->task_cpu_use_num = 0;
            wait_time_task_list = wait_time_task_list->wait_time_task_next;
        } */
        exit_critical();
        os_delay(1000);
    }
}
