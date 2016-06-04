

#define Mode_USR      0x10
#define Mode_FIQ      0x11
#define Mode_IRQ      0x12
#define Mode_SVC      0x13
#define Mode_SEC	  0x16
#define Mode_ABT      0x17
#define Mode_UND      0x1B
#define Mode_SYS      0x1F
#define Mode_MASK     0x1F
#define NOINT         0xC0
#define I_Bit         0x80
#define F_Bit         0x40


#define DRAM_BASE_PA_START 0x30000000
#define IMAGE_OFFSET 0x1000000
#define TOP_OF_STACKS_PHYSICAL      (DRAM_BASE_PA_START+IMAGE_OFFSET)    
#define TOP_OF_STACKS_VIRTUAL       (DRAM_BASE_CA_START+IMAGE_OFFSET)

// Stack Size of Each Mode
#define FIQStackSize     8192
#define IRQStackSize     8192
#define AbortStackSize   8192
#define UndefStackSize   8192
#define SVCStackSize     8192
#define SECStackSize	 8192
#define UserStackSize   10485760
#define SysStackSize     10485760
//Stack Location of Each Mode (in Physical Address)
#define FIQStack_PA    (TOP_OF_STACKS_PHYSICAL    - 0x0)
#define IRQStack_PA    (FIQStack_PA        - FIQStackSize)
#define AbortStack_PA  (IRQStack_PA        - IRQStackSize)
#define UndefStack_PA  (AbortStack_PA        - AbortStackSize)
#define SVCStack_PA    (UndefStack_PA        - UndefStackSize)
//#define UserStack_PA  (SVCStack_PA        - SVCStackSize)
#define SECStack_PA		(SVCStack_PA        - SVCStackSize)
#define SysStack_PA    (SECStack_PA        - SECStackSize)

	.extern pabort_handle
	.extern dabort_handle
	.extern reserved_handle
	.extern fiq_handle
	.extern undef_handle

	.global irq_handle
	.global _start
	.global swi_hand
	.global reset_handle
	.global _interrupt_vector_table

	.global __thread_irq_handle
	.global __thread_swi_handle
	.global swi_handle
	
_start:

_interrupt_vector_table:

	ldr		pc, 	=reset_handle
	ldr		pc, 	=undef_handle
//	ldr		pc, 	=swi_hand
	ldr		pc, 	=__thread_swi_handle
	ldr		pc, 	=pabort_handle
	ldr		pc, 	=dabort_handle
	ldr		pc, 	=reserved_handle
//	ldr		pc, 	=irq_handle
	ldr		pc, 	=__thread_irq_handle
	ldr		pc, 	=fiq_handle
	

#  define pWTCON	0x53000000
#  define INTMSK	0x4A000008	/* Interupt-Controller base addresses */
#  define INTSUBMSK	0x4A00001C
#  define CLKDIVN	0x4C000014	/* clock divisor register */

#define CLK_CTL_BASE	0x4C000000	/* tekkaman */
#define MDIV_405	0x7f << 12	/* tekkaman */
#define PSDIV_405	0x21		/* tekkaman */
#define MDIV_200	0xa1 << 12	/* tekkaman */
#define PSDIV_200	0x31		/* tekkaman */
reset_handle:	

	/*
	 * set the cpu to SVC32 mode
	 */
	mrs		r0, 	cpsr
	bic		r0, 	r0, 	#0x1f
	orr		r0, 	r0, 	#0xd3
	msr		cpsr, 	r0

	bl 	_init_clock
	
	bl 	_disable_interrupt
	
	bl 	_init_stacks
	
	bl 	cpu_init_crit
	
	bl 	_clear_bss
	
//	bl _enable_interrupt
	
	ldr 	pc, 	=main



