#include <stdio.h>
#include "irq.h"

#define TIMER_BASE  (0x51000000)
#define TCFG0   (*(volatile unsigned int *)(TIMER_BASE+0x0))
#define TCFG1   (*(volatile unsigned int *)(TIMER_BASE+0x4))
#define TCON    (*(volatile unsigned int *)(TIMER_BASE+0x8))
#define TCNTB4  (*(volatile unsigned int *)(TIMER_BASE+0x3c))

void timer_handler(void)
{
	static int count = 0;
	if(count%10 == 0)
//	printf("Hello, I am timer handler:%d\n", count);
//	count ++;
	_tx_timer_interrupt();	
}

void timer_init(void)
{

	register_irq_handler(TIMER4_OFFSET, timer_handler);

	TCFG0 &= 0xFF00FF;
	TCFG0 |= 0xf900;        // prescaler等于249
	TCFG1 &= ~0xF0000;    
	TCFG1 |= 0x30000;     //divider等于8，则设置定时器4的时钟频率为25kHz
	TCNTB4 = 1000;              //让定时器4每隔0.5秒中断一次
	TCON &= ~0xF00000;
	TCON |= 0x700000;
	TCON &= ~0x200000 ;              //定时器4开始工作

//	enable_irq();
}