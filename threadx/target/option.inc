;/**************************************************************************/ 
;/*                                                                        */ 
;/*      only for 32bit mode and dosn't support interrupt nesting          */
;/*                                                                        */  
;/* File name:               threadX_cpu.s								   */
;/* Last modified Date:      2008-03-28									   */
;/* Last Version:            0.01										   */
;/* Descriptions:            asm file for armtarget						   */
;/* QQ:                      307102293									   */
;/* ENAIL:                   wodexinxiang1949@163.com                      */
;/*                                                                        */
;/**************************************************************************/

;/* mode config  */
Mode_USR            EQU         0x10                                   ;/* 用户模式                     */
Mode_FIQ            EQU         0x11                                   ;/* 快中断模式                   */
Mode_IRQ            EQU         0x12                                   ;/* 中断模式                     */
Mode_SVC            EQU         0x13                                   ;/* 管理模式                     */
Mode_ABT            EQU         0x17                                   ;/* 中止模式                     */
Mode_UND            EQU         0x1B                                   ;/* 未定义模式                   */
Mode_SYS            EQU         0x1F                                   ;/* 系统模式                     */

NO_FIQ              EQU         0X80
NO_IRQ              EQU         0X80
