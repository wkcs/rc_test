#ifndef __OS_DEFINE_H__
#define __OS_DEFINE_H__

#ifndef NULL
#define NULL 0
#endif

#ifndef OS_ENABLE
#define OS_ENABLE 1
#endif

#ifndef OS_DISENABLE
#define OS_DISENABLE 0
#endif

typedef enum {
    NO_STATUS = 0,
    TASK_RUNNING,
    TASK_SLEEP,
    TASK_READY,
    TASK_HANG,
    TASK_HANG_TIME
} TASK_STATUS;

typedef enum {
    NO_WAIT = 0x00000000,
    WAIT_DELAY = 0x00000001,
    WAIT_SIGNAL = 0x00000002,
    WAIT_SEM = 0x00000003
} TASK_WAIT_FLAG;

typedef enum {
    NOT_IRQ = 0,
    IS_IRQ = 1
} IRQ_FLAG;

typedef enum {
    ALL_TASK_LIST = 0X01,
    READY_TASK_LIST = 0X02,
    HANG_TASK_LIST = 0X04,
    SLEEP_TASK_LIST = 0X08,
    WAIT_TIME_TASK_LIST = 0X10
} TASK_LIST;

#endif

