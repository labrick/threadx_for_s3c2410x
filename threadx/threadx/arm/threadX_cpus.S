/**************************************************************************/ 
/*                                                                        */ 
/*      only for 32bit mode and dosn't support interrupt nesting          */
/*                                                                        */  
/* File name:               threadX_cpu.s								   */
/* Last modified Date:      2008-03-28									   */
/* Last Version:            0.01										   */
/* Descriptions:            asm file for armtarget						   */
/* QQ:                      307102293									   */
/* ENAIL:                   wodexinxiang1949@163.com                      */
/*                                                                        */
/**************************************************************************/
//	PRESERVE8

/* Include necessary system files.  */
#define	Mode_USR			 		0x10								   /* 用户模式 					*/
#define	Mode_FIQ			 		0x11								   /* 快中断模式					*/
#define	Mode_IRQ			 		0x12								   /* 中断模式 					*/
#define	Mode_SVC			 		0x13								   /* 管理模式 					*/
#define	Mode_ABT			 		0x17								   /* 中止模式 					*/
#define	Mode_UND			 		0x1B								   /* 未定义模式					*/
#define	Mode_SYS			 		0x1F								   /* 系统模式 					*/
	
#define	NO_FIQ				 		0X80
#define	NO_IRQ				 		0X80


        .extern __irq_handle
        .extern __ptx_sys_statck
        .extern __ptx_svc_statck
        .extern __ptx_irq_statck
        .extern __ptx_fiq_statck
        .extern __ptx_about_statck
        .extern __ptx_undef_statck

        .extern _tx_thread_current_ptr
        .extern _tx_thread_execute_ptr
        .extern _tx_thread_system_stack_ptr
        .extern __ptx_sys_statck
        .extern __ptx_svc_statck
        
        .extern __tx_thread_contex_save
        .extern __tx_thread_contex_restore
        .extern __tx_thread_preempt_check
        
        .global __tx_thread_swi_disable_irq
        .global __tx_thread_swi_enable_irq
        .global __tx_thread_swi_schedule
        .global __tx_thread_swi_system_return
		.global __tx_initialize_low_level
		.global __tx_thread_restore
		
		.global __thread_irq_handle
		.global __thread_swi_handle


__thread_swi_handle:
    LDR     SP,         =__ptx_svc_statck
    LDR     SP,         [SP]
   	CMP     R0,         #4
    addlo   PC,         PC, R0, LSL #2
    B       .
    b		__tx_thread_swi_disable_irq
    b		__tx_thread_swi_enable_irq
    b		__tx_thread_swi_schedule
    b		__tx_thread_swi_system_return
//SwiFunction:
//    DCD     __tx_thread_swi_disable_irq
//    DCD     __tx_thread_swi_enable_irq
//    DCD		__tx_thread_swi_schedule
//    DCD     __tx_thread_swi_system_return


__tx_thread_swi_disable_irq:                                         
        STMFD   SP!,    {LR}
        MRS     R0,     SPSR
        ORR     R1,     R0,     #NO_IRQ             //  设置禁止中断标识
        MSR     SPSR_cxsf,      R1
        LDMFD   SP!,    {PC}^                       //  退出


__tx_thread_swi_enable_irq:
        STMFD   SP!,    {LR}
        MSR     SPSR_cxsf,      R1
        LDMFD   SP!,    {PC}^                                      //  退出


