/**************************************************************************/ 
/*                                                                        */ 
/*      only for 32bit mode and dosn't support interrupt nesting          */
/*                                                                        */  
/* File name:               threadX_cpu.c								  */
/* Last modified Date:      2008-03-28									  */
/* Last Version:            0.01										  */
/* Descriptions:            											  */
/* QQ:                      307102293									  */
/* ENAIL:                   wodexinxiang1949@163.com                      */
/*                                                                        */
/**************************************************************************/

/* Include necessary system files.  */
#include <stdio.h>
#include "Target.h"
#include "../os_kern/tx_api.h"
#include "../os_kern/tx_thr.h"
#include "../os_kern/tx_tim.h"
#include "../os_kern/tx_ini.h"
#include "../os_kern/tx_blo.h"
#include "threadX_cpu.h"

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/**     macro Config                                                      */ 
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define __TX_DYNAMIC_MEM_SIZE		    1 * 1024 * 1024
#define __TX_THREAD_TIMER_STKSIZE       512

    /*  dynamic memery declear  */
    int  _tx_initialize_first_memory[__TX_DYNAMIC_MEM_SIZE];

    /*  stack for timer thread  */
    int  _tx_timer_stack[__TX_THREAD_TIMER_STKSIZE];

    /*  declear for complier.   none used   */
    char    __ghsbegin_events[100];
    char    __ghsend_events[100];
    
    /*  pointer to system stack */
    extern int *__ptx_sys_statck;

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/**     function for weak complier                                        */ 
/**                                                                       */
/**************************************************************************/
/**************************************************************************/
void        __cpp_exception_init(void **pp){}
void        __cpp_exception_cleanup(void **pp){}

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _tx_initialize_low_level                                            */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function is responsible for any low-level processor            */ 
/*    initialization, including setting up interrupt vectors, saving the  */ 
/*    system stack pointer, finding the first available memory address,   */ 
/*    and setting up parameters for the system's timer thread.            */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    _tx_initialize_kernel_enter           ThreadX entry function        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/
/*void        _tx_initialize_low_level(void)
{
    _tx_thread_system_stack_ptr     = (VOID_PTR)__ptx_sys_statck;
    _tx_timer_stack_start           = &_tx_timer_stack[0];
    _tx_timer_stack_size            = sizeof(_tx_timer_stack);
    _tx_timer_priority              = 0;
    _tx_initialize_unused_memory    = (VOID_PTR) &_tx_initialize_first_memory[0];    
    __tx_initialize_low_level();
}*/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _tx_timer_interrupt                                                 */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function processes the hardware timer interrupt.  This         */ 
/*    processing includes incrementing the system clock and checking for  */ 
/*    time slice and/or timer expiration.  If either is found, the        */ 
/*    interrupt context save/restore functions are called along with the  */ 
/*    expiration functions.                                               */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _tx_thread_context_save               Save interrupted context      */ 
/*    _tx_thread_resume                     Resume timer processing thread*/ 
/*    _tx_thread_time_slice                 Time slice interrupted thread */ 
/*    _tx_thread_context_restore            Restore interrupted context   */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    interrupt vector                                                    */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/ 
VOID   _tx_timer_interrupt(VOID)
{
    _tx_timer_system_clock++;                                           /*  系统滴答时间加              */
    if (_tx_timer_time_slice) {                                         /*  如果有剩余的时间片          */
		_tx_timer_time_slice--;                                         /*  时间片调整、减一            */
		if (_tx_timer_time_slice == 0) {                                /*  如果当前时间片耗尽          */
			_tx_timer_expired_time_slice =  TX_TRUE;                    /*  设置标识                    */
		}
    }

    if (*_tx_timer_current_ptr) {                                       /*  是否有定时器需要处理        */
        _tx_timer_expired =  TX_TRUE;
    } else {
        _tx_timer_current_ptr++;                                        /*  指向下一时刻定时器就绪链表头*/
        if (_tx_timer_current_ptr == _tx_timer_list_end){
            _tx_timer_current_ptr =  _tx_timer_list_start;
		}
    }

    if ((_tx_timer_expired_time_slice) || (_tx_timer_expired)) {        /*  有事情要做                  */
		if (_tx_timer_expired) {
		    _tx_timer_expired   = TX_FALSE;
			_tx_thread_preempt_disable++;
		    _tx_thread_resume(&_tx_timer_thread);                       /*  回复定时器线程              */
		}
		if (_tx_timer_expired_time_slice) {
    		_tx_timer_expired_time_slice    = TX_FALSE;
			if (_tx_thread_time_slice() == TX_FALSE) {                  /*  不需要切换，载入分配的时间片*/
				_tx_timer_time_slice =  _tx_thread_current_ptr -> tx_time_slice;
			}
		 } 
    }
}

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _tx_thread_stack_build                                              */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function builds a stack frame on the supplied thread's stack.  */
/*    The stack frame results in a fake interrupt return to the supplied  */
/*    function pointer.                                                   */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    thread_ptr                            Pointer to thread control blk */
/*    function_ptr                          Pointer to return function    */
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    _tx_thread_create                     Create thread service         */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/ 
void        _tx_thread_stack_build(TX_THREAD *thread_ptr, void (*function_ptr)(void))
{    
    ARM_STACK    *ArmRegister   = 0;
    
    ArmRegister         = (ARM_STACK *)(((int)thread_ptr->tx_stack_end - sizeof(ARM_STACK) - 1) & 0xfffffffc);
    
    ArmRegister->cpsr   = MODE_USR32;
    ArmRegister->lr     = (int)(thread_ptr->tx_thread_entry);    
    ArmRegister->r0     = (unsigned int)thread_ptr->tx_entry_parameter;    
    ArmRegister->r1     = 0;    
    ArmRegister->r2     = 0;    
    ArmRegister->r3     = 0;    
    ArmRegister->r4     = 0;    
    ArmRegister->r5     = 0;    
    ArmRegister->r6     = 0;    
    ArmRegister->r7     = 0;    
    ArmRegister->r8     = 0;    
    ArmRegister->r9     = 0;    
    ArmRegister->r10    = 0;    
    ArmRegister->r11    = 0;    
    ArmRegister->r12    = 0;    
    ArmRegister->pc     = (int)(thread_ptr->tx_thread_entry);    

    thread_ptr->tx_stack_ptr    = (VOID_PTR)ArmRegister;
}
/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    __tx_thread_contex_restore                                          */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    When this function has been called, scheduler should be processing  */
/*    here. If no thread was in order, we execute a loop until a thread   */
/*    was actived by interrupt handle. Golbal variable 				      */
/*    _tx_thread_current_ptr was point to the active thread when return.  */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    __tx_thread_swi_system_return                                       */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/ 
void        __tx_thread_contex_restore(void)
{
    _tx_thread_current_ptr 	= _tx_thread_execute_ptr;                   /*  指向新线程地址              */
    _tx_thread_current_ptr->tx_run_count++;                             /*  调度增加                    */
    _tx_timer_time_slice 	= _tx_thread_current_ptr->tx_time_slice     /*  新的时间片                  */
                            ? _tx_thread_current_ptr->tx_time_slice
                            : _tx_thread_current_ptr->tx_new_time_slice;
}

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    __tx_thread_contex_save                                             */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    Save the time slice                                                 */
/*  																	  */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    __tx_thread_swi_system_return                                       */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/ 
void        __tx_thread_contex_save(void)
{
    if (_tx_thread_current_ptr != TX_NULL) {
        _tx_thread_current_ptr->tx_time_slice   = _tx_timer_time_slice
                                                ? _tx_timer_time_slice
                                                : _tx_thread_current_ptr->tx_new_time_slice;
    }
}

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _tx_thread_preempt_check                        68332/Green Hills   */ 
/*                                                           3.0a         */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    William E. Lamie, Express Logic, Inc.                               */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function checks for a preempt condition that might have taken  */ 
/*    place on top of an optimized assembly language ISR.  If preemption  */ 
/*    should take place, context save/restore processing is called.	      */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    0:    Current thread is active                                      */ 
/*    1:    A new thread is active but contex has saved yet               */
/*    2:    A new thread is active and contex havn't been saved           */
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*   __tx_thread_swi_system_return                                        */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*                                                                        */ 
/**************************************************************************/ 
int        __tx_thread_preempt_check (void)
{
#if 1
	if (_tx_thread_execute_ptr->tx_stack_ptr > 
            _tx_thread_execute_ptr->tx_stack_end + 64||
            ((_tx_thread_execute_ptr->tx_stack_ptr <
            _tx_thread_execute_ptr->tx_stack_start)&&(_tx_thread_execute_ptr->tx_stack_ptr!=0)))
           {
				printf("**************************************************STACK ERROR*******************************************\n");
				printf("execute_ptr name:%s\n",_tx_thread_execute_ptr->tx_thread_name);
            	printf("tx_stack_start:%x\ntx_stack_ptr:%x\ntx_stack_end:%x\n",
						_tx_thread_execute_ptr->tx_stack_start,
						_tx_thread_execute_ptr->tx_stack_ptr, 
						_tx_thread_execute_ptr->tx_stack_end);
          	    while(1);
           }
#endif




    if (_tx_thread_execute_ptr == _tx_thread_current_ptr) {
      //   Printf("__tx_thread_preempt_check return 0\n");
        return 0;
    } else {
    		// Printf("__tx_thread_preempt_check 2\n");
    		if(_tx_thread_execute_ptr==0)
    		return 0;
        	else if (_tx_thread_execute_ptr->tx_stack_ptr > 
            _tx_thread_execute_ptr->tx_stack_end ||
            _tx_thread_execute_ptr->tx_stack_ptr <
            _tx_thread_execute_ptr->tx_stack_start){
          //  while(1)
           {
             	printf("_tx_thread_current_ptr,%x\n_tx_thread_execute_ptr,%x\n",_tx_thread_current_ptr,_tx_thread_execute_ptr);
            	printf(" _tx_thread_preempt_check loop\n");
       			printf("_tx_thread_execute_ptr->tx_stack_start:%x\n _tx_thread_execute_ptr->tx_stack_ptr:%x\n _tx_thread_execute_ptr->tx_stack_end:%x\n",_tx_thread_execute_ptr->tx_stack_end,_tx_thread_execute_ptr->tx_stack_ptr, _tx_thread_execute_ptr->tx_stack_end);
          	    while(1);
           }
        }

        if (_tx_thread_current_ptr->tx_stack_ptr != 0) {
        // Printf("__tx_thread_preempt_check return 1\n");
            return 1;
        } else {
       // Printf("__tx_thread_preempt_check return 2\n");
            return 2;
        } 
    }
}

/**************************************************************************/
/*     end of file                                                        */ 
/**************************************************************************/
