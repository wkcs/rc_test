#include "os_task.h"
#include "os_mem.h"
#include "os_define.h"
#include "os_config.h"
#include "os_sys_task.h"

/*
 * 插入任务到就绪任务表
 */
os_int8 insert_task_to_rtl(_task_tcb *task_tcb)
{
    _ready_task_list *ready_task_list, *ready_task_list_f;

    enter_critical();
    if ((task_tcb -> task_list & READY_TASK_LIST) == 0) {             /*先检查任务是否处于就绪态*/
        /*
        *将任务控制块添加到就绪任务列表
        *此过程涉及到了主就绪任务列表和优先级就绪任务列表
        */
        if (ready_task_list_head == NULL) {
            /*ready_task_list不被任何任务独享，不能分配任务id，此处id为0*/
            ready_task_list = (_ready_task_list *)mem_malloc(sizeof(_ready_task_list), 0);   
            if (ready_task_list == NULL)
                return -1;  /*内存不足*/
            /*初始化就绪任务列表表项*/
            ready_task_list -> task_tcb_cycle_list_head = task_tcb;
            ready_task_list -> task_priority = task_tcb -> task_priority;
            ready_task_list -> protection_flag = OS_DISENABLE;
            ready_task_list -> task_hang_num = 0;
            ready_task_list -> next = NULL;
            ready_task_list -> last = NULL;
            /*初始循环就绪任务列表表项*/
            task_tcb -> ready_task_list_parent = ready_task_list;
            task_tcb -> ready_task_next = task_tcb;
            task_tcb -> ready_task_last = task_tcb;
            /*建立表头表尾*/
            ready_task_list_head = ready_task_list;
            ready_task_list_end = ready_task_list;
            ready_task_list_high_prio = ready_task_list;
            os_high_prio_tcb = ready_task_list_high_prio -> task_tcb_cycle_list_head;
        } else {
            ready_task_list = ready_task_list_head;
            while (ready_task_list != NULL) {
                if (task_tcb -> task_priority == ready_task_list -> task_priority)
                    break;
                else
                    ready_task_list = ready_task_list -> next;
            }
            if (ready_task_list != NULL) {      
                /*已经存在相应优先级的循环就绪任务列表*/
                if (ready_task_list -> task_tcb_cycle_list_head != NULL) {    /*非空表*/   
                    task_tcb -> ready_task_list_parent = ready_task_list;
                    task_tcb -> ready_task_last = ready_task_list -> task_tcb_cycle_list_head -> ready_task_last;
                    task_tcb -> ready_task_next = ready_task_list -> task_tcb_cycle_list_head;
                    ready_task_list -> task_tcb_cycle_list_head -> ready_task_last -> ready_task_next = task_tcb;
                    ready_task_list -> task_tcb_cycle_list_head -> ready_task_last = task_tcb;
                } else {                                                                  /*空表*/   
                    task_tcb -> ready_task_list_parent = ready_task_list;
                    task_tcb -> ready_task_last = task_tcb;
                    task_tcb -> ready_task_next = task_tcb;
                    ready_task_list -> task_tcb_cycle_list_head = task_tcb;
                    /*判断是否为当前最高优先级非空表*/
                    if (ready_task_list_high_prio -> task_priority > ready_task_list -> task_priority) {
                        ready_task_list_high_prio = ready_task_list;
                        os_high_prio_tcb = ready_task_list_high_prio -> task_tcb_cycle_list_head;
                    }
                }
            } else {
                /*不存在相应优先级的就绪任务表，需要创建*/
                ready_task_list = (_ready_task_list *)mem_malloc(sizeof(_ready_task_list), 0);
                if (ready_task_list == NULL)
                    return -1;  /*内存不足*/
                ready_task_list_f = ready_task_list_head;
                while ((ready_task_list_f != NULL) && (task_tcb -> task_priority > ready_task_list_f -> task_priority))
                    ready_task_list_f = ready_task_list_f -> next;
                if (ready_task_list_f == ready_task_list_head) {
                    /*初始化主就绪任务列表表项*/
                    ready_task_list -> task_tcb_cycle_list_head = task_tcb;
                    ready_task_list -> task_priority = task_tcb -> task_priority;
                    ready_task_list -> protection_flag = OS_DISENABLE;
                    ready_task_list -> task_hang_num = 0;
                    ready_task_list -> next = ready_task_list_head;
                    ready_task_list -> last = NULL;
                    ready_task_list_head -> last = ready_task_list;
                    /*初始化循环就绪任务列表表项*/
                    task_tcb -> ready_task_list_parent = ready_task_list;
                    task_tcb -> ready_task_next = task_tcb;
                    task_tcb -> ready_task_last = task_tcb;

                    ready_task_list_head = ready_task_list;
                    /*判断是否为当前最高优先级非空表*/
                    if (ready_task_list_high_prio -> task_priority > ready_task_list -> task_priority) {
                        ready_task_list_high_prio = ready_task_list;
                        os_high_prio_tcb = ready_task_list_high_prio -> task_tcb_cycle_list_head;
                    }
                } else if (ready_task_list_f == NULL) {
                    /*初始化主就绪任务列表表项*/
                    ready_task_list -> task_tcb_cycle_list_head = task_tcb;
                    ready_task_list -> task_priority = task_tcb -> task_priority;
                    ready_task_list -> protection_flag = OS_DISENABLE;
                    ready_task_list -> task_hang_num = 0;
                    ready_task_list -> next = NULL;
                    ready_task_list -> last = ready_task_list_end;
                    ready_task_list_end -> next = ready_task_list;
                    /*初始化循环就绪任务列表表项*/
                    task_tcb -> ready_task_list_parent = ready_task_list;
                    task_tcb -> ready_task_next = task_tcb;
                    task_tcb -> ready_task_last = task_tcb;

                    ready_task_list_end = ready_task_list;
                    if (ready_task_list_high_prio == NULL) {
                        ready_task_list_high_prio = ready_task_list;
                        os_high_prio_tcb = ready_task_list_high_prio -> task_tcb_cycle_list_head;
                    }
                } else {
                    /*初始化主就绪任务列表表项,并插入表项*/
                    ready_task_list -> task_tcb_cycle_list_head = task_tcb;
                    ready_task_list -> task_priority = task_tcb -> task_priority;
                    ready_task_list -> protection_flag = OS_DISENABLE;
                    ready_task_list -> task_hang_num = 0;
                    ready_task_list -> next = ready_task_list_f;
                    ready_task_list -> last = ready_task_list_f -> last;
                    ready_task_list_f -> last -> next = ready_task_list;
                    ready_task_list_f -> last = ready_task_list;
                    /*初始化循环就绪任务列表表项*/
                    task_tcb -> ready_task_list_parent = ready_task_list;
                    task_tcb -> ready_task_next = task_tcb;
                    task_tcb -> ready_task_last = task_tcb;
                    /*判断是否为当前最高优先级非空表*/
                    if (ready_task_list_high_prio -> task_priority > ready_task_list -> task_priority) {
                        ready_task_list_high_prio = ready_task_list;
                        os_high_prio_tcb = ready_task_list_high_prio -> task_tcb_cycle_list_head;
                    }
                }
            }
        }
        task_tcb -> task_list |= READY_TASK_LIST;
        exit_critical();
        return 0;
    } else {
        exit_critical();
        return -1;
    }
}

