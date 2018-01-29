#ifndef __OS_CPU_H__
#define __OS_CPU_H__


/*
 * 定义与编译器无关的数据类型
 */

typedef unsigned char os_bool;
typedef unsigned char os_uint8;
typedef signed char os_int8;
typedef unsigned short os_uint16;
typedef signed short os_int16;
typedef unsigned int os_uint32;
typedef signed int os_int32;
typedef float os_fp32;
typedef double os_fp64;

typedef unsigned int os_stk;
typedef unsigned int os_cpu_sr;

/*os_stk的字节数*/
#define OS_STK_SIZE 4   

#ifndef  NVIC_INT_CTRL
#define  NVIC_INT_CTRL                      *((os_cpu_sr *)0xE000ED04)
#endif

#ifndef  NVIC_PENDSVSET
#define  NVIC_PENDSVSET                                    0x10000000
#endif

#define  os_task_switch()               NVIC_INT_CTRL = NVIC_PENDSVSET
#define  os_task_switch_frq()           NVIC_INT_CTRL = NVIC_PENDSVSET                   

os_cpu_sr os_cpu_sr_save(void);
void os_cpu_sr_restore(os_cpu_sr reg_save);
void os_start_hign_prio_task(void);

#if OS_CRITICAL_METHOD == 1
extern os_uint8 os_task_control_en; /*任务调度势能控制*/
extern os_uint32 os_critical_num;   /*阻止任务调度的深度*/
#endif

#if OS_CRITICAL_METHOD == 1
#define enter_critical()        \
    {                           \
        os_critical_num++;      \
        os_task_control_en = 0; \
    }

#define exit_critical()             \
    {                               \
        if (os_critical_num > 0)    \
            os_critical_num--;      \
        if (os_critical_num == 0)   \
            os_task_control_en = 1; \
    }
#endif

os_stk *task_stack_init(void (*task)(void *p_arg), void *p_arg, os_stk *task_stack_pointer);

#endif