_init_clock:

	/* turn off the watchdog */
	ldr		r0, 	=pWTCON
	mov		r1, 	#0x0
	str		r1, 	[r0]

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov		r1, 	#0xffffffff
	ldr		r0, 	=INTMSK
	str		r1, 	[r0]



	ldr		r1, 	=0x7fff	
	ldr		r0, 	=INTSUBMSK
	str		r1, 	[r0]


	/* FCLK:HCLK:PCLK = 1:4:8 */
	ldr		r0, 	=CLKDIVN
	mov		r1, 	#5
	str		r1, 	[r0]
	
	mrc		p15, 0, r1, c1, c0, 0	
	orr		r1, 	r1, 	#0xc0000000		
	mcr		p15, 0, r1, c1, c0, 0	
	
	
	mov		r1, 	#CLK_CTL_BASE	
	mov		r2, 	#MDIV_405	
	add		r2, 	r2, 	#PSDIV_405	
	str		r2, 	[r1, #0x04]		/* MPLLCON tekkaman */
	
	mov 	pc, 	lr
	
    .LTORG 	
	
cpu_init_crit:
	/*
	 * flush v4 I/D caches
	 */
	mov		r0, 	#0
	mcr		p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr		p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, 	r0, 	#0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic		r0, 	r0, 	#0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr		r0, 	r0, 	#0x00000002	@ set bit 2 (A) Align
	orr		r0, 	r0, 	#0x00001000	@ set bit 12 (I) I-Cache
	mcr		p15, 0, r0, c1, c0, 0

	mov 	pc, lr
	
    .LTORG 	

	
//-------------------------------------------------------------------
//  Initialize Stack
//-------------------------------------------------------------------
_init_stacks:
	mrs 	r0, 	cpsr
	bic 	r0, 	r0, 	#Mode_MASK
	orr 	r1, 	r0, 	#Mode_IRQ
	msr 	cpsr_c, 	r1                  // IRQ Mode
	ldr 	sp, 	=(IRQStack_PA) 

	orr 	r1, 	r0, 	#Mode_FIQ
	msr 	cpsr_c, r1                      // FIQ Mode
	ldr 	sp, 	=(FIQStack_PA) 

	orr 	r1, 	r0, 	#Mode_ABT
	msr 	cpsr_c, r1                      // ABORT Mode
	ldr 	sp, 	=(AbortStack_PA) 

	orr 	r1, 	r0, 	#Mode_UND
	msr 	cpsr_c, r1                      // UND Mode
	ldr 	sp, 	=(UndefStack_PA) 

	orr 	r1, 	r0, 	#Mode_SYS
	msr 	cpsr_c, r1                      // UND Mode
	ldr 	sp, 	=(SysStack_PA)  

	orr 	r1, 	r0, 	#Mode_SVC
	msr 	cpsr_c, r1                      // Supervisor Mode
	ldr 	sp, 	=(SVCStack_PA)  

    mov 	pc, 	lr

   .LTORG   

.global __bss_start
.global _end
_clear_bss:
	ldr		r0, 	=__bss_start		/* find start of bss segment        */
	ldr		r1, 	=_end		/* stop here                        */
	mov 	r2, 	#0x00000000		/* clear                            */

clbss_l:
	str		r2, 	[r0]		/* clear loop...                    */
	add		r0, 	r0, 	#4
	cmp		r0, 	r1
	ble		clbss_l   
    mov 	pc, 	lr
   .LTORG    


irq_handle:
	sub		lr,		lr,		#4
	stmfd	sp!,	{r0-r12, lr}
	bl		common_irq_handler
	ldmfd	sp!,	{r0-r12, pc}^
   
   .global switest
   
switest:
	stmfd	sp!, 	{r0-r3, fp, lr}
	swi 	0x8
	ldmfd 	sp!, 	{r0-r3, fp, pc}^		
	
swi_handle:
	stmfd	sp!, 	{fp, lr}
	swi 	0x0
	ldmfd 	sp!, 	{fp, pc}	
	

swi_hand:
	stmfd	sp!, 	{r0-r13, lr}
	mov 	r1,		r0
	mrs 	r0,		spsr
	stmfd	sp!,	{r0}
	
	tst 	r0, 	#0x20
	ldrne 	r0, 	[lr, #-2]
	bicne 	r0, 	r0,		#0xff00
	ldreq 	r0, 	[lr, #-4]
	biceq 	r0, 	r0,		#0xff000000
	
	bl 		c_swi_handler
	
	ldmfd	sp!,	{r0}
	msr 	spsr_cf, r0
	ldmfd 	sp!, 	{r0-r13, pc}^

	.global _disable_interrupt
_disable_interrupt:
	mrs 	r0,cpsr
	orr 	r1,r0,#NOINT
	msr 	cpsr_c,r1
	and		r0, r0,#NOINT		// Return old interrupt bits
	mov		pc, lr
	
	.LTORG   	

	.global _enable_interrupt
_enable_interrupt:
	mrs 	r1,cpsr
	bic 	r1,r1,#NOINT
	and     r0, r0,#NOINT 	// Extract only the interrupt bits
	orr		r1,r1,r0			// Mask 
	msr 	cpsr_c,r1
	mov   	pc, lr
	
	.LTORG  

	
	.global _set_interrupt_vector
_set_interrupt_vector:
	
	mcr	p15, 0, r0, c12, c0, 0
	mrc	p15, 0, r0, c12, c0, 0
	
	mov pc, lr