os_int8 task_tcb_init(_task_tcb *task_tcb, os_uint32 task_id,void (*task)(void *p_arg), os_uint8 *task_name, os_stk *task_stack_pointer, \
                      os_stk task_stack_size, os_uint8 task_priority, os_uint32 task_time_slice)
{
    enter_critical();
    /* 任务栈顶指针*/
    task_tcb -> task_stack_pointer = task_stack_pointer;
    /* 任务栈起始地址*/
    task_tcb -> task_stack_start_addr = task_stack_pointer;
    /* 任务栈大小*/
    task_tcb -> task_stack_size = task_stack_size * OS_STK_SIZE;   
    /*任务优先级*/          
	task_tcb -> task_priority = task_priority;   
    /*任务id*/          
	task_tcb -> task_id = task_id;       
    /*任务名*/
	task_tcb -> task_name = task_name;
    /*任务入口函数*/
	task_tcb -> task_addr = task;
    /*任务状态*/	
    task_tcb -> task_status = TASK_READY;
    /*任务时间片初始值*/
    task_tcb -> task_time_slice_init = task_time_slice;
    /*任务时间片剩余*/
    task_tcb -> task_time_slice_over = task_time_slice;
    /*任务等待事件标志*/
    task_tcb -> task_wait_flag = NO_WAIT;
    /*任务延时剩余时间*/
    task_tcb -> task_delay_time_over = 0;
    /*任务CPU使用计数*/
    task_tcb -> task_cpu_use_num = 0;
    /*任务CPU使用率*/
    task_tcb -> task_cpu_usage = 0;
    task_tcb -> task_list = ALL_TASK_LIST;
    /*索引级就绪任务列表表项*/
    task_tcb -> ready_task_list_parent = NULL;
    if (task_tcb_list_head == NULL) {
        task_tcb -> next = NULL;
        task_tcb -> last = NULL;
        task_tcb_list_head = task_tcb;
        task_tcb_list_end = task_tcb;
    } else {
        task_tcb -> next = NULL;
        task_tcb -> last = task_tcb_list_end;
        task_tcb_list_end -> next = task_tcb;
        task_tcb_list_end = task_tcb;
    }
    task_tcb -> ready_task_next = NULL;
    task_tcb -> ready_task_last = NULL;
    task_tcb -> hang_task_next = NULL;
    task_tcb -> hang_task_last = NULL;
    task_tcb -> sleep_task_next = NULL;
    task_tcb -> sleep_task_last = NULL;
    /*为时间相关的等待任务创建，以便在临界区也可以操作*/
    task_tcb -> wait_time_task_next = NULL;
    task_tcb -> wait_time_task_last = NULL;
    exit_critical();
	return 0;
}

_task_tcb *creat_task(void (*task)(void *p_arg), void *p_arg, os_uint8 *task_name, os_stk *task_stack_pointer, \
                   os_stk task_stack_size, os_uint8 task_priority, os_uint32 task_time_slice)
{
    _task_tcb *task_tcb;
    os_uint32 task_id;
    if (task_time_slice > 0) {
        enter_critical();
        /*
        * 分配任务控制块
        */
        task_tcb = (_task_tcb *)mem_malloc(sizeof(_task_tcb), 0);

        /*
        * 请求分配任务id
        */
        task_id = get_task_id(task_tcb);

        /*
        * 为任务控制块所在内存块设置任务id
        */
        mem_set_task_id((void *)task_tcb, task_id);

        /*
        * 初始化任务控制块
        */
        task_tcb_init(task_tcb, task_id, task, task_name, task_stack_pointer, task_stack_size, \
                    task_priority, task_time_slice);
        
        /*
        * 初始化任务堆栈
        */
        task_tcb -> task_stack_pointer = task_stack_init(task, p_arg, task_stack_pointer);

        /*
        * 将任务插入就绪任务列表
        */
        if (insert_task_to_rtl(task_tcb) != 0) {
            exit_critical();
            return NULL;
        }
        exit_critical();
        return task_tcb;
    } else
        return NULL;
}

