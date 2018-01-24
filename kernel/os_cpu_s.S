.extern  os_running                                           @ External references.
.extern  os_high_prio_tcb
.extern  os_run_prio_tcb


.global  os_start_hign_prio_task                                     
.global  os_task_switch
.global  os_task_switch_frq
.global  os_cpu_sr_save                                      
.global  os_cpu_sr_restore 
.global  PendSV_Handler


.equ NVIC_INT_CTRL,   0xE000ED04                              @ Interrupt control state register.
.equ NVIC_SYSPRI14,   0xE000ED22                              @ System priority register (priority 14).
.equ NVIC_PENDSV_PRI,     0xFFFF                              @ PendSV priority value (lowest).
.equ NVIC_PENDSVSET,  0x10000000                              @ Value to trigger PendSV exception.


.text
.align 2
.thumb
.syntax unified



.thumb_func
os_cpu_sr_save:
        MRS     R0, PRIMASK                     @ Set prio int mask to mask all (except faults)
        CPSID   I 
        BX      LR

.thumb_func
os_cpu_sr_restore:                                 @ See Note #2.
        MSR     PRIMASK, R0
        BX      LR


.thumb_func
os_start_hign_prio_task:
        LDR     R4, =NVIC_SYSPRI14      @ set the PendSV exception priority
        LDR     R5, =NVIC_PENDSV_PRI
        STR     R5, [R4]

        MOV     R4, #0                 @ set the PSP to 0 for initial context switch call
        MSR     PSP, R4

        LDR     R4, =os_running         @ os_running = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

                                       @切换到最高优先级的任务
        LDR     R4, =NVIC_INT_CTRL     @rigger the PendSV exception (causes context switch)
        LDR     R5, =NVIC_PENDSVSET
        STR     R5, [R4]

        CPSIE   I                      @enable interrupts at processor level
os_start_hang:
        B       os_start_hang          @should never get here



.thumb_func
PendSV_Handler:
    CPSID   I                                                   @ Prevent interruption during context switch
    MRS     R0, PSP                                             @ PSP is process stack pointer 如果在用PSP堆栈,则可以忽略保存寄存器,参考CM3权威中的双堆栈-白菜注
    CBZ     R0, PendSV_Handler_Nosave		                    @ Skip register save the first time

    SUBS    R0, R0, #0x20                                       @ Save remaining regs r4-11 on process stack
    STM     R0, {R4-R11}

    LDR     R1, =os_run_prio_tcb                                @ OSTCBCur->OSTCBStkPtr = SP@
    LDR     R1, [R1]
    STR     R0, [R1]                                            @ R0 is SP of process being switched out

                                                                @ At this point, entire context of process has been saved
PendSV_Handler_Nosave:
    LDR     R0, =os_run_prio_tcb                                @ os_run_prio_tcb  = os_high_prio_tcb@
    LDR     R1, =os_high_prio_tcb
    LDR     R2, [R1]
    STR     R2, [R0]

    LDR     R0, [R2]                                            @ R0 is new process SP@ SP = OSTCBHighRdy->OSTCBStkPtr@
    LDM     R0, {R4-R11}                                        @ Restore r4-11 from new process stack
    ADDS    R0, R0, #0x20
    MSR     PSP, R0                                             @ Load PSP with new process SP
    ORR     LR, LR, #0x04                                       @ Ensure exception return uses process stack
    CPSIE   I
    BX      LR                                                  @ Exception return will restore remaining context

.end


