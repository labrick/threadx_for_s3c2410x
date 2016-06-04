/**************************************************************************/ 
/*                                                                        */ 
/*      only for 32bit mode and dosn't support interrupt nesting          */
/*                                                                        */  
/* File name:               threadX_cpu.h								  */
/* Last modified Date:      2008-03-28									  */
/* Last Version:            0.01										  */
/* Descriptions:            header file     							  */
/* QQ:                      307102293									  */
/* ENAIL:                   wodexinxiang1949@163.com                      */
/*                                                                        */
/**************************************************************************/

#ifndef __THREADX_CPU_H
#define __THREADX_CPU_H

/**************************************************************************/
/*                                                                         */ 
/*     data struct for arm target                                          */ 
/*                                                                         */
/**************************************************************************/
typedef struct arm_stack_struct {
    unsigned int        cpsr;
    unsigned int        lr;
    unsigned int        r0;
    unsigned int        r1;
    unsigned int        r2;
    unsigned int        r3;
    unsigned int        r6;
    unsigned int        r4;
    unsigned int        r5;
    unsigned int        r7;
    unsigned int        r8;
    unsigned int        r9;
    unsigned int        r10;
    unsigned int        r11;
    unsigned int        r12;
    unsigned int        pc;
} ARM_STACK;

/**************************************************************************/
/*                                                                         */ 
/*     function of arm target for threadX                                  */ 
/*                                                                         */
/**************************************************************************/
extern void __tx_initialize_low_level   (void);
extern void _tx_timer_interrupt         (void);

#endif
/**************************************************************************/
/*     end of file                                                        */ 
/**************************************************************************/