/*
 * 从就绪任务列表中找到优先级最高的非空表
 */
_ready_task_list *find_high_prio_from_rtl(void)
{
    _ready_task_list *ready_task_list;

    /*
     * 遍历就绪任务列表，寻找第一个非空表
     */
    enter_critical();
    ready_task_list = ready_task_list_head;    /*从表头开始*/
	while (ready_task_list != NULL) {
        if (ready_task_list -> task_tcb_cycle_list_head != NULL) {
            os_high_prio_tcb = ready_task_list -> task_tcb_cycle_list_head;
            exit_critical();
            return ready_task_list;
        }
        ready_task_list = ready_task_list -> next;
    }
    exit_critical();
    /*未找到F非空表*/
    return NULL;
}

/*
 * 删除就绪任务列表的空表
 */
os_int8 delete_rtl_null(_ready_task_list *ready_task_list) 
{
    enter_critical();
    if (ready_task_list -> task_tcb_cycle_list_head == NULL && ready_task_list -> protection_flag == OS_DISENABLE) {
         if (ready_task_list == ready_task_list_head) {
            ready_task_list_head = ready_task_list -> next;
            ready_task_list_head -> last = NULL;
        } else if (ready_task_list == ready_task_list_end) {
            ready_task_list_end = ready_task_list -> last;
            ready_task_list_end -> next = NULL;
        } else {
            ready_task_list -> last -> next = ready_task_list -> next;
            ready_task_list -> next -> last = ready_task_list -> last;
        }
        mem_free(ready_task_list);
        exit_critical();
        return 0;
    } else {
        exit_critical();
        return 1;     /*非空表，不删除*/
    }
}

/*
 * 从就绪任务列表中找到对应id的任务
 */
_task_tcb *find_tcb_for_rtl_to_id(os_uint32 task_id)
{
    _ready_task_list *ready_task_list;
	_task_tcb_cycle_list *task_tcb_cycle_list, *task_tcb_cycle_list_head;

    /*
     * 遍历就绪任务列表，找到对应id的任务
     */
    enter_critical();
    ready_task_list = ready_task_list_high_prio;    /*从第一个非空表开始*/
	while (ready_task_list != NULL)
	{
        if (ready_task_list -> task_tcb_cycle_list_head != NULL) {   /*跳过空表*/
            task_tcb_cycle_list_head = ready_task_list -> task_tcb_cycle_list_head;
            task_tcb_cycle_list = ready_task_list -> task_tcb_cycle_list_head;
            if (task_id == task_tcb_cycle_list_head -> task_id) {
                exit_critical();
                return task_tcb_cycle_list_head;
            }
            task_tcb_cycle_list = task_tcb_cycle_list -> ready_task_next;
            while (task_tcb_cycle_list != task_tcb_cycle_list_head)
            {
                if (task_id == task_tcb_cycle_list -> task_id) {
                    exit_critical();
                    return task_tcb_cycle_list;
                }
                task_tcb_cycle_list = task_tcb_cycle_list -> ready_task_next;
            }
        }
        ready_task_list = ready_task_list -> next;
	}
    exit_critical();
    /*未找到对应id的任务*/
    return NULL;
}

/*
 * 根据任务id获取任务控制块
 * 如果id有问题可能会导致内存出错
 * 因此使用此函数一定要确保任务id的准确性
 */
_task_tcb *find_tcb_for_id(os_uint32 task_id)
{
    _task_tcb *task_tcb;
    enter_critical();
    task_tcb = (_task_tcb *)((os_cpu_sr)mem_block_obj_list_head + task_id);
    /*检查转化的任务控制块地址是否合法*/
    if (((void *)task_tcb > (void *)mem_block_obj_list_head) && \
    ((void *)task_tcb < (void *)(mem_block_obj_list_head + MEM_MAX_SIZE)) && \
    (((os_cpu_sr)task_tcb - (os_cpu_sr)(mem_block_obj_list_head + 1)) % MEM_BLOCK_SIZE == 0))
        if (task_tcb -> task_id == task_id) {
            exit_critical();
            return task_tcb;
        }

    /*未找到对应id的任务*/
    exit_critical();
    return NULL;
}

/*
 * 从就绪任务列表删除任务,如果变成空表则删除空表
 */
