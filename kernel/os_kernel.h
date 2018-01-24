#ifndef __OS_KERNLE_H__
#define __OS_KERNLE_H__

#include "os_cpu.h"
//#include "os_task.h"

#define _task_tcb_cycle_list _task_tcb         /*相同优先级就绪任务的循环列表*/
#define _hang_task_list _task_tcb
#define _wait_time_task_list _task_tcb
#define _sleep_task_list _task_tcb

typedef void (*task)(void *p_arg);

struct __task_tcb;
struct __ready_task_list_main;
struct __ready_task_list_prio;

/*
 * 任务控制块
 */
typedef struct __task_tcb
{
    /* 任务栈顶指针*/
    os_stk *task_stack_pointer;
    /* 任务栈起始地址*/
    os_stk *task_stack_start_addr;
    /* 任务栈大小*/
    os_uint32 task_stack_size;   
    /*任务优先级*/          
	os_uint8 task_priority;
    /*任务优先级初始值*/          
	//os_uint8 task_priority_init;
    /*任务id*/          
	os_uint32 task_id;          
    /*任务名*/
	os_uint8 *task_name;
    /*任务入口函数*/
	task task_addr;
    /*任务状态*/	
    os_uint8 task_status;
    /*任务时间片初始值*/
    os_uint32 task_time_slice_init;
    /*任务时间片剩余*/
    os_uint32 task_time_slice_over;
    /*任务等待事件标志*/
    os_cpu_sr task_wait_flag;
    /*任务延时剩余时间*/
    os_uint32 task_delay_time_over;
    /*任务CPU使用计数*/
    os_uint16 task_cpu_use_num;
    /*任务CPU使用率*/
    os_uint16 task_cpu_usage;
    /*任务当前所在列表*/
    os_uint8 task_list;
    /*任务等待信号量地址*/
    //os_sem_t *task_sem;
    /*索引级就绪任务列表表项*/
    struct __ready_task_list *ready_task_list_parent;
    struct __task_tcb *next;
    struct __task_tcb *last;
    struct __task_tcb *ready_task_next;
    struct __task_tcb *ready_task_last;
    struct __task_tcb *hang_task_next;
    struct __task_tcb *hang_task_last;
    struct __task_tcb *sleep_task_next;
    struct __task_tcb *sleep_task_last;
    /*为时间相关的等待任务创建，以便在临界区也可以操作*/
    struct __task_tcb *wait_time_task_next;
    struct __task_tcb *wait_time_task_last;
}_task_tcb;



/*
 * 就绪任务列表
 */
typedef struct __ready_task_list {
    _task_tcb_cycle_list *task_tcb_cycle_list_head;
    os_uint8 task_priority;
    /*就绪表保护选项，被保护时禁止删除*/
    os_uint8 protection_flag;
    os_uint16 task_hang_num;
    struct __ready_task_list *next;
    struct __ready_task_list *last;
}_ready_task_list;

/*
 * 系统统计结构体
 */
typedef struct {
    os_uint16 os_cpu_usage;
    os_uint32 os_cpu_use_num;
}_os_statistical_struct;

/*总任务列表*/
extern _task_tcb *task_tcb_list_head, *task_tcb_list_end;
/*就绪任务列表中优先级最高的非空表*/
extern _ready_task_list *ready_task_list_high_prio;

extern _ready_task_list *ready_task_list_head, *ready_task_list_end;
/*挂起任务列表*/
extern _hang_task_list *hang_task_list_head, *hang_task_list_end;
/*睡眠任务列表*/
extern _sleep_task_list *sleep_task_list_head, *sleep_task_list_end;
/*时间相关挂起任务列表*/
extern _wait_time_task_list *wait_time_task_list_head, *wait_time_task_list_end;

/*系统最高优先级就绪任务任务控制块和当前运行任务任务控制块*/
extern _task_tcb *os_high_prio_tcb, *os_run_prio_tcb;
/*挂起任务检查任务的任务控制块*/
extern _task_tcb *sys_hang_check_task_tcb;
/*空闲任务的任务控制块*/
extern _task_tcb *idle_task_tcb;
/*统计任务的任务控制块*/
extern _task_tcb *os_statistics_task_tcb;


extern os_uint8 os_running;

extern _os_statistical_struct os_statistical_struct;

/*初始化系统内核*/
os_int8 kernel_init(void);
/*初始化系统内核*/
void kernel_start(void);

#endif

