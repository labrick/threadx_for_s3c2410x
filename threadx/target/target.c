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
//#include "config.h"

#define __TX_SYS_STATCK             1024
#define __TX_SVC_STATCK             1024
#define __TX_IRQ_STATCK             1024
#define __TX_FIQ_STATCK             512
#define __TX_ABOUT_STATCK           512
#define __TX_UDEF_STATCK            512

static int     _tx_SYS_statck[__TX_SYS_STATCK];
static int     _tx_SVC_statck[__TX_SVC_STATCK];
static int     _tx_IRQ_statck[__TX_IRQ_STATCK];
static int     _tx_FIQ_statck[__TX_FIQ_STATCK];
static int     _tx_ABOUT_tatck[__TX_ABOUT_STATCK];
static int     _tx_UNDEF_statck[__TX_UDEF_STATCK];

int    *__ptx_sys_statck    = &_tx_SYS_statck[__TX_SYS_STATCK - 1];
int    *__ptx_svc_statck    = &_tx_SVC_statck[__TX_SVC_STATCK - 1];
int    *__ptx_irq_statck    = &_tx_IRQ_statck[__TX_IRQ_STATCK - 1];
int    *__ptx_fiq_statck    = &_tx_FIQ_statck[__TX_FIQ_STATCK - 1];
int    *__ptx_about_statck  = &_tx_ABOUT_tatck[__TX_ABOUT_STATCK - 1];
int    *__ptx_undef_statck  = &_tx_UNDEF_statck[__TX_UDEF_STATCK - 1];


//__align (0x40000) unsigned char    cFramBuffer[640][480][2];
//unsigned char   *pucFrameBuffer =  &cFramBuffer[0][0][0];
/**************************************************************************/
/*                                                                        */ 
/*                                                                        */ 
/*                                                                        */
/**************************************************************************/
/*void Timer0_Exception (void)
{
    _tx_timer_interrupt();
    rSRCPND 	= 1<<10;
    rINTPND 	= rINTPND;
}

void  tickInit (void)
{
    HandleTIMER0    = (int)Timer0_Exception;
    rTCFG0          =  0;
    rTCFG1          =  1;
    rTCNTB0         = (PCLK / (4 * 200)) - 1;
    rTCON           = (1 << 1);
    rTCON           = (1 << 0) | (1 << 3);
    rSRCPND         = (1 << 10);
    rINTPND		    = (1 << 10);
    rINTMSK        &= ~(1 << 10);
}*/
extern void timer_init(void);
void  targetInit (void)
{
   //tickInit();
	uart_init();
	timer_init();

}


#if 0
void   tx_application_define(void *first_unused_memory)
{
	
}
#endif

/**************************************************************************/
/*     The implementations for some library functions For more details,   */
/*     please refer to the ADS compiler handbook and The library function */
/*     manual User could change it as needed                              */
/**************************************************************************/


/**************************************************************************/
/*     end of file                                                        */ 
/**************************************************************************/