os_int8 delete_task_for_rtl_d(_task_tcb *task_tcb)
{
    _ready_task_list *ready_task_list;
    enter_critical();
    ready_task_list = task_tcb -> ready_task_list_parent;
    if (ready_task_list -> task_tcb_cycle_list_head != NULL && (task_tcb -> task_list & READY_TASK_LIST) == READY_TASK_LIST) {
        if (ready_task_list -> task_tcb_cycle_list_head == ready_task_list -> task_tcb_cycle_list_head -> ready_task_next) {   /*此优先级下只有一个任务，则删除此优先级的表项*/
            if (ready_task_list -> protection_flag == OS_DISENABLE) {
                /*先从就绪任务列表中删除表项*/
                if (ready_task_list -> last != NULL) {
                    ready_task_list -> last -> next = ready_task_list -> next;
                    if (ready_task_list -> next != NULL) {
                        ready_task_list -> next -> last = ready_task_list -> last;
                    } else {        /*表头处理*/
                        ready_task_list_end = ready_task_list -> last;
                        ready_task_list_end -> next = NULL;
                    }
                    if (ready_task_list == ready_task_list_high_prio)
                        ready_task_list_high_prio = find_high_prio_from_rtl();
                } else {              /*表尾处理*/
                    /*寻找新的表头*/
                    ready_task_list_head = ready_task_list -> next;
                    ready_task_list_head -> last = NULL;
                    ready_task_list_high_prio = find_high_prio_from_rtl();
                }
                /*释放内存*/
                mem_free((void *)ready_task_list);
            } else {
                ready_task_list -> task_tcb_cycle_list_head = NULL;     
                if (ready_task_list == ready_task_list_high_prio)
                    ready_task_list_high_prio = find_high_prio_from_rtl();
            }

        } else {
            /*从优先级就绪任务列表中删除表项*/
            task_tcb -> ready_task_last -> ready_task_next = task_tcb -> ready_task_next;
            task_tcb -> ready_task_next -> ready_task_last = task_tcb -> ready_task_last;
            if (task_tcb == ready_task_list -> task_tcb_cycle_list_head) {
                ready_task_list -> task_tcb_cycle_list_head = task_tcb -> ready_task_next;
                if (ready_task_list == ready_task_list_high_prio)
                    os_high_prio_tcb = ready_task_list -> task_tcb_cycle_list_head;
            }
        }
        task_tcb -> task_status = NO_STATUS;
        task_tcb -> task_list &= ((~READY_TASK_LIST) & 0xff);
    } else {
        exit_critical();
        return -1;
    }
    exit_critical();
    return 0;
}

/*
 * 从就绪任务列表删除任务,如果变成空表并不删除
 */
os_int8 delete_task_for_rtl_s(_task_tcb *task_tcb)
{
    _ready_task_list *ready_task_list;
    enter_critical();
    ready_task_list = task_tcb -> ready_task_list_parent;
    if (ready_task_list -> task_tcb_cycle_list_head != NULL && (task_tcb -> task_list & READY_TASK_LIST) == READY_TASK_LIST) {
        if (ready_task_list -> task_tcb_cycle_list_head == ready_task_list -> task_tcb_cycle_list_head -> ready_task_next) {   /*此优先级下只有一个任务，则删除此优先级的表项*/
            ready_task_list -> task_tcb_cycle_list_head = NULL;     
            if (ready_task_list == ready_task_list_high_prio)
                ready_task_list_high_prio = find_high_prio_from_rtl();
        } else {
            /*从优先级就绪任务列表中删除表项*/
            task_tcb -> ready_task_last -> ready_task_next = task_tcb -> ready_task_next;
            task_tcb -> ready_task_next -> ready_task_last = task_tcb -> ready_task_last;
            if (task_tcb == ready_task_list -> task_tcb_cycle_list_head) {
                ready_task_list -> task_tcb_cycle_list_head = task_tcb -> ready_task_next;
                if (ready_task_list == ready_task_list_high_prio)
                    os_high_prio_tcb = ready_task_list -> task_tcb_cycle_list_head;
            }
        }
        task_tcb -> task_status = NO_STATUS;
        task_tcb -> task_list &= ((~READY_TASK_LIST) & 0xff);
    } else {
        exit_critical();
        return -1;
    }
    exit_critical();
    return 0;
}

/*
 * 将从挂起状态转到就绪态的任务快速插入到就绪任务列表
 */
os_int8 insert_task_to_rtl_h(_task_tcb *task_tcb)
{
    _ready_task_list *ready_task_list;
    enter_critical();
    if ((task_tcb -> task_list & READY_TASK_LIST) == 0) {
        ready_task_list = task_tcb -> ready_task_list_parent;
        if (ready_task_list != NULL) {
            if (ready_task_list -> task_tcb_cycle_list_head != NULL) {
                task_tcb -> ready_task_last = ready_task_list -> task_tcb_cycle_list_head -> ready_task_last;
                task_tcb -> ready_task_next = ready_task_list -> task_tcb_cycle_list_head;
                ready_task_list -> task_tcb_cycle_list_head -> ready_task_last -> ready_task_next = task_tcb;
                ready_task_list -> task_tcb_cycle_list_head -> ready_task_last = task_tcb;
            } else {
                task_tcb -> ready_task_last = task_tcb;
                task_tcb -> ready_task_next = task_tcb;
                ready_task_list -> task_tcb_cycle_list_head = task_tcb;
            }
            task_tcb -> task_list |= READY_TASK_LIST;
            ready_task_list_high_prio = find_high_prio_from_rtl();
        } else {
            exit_critical();
            return -1;
        }
    } else {
        exit_critical();
        return -1;
    }
    exit_critical();
    return 0;
}

/*
 * 从挂起任务列表删除任务
 */
