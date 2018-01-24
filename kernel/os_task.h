#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#include "os_cpu.h"
#include "os_kernel.h"
#include "os_config.h"

#define get_task_id(task_tcb) (os_uint32)((os_cpu_sr)task_tcb - (os_cpu_sr)mem_block_obj_list_head)
#define ready_task_list_lock(ready_task_list) ready_task_list->protection_flag = OS_ENABLE
#define ready_task_list_unlock(ready_task_list) ready_task_list->protection_flag = OS_DISENABLE


os_int8 insert_task_to_rtl(_task_tcb *task_tcb);
os_int8 task_tcb_init(_task_tcb *task_tcb, os_uint32 task_id, void (*task)(void *p_arg), os_uint8 *task_name, os_stk *task_stack_pointer, \
                      os_stk task_stack_size, os_uint8 task_priority, os_uint32 task_time_slice);
_task_tcb *creat_task(void (*task)(void *p_arg), void *p_arg, os_uint8 *task_name, os_stk *task_stack_pointer, \
                      os_stk task_stack_size, os_uint8 task_priority, os_uint32 task_time_slice);
_ready_task_list *find_high_prio_from_rtl(void);
_task_tcb *find_tcb_for_rtl_to_id(os_uint32 task_id);
_task_tcb *find_tcb_for_id(os_uint32 task_id);
os_int8 delete_task_for_rtl_d(_task_tcb *task_tcb);
os_int8 delete_task_for_rtl_s(_task_tcb *task_tcb);
os_int8 insert_task_to_rtl_h(_task_tcb *task_tcb);
os_int8 delete_task_for_htl(_task_tcb *task_tcb);
os_int8 insert_task_to_htl(_task_tcb *task_tcb);
os_int8 delete_task_for_tcb(_task_tcb *task_tcb);
os_int8 delete_task_for_id(os_uint32 task_id);
os_int8 delete_task_for_name(os_uint8 *task_name);
os_int8 hang_task_for_tcb(_task_tcb *task_tcb, os_cpu_sr wait_flag, os_uint8 is_irq);
os_int8 hang_task_for_id(os_uint32 task_id, os_cpu_sr wait_flag, os_uint8 is_irq);
os_int8 restore_task_for_tcb(_task_tcb *task_tcb, os_uint32 wait_flag, os_uint8 is_irq);
os_int8 restore_task_for_id(os_uint32 task_id, os_uint32 wait_flag, os_uint8 is_irq);
os_int8 delete_task_for_stl(_task_tcb *task_tcb);
os_int8 sleep_task_for_tcb(_task_tcb *task_tcb, os_uint8 is_irq);
os_int8 sleep_task_for_id(os_uint32 task_id, os_uint8 is_irq);
os_int8 wake_task_for_tcb(_task_tcb *task_tcb, os_uint8 is_irq);
os_int8 wake_task_for_id(os_uint32 task_id, os_uint8 is_irq);
os_int8 delete_task_for_wttl(_task_tcb *task_tcb);
os_int8 insert_task_to_wttl(_task_tcb *task_tcb);
os_int8 hang_time_task_for_tcb(_task_tcb *task_tcb, os_cpu_sr wait_flag, os_uint8 is_irq);
os_int8 hang_time_task_for_id(os_uint32 task_id, os_cpu_sr wait_flag, os_uint8 is_irq);
os_int8 restore_time_task_for_tcb(_task_tcb *task_tcb, os_uint32 wait_flag, os_uint8 is_irq);
os_int8 restore_time_task_for_id(os_uint32 task_id, os_uint32 wait_flag, os_uint8 is_irq);
os_int8 task_init(void);
void os_sys_task_check(void);
void os_task_control(void);
void os_task_switch_start(void);

#endif
