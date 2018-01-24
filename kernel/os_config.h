#ifndef __OS_CONFIG_H__
#define __OS_CONFIG_H__

#define MEM_BLOCK_SIZE 32        /*内存块大小为32字节*/
#define MEM_MAX_SIZE (10 * 1024) /*最大管理内存 15K*/
#define MEM_BLOCK_NUM (MEM_MAX_SIZE / MEM_BLOCK_SIZE)

#define OS_CRITICAL_METHOD 1 /*进入临界段的方法*/

#define OS_MIN_PRIO 255 /*最小优先级*/

/*
 * 系统任务定义
 */
/*空闲任务*/
#define IDLE_TASK_PRIO OS_MIN_PRIO
#define IDLE_TASK_STACK_SIZE 64 /*单位为CPU堆栈最小偏移大小*/
/*挂起任务检查任务*/
#define SYS_HANG_CHECK_TASK_PRIO 0
#define SYS_HANG_CHECK_TASK_STACK_SIZE 256 /*单位为CPU堆栈最小偏移大小*/
/*统计任务*/
#define OS_STATISTICS_TASK_PRIO 1
#define OS_STATISTICS_TASK_STACK_SIZE 256 /*单位为CPU堆栈最小偏移大小*/

#endif
