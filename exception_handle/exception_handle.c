#include <stdio.h>
#include "irq.h"

extern unsigned int _interrupt_vector_table;

void _set_interrupt_vector(volatile unsigned int * vector_table_ptr);
void setup_vector_addr(void)
{
	volatile unsigned int * vector_table_ptr = &_interrupt_vector_table;

	// Update the interrupt vector address
	_set_interrupt_vector(vector_table_ptr);
}

void c_swi_handler(unsigned int nr, unsigned int arg)
{	
	static int i = 0;
	i ++;
    printf("\r\ntest:%d,c_swi_handler:%d,arg:%d\r\n",i, nr, arg);
    return;
}

void undef_handle(void)
{
	printf("undefined instruction exception.\n");
	while(1);
}

void fiq_handle(void)
{
	printf("fiq exception.\n");
	while(1);
}
#if 1
extern void irq_handle(void) __attribute__((weak));
void irq_handle(void) 
{
	printf("irq exception.\n");
	while(1);
}
#endif

void pabort_handle(void)
{
	unsigned int temp;
	printf("pabort exception.\n");
		__asm__ __volatile__(
	   "mov %0, r14\n"
		:"=r"(temp)
		:
		);
		printf("pabort lr=0x%x\r\n",temp);
	while(1);
}

void dabort_handle(void)
{

	printf("dabort exception.\n");
	while(1);
}

void reserved_handle(void)
{

	printf("reserved exception.\n");
	while(1);
}

void umask_int(unsigned int offset){
	*(volatile unsigned int *)INTMSK&=~(1<<offset);
}

void enable_irq(void){
	asm volatile (
		"mrs r4,cpsr\n\t"
		"bic r4,r4,#0x80\n\t"
		"msr cpsr,r4\n\t"
		:::"r4"
	);
}

void disable_irq(void){
	asm volatile (
		"mrs r4,cpsr\n\t"
		"orr r4,r4,#0x80\n\t"
		"msr cpsr,r4\n\t"
		:::"r4"
	);
}
//static irq_func irq_list[TOTAL_OFFSET];
//static (void *irq_list)(void);
//void(*irq_list[])(void);
static void (*irq_list[TOTAL_OFFSET])(void) = {NULL,};
void register_irq_handler(int offset, void(*f)())
{
	
	if(f != NULL && offset >=0 && offset < 32)
	irq_list[offset] = f;
	
	*(volatile unsigned int *)INTMSK&=~(1<<offset);
	*(volatile unsigned int *)SRCPND|= (1<<offset);
	*(volatile unsigned int *)INTPND|= (1<<offset);
}

void common_irq_handler(void)
{
	
	unsigned int tmp;
	static int count = 0;
//	disable_irq();

	tmp = (*(volatile unsigned int *)INTOFFSET);
//	printf("interrupt occur:%d, count:%d\n",tmp, count);
	if(irq_list[tmp] != NULL)
	irq_list[tmp]();

	*(volatile unsigned int *)SRCPND |= (1<<tmp);
	*(volatile unsigned int *)INTPND |= (1<<tmp);

	count ++;
//	enable_irq();
}