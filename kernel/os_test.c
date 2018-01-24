#include "os_test.h"

void os_print_task_list(void)
{
    uint32_t i;
	os_uint16 stack_usage;
	_ready_task_list *ready_task_list_list;
	_task_tcb_cycle_list *task_tcb_cycle_list_list, *task_tcb_cycle_list_head;
	_hang_task_list *hang_task_list;
	_sleep_task_list *sleep_task_list;
    _wait_time_task_list *wait_time_task_list;
    printf("*************************************************************************************\r\n");
    printf(" num      name   prio     id   status   wait_flag  stask_usage  task_list\r\n");
    i = 0;
    ready_task_list_list = ready_task_list_head;
    while (ready_task_list_list != NULL)
    {
        if (ready_task_list_list->task_tcb_cycle_list_head != NULL)
        {
            i++;
            task_tcb_cycle_list_head = ready_task_list_list->task_tcb_cycle_list_head;
            task_tcb_cycle_list_list = ready_task_list_list->task_tcb_cycle_list_head;
            stack_usage = get_task_stack_usage_for_tcb(task_tcb_cycle_list_head);
            printf("%4d%11s%6d%8d%6d    0X%08x    %3d.%02d%%       0X%02x\r\n", i, task_tcb_cycle_list_head->task_name, task_tcb_cycle_list_head->task_priority,  \
                   task_tcb_cycle_list_head->task_id, task_tcb_cycle_list_head->task_status, task_tcb_cycle_list_head->task_wait_flag,  \
                   stack_usage / 100, stack_usage % 100, task_tcb_cycle_list_head->task_list);
            task_tcb_cycle_list_list = task_tcb_cycle_list_list->ready_task_next;
            while (task_tcb_cycle_list_list != task_tcb_cycle_list_head)
            {
                i++;
                stack_usage = get_task_stack_usage_for_tcb(task_tcb_cycle_list_list);
                printf("%4d%11s%6d%8d%6d    0X%08x    %3d.%02d%%       0X%02x\r\n", i, task_tcb_cycle_list_list->task_name, task_tcb_cycle_list_list->task_priority,  \
                       task_tcb_cycle_list_list->task_id, task_tcb_cycle_list_list->task_status, task_tcb_cycle_list_list->task_wait_flag,  \
                       stack_usage / 100, stack_usage % 100, task_tcb_cycle_list_list->task_list);  \
                task_tcb_cycle_list_list = task_tcb_cycle_list_list->ready_task_next;
            }
        }
        ready_task_list_list = ready_task_list_list->next;
    }

    hang_task_list = hang_task_list_head;
    while (hang_task_list != NULL)
    {
        i++;
        stack_usage = get_task_stack_usage_for_tcb(hang_task_list);
        printf("%4d%11s%6d%8d%6d    0X%08x    %3d.%02d%%       0X%02x\r\n", i, hang_task_list->task_name, hang_task_list->task_priority,  \
               hang_task_list->task_id, hang_task_list->task_status, hang_task_list->task_wait_flag,  \
               stack_usage / 100, stack_usage % 100, hang_task_list->task_list);
        hang_task_list = hang_task_list->hang_task_next;
    }
    sleep_task_list = sleep_task_list_head;
    while (sleep_task_list != NULL)
    {
        i++;
        stack_usage = get_task_stack_usage_for_tcb(sleep_task_list);
        printf("%4d%11s%6d%8d%6d    0X%08x    %3d.%02d%%       0X%02x\r\n", i, sleep_task_list->task_name, sleep_task_list->task_priority,  \
               sleep_task_list->task_id, sleep_task_list->task_status, sleep_task_list->task_wait_flag,  \
               stack_usage / 100, stack_usage % 100, sleep_task_list->task_list);
        sleep_task_list = sleep_task_list->sleep_task_next;
    }
    wait_time_task_list = wait_time_task_list_head;
		while (wait_time_task_list != NULL)
		{
			i++;
			stack_usage = get_task_stack_usage_for_tcb(wait_time_task_list);
			printf("%4d%11s%6d%8d%6d    0X%08x    %3d.%02d%%       0X%02x\r\n", i, wait_time_task_list -> task_name, wait_time_task_list -> task_priority, \
				wait_time_task_list -> task_id, wait_time_task_list -> task_status, wait_time_task_list -> task_wait_flag, \
				stack_usage / 100, stack_usage % 100, wait_time_task_list->task_list);
			wait_time_task_list = wait_time_task_list -> wait_time_task_next;
		}
    printf("*************************************************************************************\r\n");
}

void os_print_cpu_usg_list(void)
{
    uint32_t i;
	_task_tcb *task_tcb_list;
    printf("******************************************************************\r\n");
    printf(" num      name   prio     cpu_usage  status  task_list\r\n");
    i = 0;
    task_tcb_list = task_tcb_list_head;
    while (task_tcb_list != NULL)
    {
        i++;
        printf("%4d%11s%6d    %3d.%02d%%        %d        0X%02x\r\n", i, task_tcb_list->task_name, task_tcb_list->task_priority,  \
                task_tcb_list -> task_cpu_usage / 100, task_tcb_list -> task_cpu_usage % 100, task_tcb_list->task_status, task_tcb_list->task_list);
        task_tcb_list = task_tcb_list->next;
    }
            
    i++;
    printf("%4d%17s    %3d.%02d%%\r\n", i, "Total usage",  \
                   (10000 - idle_task_tcb -> task_cpu_usage) / 100, (10000 - idle_task_tcb -> task_cpu_usage) % 100);
    //printf("%4d%17s    %d\r\n", i, "Total usage", idle_task_tcb -> task_cpu_usage);
    printf("******************************************************************\r\n");
}

