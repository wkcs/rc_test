#include "os_cpu.h"

#if OS_CRITICAL_METHOD == 1
    os_uint8 os_task_control_en = 1;            /*任务调度势能控制*/
    os_uint32 os_critical_num = 0;              /*阻止任务调度的深度*/
#endif

os_stk *task_stack_init(void (*task)(void *p_arg), void *p_arg, os_stk *task_stack_pointer)
{
    os_stk *stk;

    stk = task_stack_pointer;                                  

 
    *(stk) = (os_cpu_sr)0x01000000L;   
    *(--stk) = (os_cpu_sr)task;        
    *(--stk) = (os_cpu_sr)0xFFFFFFFEL; 
    *(--stk) = (os_cpu_sr)0x12121212L; 
    *(--stk) = (os_cpu_sr)0x03030303L; 
    *(--stk) = (os_cpu_sr)0x02020202L;
    *(--stk) = (os_cpu_sr)0x01010101L; 
    *(--stk) = (os_cpu_sr)p_arg;      

    
    *(--stk) = (os_cpu_sr)0x11111111L; 
    *(--stk) = (os_cpu_sr)0x10101010L;
    *(--stk) = (os_cpu_sr)0x09090909L; 
    *(--stk) = (os_cpu_sr)0x08080808L;
    *(--stk) = (os_cpu_sr)0x07070707L; 
    *(--stk) = (os_cpu_sr)0x06060606L; 
    *(--stk) = (os_cpu_sr)0x05050505L;
    *(--stk) = (os_cpu_sr)0x04040404L; 

    return (stk);
}