os_int8 delete_task_for_htl(_task_tcb *task_tcb)
{
    if (hang_task_list_head != NULL) {
        enter_critical();
        if ((task_tcb -> task_list & HANG_TASK_LIST) == HANG_TASK_LIST) {
            if (task_tcb -> hang_task_next == NULL) {
                if (task_tcb -> hang_task_last == NULL) {
                    hang_task_list_head = NULL;
                    hang_task_list_end = NULL;
                } else {
                    task_tcb -> hang_task_last -> hang_task_next = NULL;
                    hang_task_list_end = task_tcb -> hang_task_last;
                }
            } else if (task_tcb -> hang_task_last == NULL){
                task_tcb -> hang_task_next -> hang_task_last = NULL;
                hang_task_list_head = task_tcb -> hang_task_next;
            } else {
                task_tcb -> hang_task_next -> hang_task_last = task_tcb -> hang_task_last;
                task_tcb -> hang_task_last -> hang_task_next = task_tcb -> hang_task_next;
            }
            task_tcb -> task_status = NO_STATUS;
            task_tcb -> task_list &= ((~HANG_TASK_LIST) & 0xff);
        } else {
            exit_critical();
            return -1;
        }
        exit_critical();
    } else
        return -1;
    return 0;
}

/*
 * 将任务插入到挂起任务列表
 */
os_int8 insert_task_to_htl(_task_tcb *task_tcb)
{
    enter_critical();
    if ((task_tcb -> task_list & HANG_TASK_LIST) == 0) {
        if (hang_task_list_head == NULL) {
            hang_task_list_head = task_tcb;
            hang_task_list_end = task_tcb;
            task_tcb -> hang_task_next = NULL;
            task_tcb -> hang_task_last = NULL;
        } else {
            hang_task_list_end -> hang_task_next = task_tcb;
            task_tcb -> hang_task_last = hang_task_list_end;
            task_tcb -> hang_task_next = NULL;
            hang_task_list_end = task_tcb;
        }
        task_tcb -> task_list |= HANG_TASK_LIST;
        exit_critical();
        return 0;
    } else {
        exit_critical();
        return -1;
    }
}

/*
 * 根据任务控制块删除任务
 */
os_int8 delete_task_for_tcb(_task_tcb *task_tcb)
{
    if (task_tcb != NULL) {
        enter_critical();
        if (task_tcb -> task_status != TASK_RUNNING) {
            if (task_tcb -> task_list & READY_TASK_LIST == READY_TASK_LIST) 
                delete_task_for_rtl_d(task_tcb);
            else if (task_tcb -> task_list & HANG_TASK_LIST == HANG_TASK_LIST) 
                delete_task_for_htl(task_tcb);
            else if (task_tcb -> task_list & SLEEP_TASK_LIST == SLEEP_TASK_LIST)
                delete_task_for_stl(task_tcb);
            else if (task_tcb -> task_list & WAIT_TIME_TASK_LIST == WAIT_TIME_TASK_LIST)
                delete_task_for_wttl(task_tcb);
            /*释放任务控制块内存*/
            if (task_tcb == task_tcb_list_head) {
                task_tcb_list_head = task_tcb -> next;
                task_tcb_list_head -> last = NULL;
            } else if (task_tcb == task_tcb_list_end) {
                task_tcb_list_end = task_tcb -> last;
                task_tcb_list_end -> next = NULL;
            } else {
                task_tcb -> last -> next = task_tcb -> next;
                task_tcb -> next -> last = task_tcb -> last;
            }
            mem_free_for_task_id(task_tcb -> task_id);
        }
        exit_critical();
        return 0;
    } else
        return -1;
}

/*
 * 根据任务id删除任务
 */
os_int8 delete_task_for_id(os_uint32 task_id)
{
    _task_tcb *task_tcb;
    os_int8 status;
    enter_critical();
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb != NULL) {
        status = delete_task_for_tcb(task_tcb);
        exit_critical();
        return status;
    } else {
        exit_critical();
        return -1;
    }
}

/*
 * 根据任务控制块挂起任务
 */