__tx_thread_swi_system_return:
   		MRS     R0,         SPSR
	    MOV     R1,         LR
	    MSR     CPSR_cxsf,  #(Mode_SYS | NO_IRQ)
	    STMFD   SP!,    {R1}                                    // PC_pre
	    STMFD   SP!,    {R0-R12, LR}							// reg
	    STMFD   SP!,    {R0}									// cpsr
	    
	    LDR     R0,     =_tx_thread_current_ptr
	    LDR     R0,     [R0]
	    STR     SP,     [R0,    #8]
	    //LDR    PC,     =__tx_thread_restore
	   //b LED4;

__tx_thread_restore:    
    MSR     CPSR_cxsf,  #(Mode_SYS | NO_IRQ)
    LDR     SP,     =__ptx_sys_statck
    LDR     SP,     [SP]

    LDR     R0,     =_tx_thread_execute_ptr
    LDR     R0,     [R0]
   	CMP     R0,    #0
   	BEQ		__tx_thread_restore_loop
    LDR     R0,     =_tx_thread_execute_ptr
    LDR     R0,     [R0]
    LDR     R1,     =_tx_thread_current_ptr
    LDR     R1,     [R1]
    CMP     R0,     R1
    BNE     __tx_thread_shedu
    MSR     CPSR_cxsf,   #(Mode_SYS)
__tx_thread_restore_loop:
	MSR     CPSR_cxsf,   #(Mode_SYS)
	LDR     R0,     =_tx_thread_execute_ptr
    LDR     R0,     [R0]
  	CMP     R0,    #0
   	BEQ		__tx_thread_restore_loop
    BNE     __tx_thread_shedu   
    
__tx_thread_shedu:    
    BL      __tx_thread_contex_restore
    
    LDR     R0,     =_tx_thread_current_ptr
    MOV     R2,     #0
    LDR     R0,     [R0]
    LDR     R1,     [R0,    #8]
    STR     R2,     [R0,    #8]
    LDMFD   R1!,    {R0,    LR}
    ADD     SP,     R1,     #4*14
    
    MSR     CPSR_cxsf,      #(Mode_SVC | NO_IRQ)
    MOV     SP,             R1
    MSR     SPSR_cxsf,      R0
    LDMFD   SP!,    {R0-R12, PC}^


__tx_thread_swi_schedule:
    LDR     PC,     =__tx_thread_restore
/***
void        _tx_initialize_low_level(void)
{
    _tx_thread_system_stack_ptr     = (VOID_PTR)__ptx_sys_statck;
    _tx_timer_stack_start           = &_tx_timer_stack[0];
    _tx_timer_stack_size            = sizeof(_tx_timer_stack);
    _tx_timer_priority              = 0;
    _tx_initialize_unused_memory    = (VOID_PTR) &_tx_initialize_first_memory[0];    
    __tx_initialize_low_level();
}
***/	
		.global _tx_initialize_low_level
_tx_initialize_low_level:
	ldr		r0,		=_tx_thread_system_stack_ptr
	ldr		r1,		=__ptx_sys_statck
	ldr		r1,		[r1]
	str		r1,		[r0]

	ldr		r0,		=_tx_timer_stack_start
	ldr		r1,		=_tx_timer_stack
	str		r1,		[r0]

	ldr		r0,		=_tx_timer_stack_size
	mov		r1,		#2048
	str		r1,		[r0]
	
	ldr		r0,		=_tx_timer_priority
	mov		r1,		#0
	str		r1,		[r0]

	ldr		r0,		=_tx_initialize_unused_memory
	ldr		r1,		=_tx_initialize_first_memory
	str		r1,		[r0]
	
//__tx_initialize_low_level:
	MOV             R0,     LR
    MSR             CPSR_c, #(Mode_SVC | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_svc_statck
    ldr             sp,     [sp]
       
    MSR             CPSR_c, #(Mode_IRQ | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_irq_statck
    ldr             sp,     [sp]
        
    MSR             CPSR_c, #(Mode_FIQ | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_fiq_statck
    ldr             sp,     [sp]
    MSR             CPSR_c, #(Mode_ABT | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_about_statck
    ldr             sp,     [sp]
    MSR             CPSR_c, #(Mode_UND | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_undef_statck
    ldr             sp,     [sp]
    MSR             CPSR_c, #(Mode_SYS | NO_FIQ | NO_IRQ)
    ldr             sp,     =__ptx_sys_statck
    ldr             sp,     [sp]
        
    MOV				LR,		R0
    MOV             PC,     LR


__thread_irq_handle:
        SUB     LR,     LR,     #4                      // 计算返回地址
        STMFD   SP!,    {R0-R3, R12, LR}
		 
        // 执行中断服务程序
        // __irq_handle();
        //
        //BL     irq_handle

     	bl common_irq_handler;
     //	cmp r1,#0
     //	movhs r0,r0
     //	cmp r0,r1
     //	movlo r0,r1
     //	bl PrintAsm
     
//after_cmp:    
 //       mov lr,pc
 //       mov pc, r0  
 
       // LDMFD SP!,{r0-r3,r12,pc}^   
        
        // int   ret = _tx_thread_check_preep();
        // switch (ret) {
        // case 0:   __irq_exit_sample();
        // case 1:   __irq_thread_swith_save_no();
        // case 2:   __irq_thread_swich_save_all();
        BL      __tx_thread_preempt_check
      //  B LED2;
        CMP     R0,         #4
        addlo   PC,         PC, R0, LSL #2
		b		.
		b		__irq_exit_sample
		b		__irq_thread_swith_save_no
	 	b		__irq_thread_swich_save_all


__irq_exit_sample:
        LDMFD   SP!,        {R0-R3, R12, PC}^        


//__irq_thread_exit:
//        DCD     __irq_exit_sample
//        DCD		__irq_thread_swith_save_no
//        DCD     __irq_thread_swich_save_all


        
__irq_thread_swith_save_no:                
        ADD     SP,     SP,     #4*6
        MSR     CPSR_cxsf,      #(Mode_SYS | NO_IRQ)        
        B       __irq_thread_swith_exit
        
__irq_thread_swich_save_all:			
        MRS     R0,     SPSR				//r0
        STMFD   SP!,    {R0}
        ADD     SP,     SP,     #4*7
        SUB     R0,     SP,		#4			// SPSR R0 R1 R2 R3 R12 PC
                                            //                       ^
        									//					   R0
        MSR     CPSR_cxsf,      #(Mode_SYS | NO_IRQ)
        LDMFA   R0!,    {R1}				//                  R0
        STMFD   SP!,    {R1}                // 
        									//	    		    ^
        LDMFA   R0!,    {R1-R3,  R12}		//      			   R0
        STMFD   SP!,    {R1-R12    }        //	   ^
        LDMFA   R0!,    {R1-R2}		        //	  R0
        STMFD   SP!,    {R2}        		//
        
        STMFD   SP!,    {R1,     LR}        //
        
        
        ldr     r0,     =_tx_thread_current_ptr
        ldr     r0,     [r0]
        STR     sp,     [r0,    #8]
        
__irq_thread_swith_exit:
        LDR     PC,     =__tx_thread_restore

 #if 0       
irq_handle:
        STMFD SP!,{r0-r3,r12,lr}           //保存现场  
        ldr lr, =irq_return               //设置中断异常处理程序返回地址到下面的位置  
       	ldr r0,=0x71200f00
       	ldr r0,[r0]
       	mov pc, r0                //直接进入到中断函数处理  
irq_return:  
        LDMFD SP!,{r0-r3,r12,lr}           //恢复现场  
        SUBS PC,LR,#4 
 #endif       

        

;/**************************************************************************/
;/*     end of file                                                        */ 
;/**************************************************************************/
