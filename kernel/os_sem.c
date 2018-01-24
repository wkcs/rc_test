// #include "os_sem.h"
// #include "os_mem.h"

// os_sem_t *creat_sem(void)
// {
//     os_sem_t *sem;
//     enter_critical();
//     sem = (os_sem_t *)mem_malloc(sizeof(os_sem_t), 0);
//     sem -> priority = 0;
//     sem -> sem = 1;
//     sem -> wait_sem_task_list_head = NULL;
//     exit_critical();
//     return sem;
// }

// os_int8 apply_sem(_task_tcb *task_tcb, os_sem_t *sem)
// {
//     os_int8 status;
//     _task_tcb *wait_sem_task_list, *wait_sem_task_list_head;
//     if(_task_tcb != NULL)
//     {
//         if(sem -> sem > 0)
//         {
//             sem -> priority = task_tcb -> task_priority;
//             (sem -> sem)--;
//             sem -> wait_sem_task_list_head = NULL;
//         } 
//         else
//         {
//             enter_critical();
//             if (task_tcb -> task_list & READY_TASK_LIST == READY_TASK_LIST) {         /*检查任务状态，睡眠任务不能被挂起*/
//                 status = delete_task_for_rtl_s(task_tcb);
//                 if (status != 0) {
//                     exit_critical();
//                     return status;
//                 }
//                 ready_task_list = task_tcb -> ready_task_list_parent;
//                 ready_task_list -> task_hang_num++;
//                 ready_task_list_lock(ready_task_list);
//                 task_tcb -> task_status = TASK_HANG;
//                 task_tcb -> task_wait_flag = WAIT_SEM;
//                 if (sem -> wait_sem_task_list_head != NULL) 
//                 {
//                     wait_sem_task_list = sem -> wait_sem_task_list_head;
//                     while(wait_sem_task_list != NULL)
//                     {
//                         if (wait_sem_task_list -> task_priority > sem -> priority)
//                             break;
//                         if (wait_sem_task_list -> hang_task_next == NULL)
//                         {
//                             task_tcb -> hang_task_next = NULL;
//                             task_tcb -> hang_task_last = wait_sem_task_list;
//                             wait_sem_task_list -> hang_task_next = task_tcb;
//                         }
//                         wait_sem_task_list = wait_sem_task_list -> hang_task_next;

//                     }
//                     if (wait_sem_task_list == sem -> wait_sem_task_list_head)
//                     {
//                         task_tcb -> hang_task_next = wait_sem_task_list;
//                         task_tcb -> hang_task_last = NULL;
//                         wait_sem_task_list -> hang_task_last = task_tcb;
//                         sem -> wait_sem_task_list_head = task_tcb;
//                     } 
//                     else if (wait_sem_task_list != NULL)
//                     {
//                         task_tcb -> hang_task_next = wait_sem_task_list;
//                         task_tcb -> hang_task_last = wait_sem_task_list -> hang_task_last;
//                     }
//                 }
//                 else
//                 {
//                     task_tcb -> hang_task_next = NULL;
//                     task_tcb -> hang_task_last = NULL;
//                     sem -> wait_sem_task_list_head = task_tcb;
//                 }
//                 task_tcb -> task_list |= HANG_TASK_LIST;
//             //if (sem -> priority < task_tcb -> task_priority)
//         }
//     }
// }

// os_int8 release_sem(_task_tcb *task_tcb, os_sem_t *sem)
// {
//     os_sem_t *sem;
//     sem = (os_sem_t *)mem_malloc(sizeof(os_sem_t), 0);
//     sem -> priority = 0;
//     sem -> sem = 1;
//     sem -> wait_sem_task_list_head = NULL;

//     return sem;
// }