os_int8 hang_task_for_tcb(_task_tcb *task_tcb, os_cpu_sr wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _ready_task_list *ready_task_list;
    if (task_tcb != NULL) {
        enter_critical();
        if (task_tcb -> task_list & READY_TASK_LIST == READY_TASK_LIST) {         /*检查任务状态，睡眠任务不能被挂起*/
            status = delete_task_for_rtl_s(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            ready_task_list -> task_hang_num++;
            ready_task_list_lock(ready_task_list);
            task_tcb -> task_status = TASK_HANG;
            task_tcb -> task_wait_flag = wait_flag;
            status = insert_task_to_htl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        }
        exit_critical();
    } else
        return -1;

    return 0;
}

/*
 * 根据任务id挂起任务
 */
os_int8 hang_task_for_id(os_uint32 task_id, os_cpu_sr wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = hang_task_for_tcb(task_tcb, wait_flag, is_irq);

    return status;
}

/*
 * 根据任务控制块恢复挂起任务
 */
os_int8 restore_task_for_tcb(_task_tcb *task_tcb, os_uint32 wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _ready_task_list *ready_task_list;
    if (task_tcb != NULL) {
        enter_critical();
        if (task_tcb -> task_list & HANG_TASK_LIST == HANG_TASK_LIST) {         /*检查任务状态*/
            status = delete_task_for_htl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            if ((ready_task_list -> task_hang_num) > 0)
                ready_task_list -> task_hang_num--;
            if ((ready_task_list -> task_hang_num) == 0)
                ready_task_list_unlock(ready_task_list);
            task_tcb -> task_status = TASK_READY;
            task_tcb -> task_wait_flag &= (~wait_flag);
            status = insert_task_to_rtl_h(task_tcb);
            if (status != 0) {
                status = insert_task_to_rtl(task_tcb);
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        }
        exit_critical();
    } else
        return -1;
    return 0;
}

/*
 * 根据任务id恢复挂起任务
 */
os_int8 restore_task_for_id(os_uint32 task_id, os_uint32 wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = restore_task_for_tcb(task_tcb, wait_flag, is_irq);
    return status;
}

/*
 * 从睡眠任务列表删除任务
 */
os_int8 delete_task_for_stl(_task_tcb *task_tcb)
{
    if (sleep_task_list_head != NULL) {
        enter_critical();
        if (task_tcb -> task_list & SLEEP_TASK_LIST == SLEEP_TASK_LIST) {
            if (task_tcb -> sleep_task_next == NULL) {
                if (task_tcb -> sleep_task_last == NULL) {
                    sleep_task_list_head = NULL;
                    sleep_task_list_end = NULL;
                } else {
                    task_tcb -> sleep_task_last -> sleep_task_next = NULL;
                    sleep_task_list_end = task_tcb -> sleep_task_last;
                }
            } else if (task_tcb -> sleep_task_last == NULL){
                task_tcb -> sleep_task_next -> sleep_task_last = NULL;
                sleep_task_list_head = task_tcb -> sleep_task_next;
            } else {
                task_tcb -> sleep_task_next -> sleep_task_last = task_tcb -> sleep_task_last;
                task_tcb -> sleep_task_last -> sleep_task_next = task_tcb -> sleep_task_next;
            }
            task_tcb -> task_status = NO_STATUS;
            task_tcb -> task_list &= ((~SLEEP_TASK_LIST) & 0xff);
        } else {
             exit_critical();
            return -1;
        }
         exit_critical();
    } else
        return -1;
    return 0;
}

/*
 * 将任务插入到睡眠任务列表
 */
os_int8 insert_task_to_stl(_task_tcb *task_tcb)
{
    enter_critical();
    if (task_tcb -> task_list & SLEEP_TASK_LIST == 0) {
        if (sleep_task_list_head == NULL) {
            sleep_task_list_head = task_tcb;
            sleep_task_list_end = task_tcb;
            task_tcb -> sleep_task_next = NULL;
            task_tcb -> sleep_task_last = NULL;
        } else {
            sleep_task_list_end -> sleep_task_next = task_tcb;
            task_tcb -> last = sleep_task_list_end;
            task_tcb -> next = NULL;
            sleep_task_list_end = task_tcb;
        }
        task_tcb -> task_list |= SLEEP_TASK_LIST;
        exit_critical();
        return 0;
    } else {
        exit_critical();
        return -1;
    }
}

/*
 * 根据任务控制块使任务进入睡眠
 */
os_int8 sleep_task_for_tcb(_task_tcb *task_tcb, os_uint8 is_irq)
{
     os_int8 status;
    _ready_task_list *ready_task_list;
	enter_critical();
    if (task_tcb != NULL) {
        if (task_tcb -> task_list & READY_TASK_LIST == READY_TASK_LIST) {         /*检查任务状态*/
            status = delete_task_for_rtl_d(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            task_tcb -> task_status = TASK_SLEEP;
            status = insert_task_to_stl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else if (task_tcb -> task_list & HANG_TASK_LIST == HANG_TASK_LIST){
            status = delete_task_for_htl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            if ((ready_task_list -> task_hang_num) > 0)
                ready_task_list -> task_hang_num--;
            if ((ready_task_list -> task_hang_num) == 0) {
                ready_task_list_unlock(ready_task_list);
                delete_rtl_null(ready_task_list);
            }
            task_tcb -> task_status = TASK_SLEEP;
            status = insert_task_to_stl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else if (task_tcb -> task_list & WAIT_TIME_TASK_LIST == WAIT_TIME_TASK_LIST){
            status = delete_task_for_wttl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            if ((ready_task_list -> task_hang_num) > 0)
                ready_task_list -> task_hang_num--;
            if ((ready_task_list -> task_hang_num) == 0) {
                ready_task_list_unlock(ready_task_list);
                delete_rtl_null(ready_task_list);
            }
            task_tcb -> task_status = TASK_SLEEP;
            status = insert_task_to_stl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        }
    } else {
        exit_critical();
        return -1;
    }
    exit_critical();
    return 0;
}

/*
 * 根据任务id使任务进入睡眠
 */
os_int8 sleep_task_for_id(os_uint32 task_id, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = sleep_task_for_tcb(task_tcb, is_irq);
    return status;
}

/*
 * 根据任务控制块唤醒睡眠任务
 */
os_int8 wake_task_for_tcb (_task_tcb *task_tcb, os_uint8 is_irq)
{
    os_int8 status;
    enter_critical();
    if (task_tcb != NULL) {
        if (task_tcb -> task_list & SLEEP_TASK_LIST == SLEEP_TASK_LIST) {         /*检查任务状态*/
            status = delete_task_for_stl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            task_tcb -> task_status = TASK_READY;
            status = insert_task_to_rtl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        } else {
            exit_critical();
            return 0;
        }
    } else {
        exit_critical();
        return -1;
    }
}

/*
 * 根据任务id唤醒睡眠任务
 */
os_int8 wake_task_for_id (os_uint32 task_id, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = wake_task_for_tcb(task_tcb, is_irq);
    return status;
}

/*
 * 从时间相关挂起任务列表删除任务
 */
os_int8 delete_task_for_wttl(_task_tcb *task_tcb)
{
    if (wait_time_task_list_head != NULL) {
        enter_critical();
        if (task_tcb -> task_list & WAIT_TIME_TASK_LIST == WAIT_TIME_TASK_LIST) {
            if (task_tcb -> wait_time_task_next == NULL) {
                if (task_tcb -> wait_time_task_last == NULL) {
                    wait_time_task_list_head = NULL;
                    wait_time_task_list_end = NULL;
                } else {
                    task_tcb -> wait_time_task_last -> wait_time_task_next = NULL;
                    wait_time_task_list_end = task_tcb -> wait_time_task_last;
                }
            } else if (task_tcb -> wait_time_task_last == NULL){
                task_tcb -> wait_time_task_next -> wait_time_task_last = NULL;
                wait_time_task_list_head = task_tcb -> wait_time_task_next;
            } else {
                task_tcb -> wait_time_task_last -> wait_time_task_next = task_tcb -> wait_time_task_next;
                task_tcb -> wait_time_task_next -> wait_time_task_last = task_tcb -> wait_time_task_last;
            }
            task_tcb -> task_status = NO_STATUS;
            task_tcb -> task_list &= ((~WAIT_TIME_TASK_LIST) & 0xff);
        } else {
             exit_critical();
            return -1;
        }
         exit_critical();
    } else
        return -1;
    return 0;
}

/*
 * 将任务插入到时间相关挂起任务列表
 */
os_int8 insert_task_to_wttl(_task_tcb *task_tcb)
{
    enter_critical();
    if (task_tcb -> task_list & WAIT_TIME_TASK_LIST == 0) {
        if (wait_time_task_list_head == NULL) {
            task_tcb -> wait_time_task_next = NULL;
            task_tcb -> wait_time_task_last = NULL;
            wait_time_task_list_end = task_tcb;
            wait_time_task_list_head = task_tcb;
        } else {
            task_tcb -> wait_time_task_last = wait_time_task_list_end;
            task_tcb -> wait_time_task_next = NULL;
            wait_time_task_list_end -> wait_time_task_next = task_tcb;
            wait_time_task_list_end = task_tcb;
        }
        task_tcb -> task_list |= WAIT_TIME_TASK_LIST;
        exit_critical();
        return 0;
    } else {
        exit_critical();
        return -1;
    }
}

/*
 * 根据任务控制块使任务进入时间相关挂起
 */
os_int8 hang_time_task_for_tcb(_task_tcb *task_tcb, os_cpu_sr wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _ready_task_list *ready_task_list;
    if (task_tcb != NULL) {
        enter_critical();
        if (task_tcb -> task_list & READY_TASK_LIST == READY_TASK_LIST) {         /*检查任务状态，睡眠任务不能被挂起*/
            status = delete_task_for_rtl_s(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            ready_task_list -> task_hang_num++;
            ready_task_list_lock(ready_task_list);
            task_tcb -> task_status = TASK_HANG_TIME;
            task_tcb -> task_wait_flag = wait_flag;
            status = insert_task_to_wttl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        }
        exit_critical();
    } else
        return -1;

    return 0;
}

/*
 * 根据任务id挂起任务
 */
os_int8 hang_time_task_for_id(os_uint32 task_id, os_cpu_sr wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = hang_time_task_for_tcb(task_tcb, wait_flag, is_irq);

    return status;
}


/*
 * 根据任务控制块恢复挂起任务
 */
os_int8 restore_time_task_for_tcb(_task_tcb *task_tcb, os_uint32 wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _ready_task_list *ready_task_list;
    if (task_tcb != NULL) {
        enter_critical();
        if (task_tcb -> task_list & WAIT_TIME_TASK_LIST == WAIT_TIME_TASK_LIST) {         /*检查任务状态*/
            status = delete_task_for_wttl(task_tcb);
            if (status != 0) {
                exit_critical();
                return status;
            }
            ready_task_list = task_tcb -> ready_task_list_parent;
            if ((ready_task_list -> task_hang_num) > 0)
                ready_task_list -> task_hang_num--;
            if ((ready_task_list -> task_hang_num) == 0)
                ready_task_list_unlock(ready_task_list);
            task_tcb -> task_status = TASK_READY;
            task_tcb -> task_wait_flag &= (~wait_flag);
            status = insert_task_to_rtl_h(task_tcb);
            if (status != 0) {
                status = insert_task_to_rtl(task_tcb);
                exit_critical();
                return status;
            }
        } else {
            exit_critical();
            return -1;
        }
        if (is_irq == NOT_IRQ) {
            exit_critical();
            os_task_switch_start();
            return 0;
        }
        exit_critical();
    } else
        return -1;
    return 0;
}

/*
 * 根据任务id恢复挂起任务
 */
os_int8 restore_time_task_for_id(os_uint32 task_id, os_uint32 wait_flag, os_uint8 is_irq)
{
    os_int8 status;
    _task_tcb *task_tcb;
    /*找到对应的任务控制块*/
    task_tcb = find_tcb_for_id(task_id);
    if (task_tcb == NULL)
        return -1;
    status = restore_time_task_for_tcb(task_tcb, wait_flag, is_irq);
    return status;
}

/*
 * 创建系统任务
 */
os_int8 task_init(void)
{
    idle_task_tcb = creat_task(idle_task, (void *)0, idle_task_name, (os_stk *)&idle_task_stack[IDLE_TASK_STACK_SIZE - 1], \
                        IDLE_TASK_STACK_SIZE, IDLE_TASK_PRIO, 10);
    sys_hang_check_task_tcb = creat_task(sys_hang_check_task, (void *)0, sys_hang_check_task_name, \
                        (os_stk *)&sys_hang_check_task_stack[SYS_HANG_CHECK_TASK_STACK_SIZE - 1], \
                        SYS_HANG_CHECK_TASK_STACK_SIZE, SYS_HANG_CHECK_TASK_PRIO, 10);
    os_statistics_task_tcb = creat_task(os_statistics_task, (void *)0, os_statistics_task_name, \
                        (os_stk *)&os_statistics_task_stack[OS_STATISTICS_TASK_STACK_SIZE - 1], \
                        OS_STATISTICS_TASK_STACK_SIZE, OS_STATISTICS_TASK_PRIO, 10);
    return 0;
}

/*
 * 检查是否需要恢复系统任务
 */
void os_sys_task_check(void)
{
    _hang_task_list *hang_task_list;
    hang_task_list = hang_task_list_head;
    while (hang_task_list != NULL && hang_task_list->task_wait_flag & WAIT_DELAY)
    {
        if (hang_task_list -> task_delay_time_over > 0) {
            hang_task_list -> task_delay_time_over--;
            #if OS_CRITICAL_METHOD == 1
            if (os_task_control_en == 1) {
            #endif
                if (hang_task_list -> task_delay_time_over == 0)
                    restore_time_task_for_tcb(hang_task_list, WAIT_DELAY, IS_IRQ);    
            #if OS_CRITICAL_METHOD == 1
            }
            #endif
        } else {
            #if OS_CRITICAL_METHOD == 1
            if (os_task_control_en == 1) {
            #endif
                restore_time_task_for_tcb(hang_task_list, WAIT_DELAY, IS_IRQ);    
            #if OS_CRITICAL_METHOD == 1
            }
            #endif
        }
        hang_task_list = hang_task_list -> hang_task_next;
    }
    #if OS_CRITICAL_METHOD == 1
    if (os_task_control_en == 1) {
    #endif
    if (hang_task_list_head != NULL && !((hang_task_list_head == hang_task_list_end) && (hang_task_list_head == sys_hang_check_task_tcb))) {
        restore_task_for_tcb(sys_hang_check_task_tcb, NO_WAIT, IS_IRQ);
    }
    #if OS_CRITICAL_METHOD == 1
    }
    #endif
}


/*
 * 系统任务调度，在每个时钟周期执行，运行就绪任务列表的第一个任务
 */
void os_task_control(void)
{
    os_run_prio_tcb -> task_cpu_use_num++;
    #if OS_CRITICAL_METHOD == 1
    if (os_task_control_en == 1) {
    #endif
        if (os_run_prio_tcb != NULL){
            if (os_run_prio_tcb -> task_time_slice_over > 0) {
                os_run_prio_tcb -> task_time_slice_over--;
                if (os_run_prio_tcb -> task_time_slice_over == 0){
                    os_run_prio_tcb -> task_time_slice_over = os_run_prio_tcb -> task_time_slice_init;
                    if (os_run_prio_tcb -> task_status == TASK_RUNNING)
                        os_run_prio_tcb -> task_status = TASK_READY;
                    os_run_prio_tcb -> ready_task_list_parent -> task_tcb_cycle_list_head = os_run_prio_tcb -> ready_task_next;
                } 
            } else {
                os_run_prio_tcb -> task_time_slice_over = os_run_prio_tcb -> task_time_slice_init;
                if (os_run_prio_tcb -> task_status == TASK_RUNNING)
                    os_run_prio_tcb -> task_status = TASK_READY;
                os_run_prio_tcb -> ready_task_list_parent -> task_tcb_cycle_list_head = os_run_prio_tcb -> ready_task_next;
            }
        }
    #if OS_CRITICAL_METHOD == 1
    } else {
        if (os_run_prio_tcb -> task_time_slice_over > 0)
            os_run_prio_tcb -> task_time_slice_over--;           /*为了保证任务的实时性，在关闭调度器的时候任务时间片也会消耗，但是关闭中断时则不会消耗*/
    }
    #endif
}

/*
 * 触发任务切换
 */
void os_task_switch_start(void)
{
    #if OS_CRITICAL_METHOD == 1
    if (os_task_control_en == 1) {
    #endif
        //if (os_high_prio_tcb != os_run_prio_tcb) {
			//os_high_prio_tcb -> task_cpu_use_num++;
            if (os_run_prio_tcb -> task_status == TASK_RUNNING)
                os_run_prio_tcb -> task_status = TASK_READY;
            os_high_prio_tcb -> task_status = TASK_RUNNING;
            //printf("os_high_name:%s\r\n", os_high_prio_tcb -> task_name);
            os_task_switch();
        //}
    #if OS_CRITICAL_METHOD == 1
    }
    #endif
}