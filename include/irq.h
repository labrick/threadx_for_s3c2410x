
#define _ISR_STARTADDRESS  0x00000000
#define pISR_EINT0  (*(unsigned *)(_ISR_STARTADDRESS+0x20))
#define pISR_EINT1  (*(unsigned *)(_ISR_STARTADDRESS+0x24))
#define pISR_EINT2  (*(unsigned *)(_ISR_STARTADDRESS+0x28))
#define pISR_EINT3  (*(unsigned *)(_ISR_STARTADDRESS+0x2c))
#define pISR_EINT4_7 (*(unsigned *)(_ISR_STARTADDRESS+0x30))
#define pISR_EINT8_23 (*(unsigned *)(_ISR_STARTADDRESS+0x34))
#define pISR_CAM  (*(unsigned *)(_ISR_STARTADDRESS+0x38))  // Added for 2440.
#define pISR_BAT_FLT (*(unsigned *)(_ISR_STARTADDRESS+0x3c))
#define pISR_TICK  (*(unsigned *)(_ISR_STARTADDRESS+0x40))
#define pISR_WDT_AC97  (*(unsigned *)(_ISR_STARTADDRESS+0x44))//Change to pISR_WDT_AC97 for 2440A
#define pISR_TIMER0   (*(unsigned *)(_ISR_STARTADDRESS+0x48))
#define pISR_TIMER1   (*(unsigned *)(_ISR_STARTADDRESS+0x4c))
#define pISR_TIMER2  (*(unsigned *)(_ISR_STARTADDRESS+0x50))
#define pISR_TIMER3  (*(unsigned *)(_ISR_STARTADDRESS+0x54))
#define pISR_TIMER4  (*(unsigned *)(_ISR_STARTADDRESS+0x58))
#define pISR_UART2  (*(unsigned *)(_ISR_STARTADDRESS+0x5c))
#define pISR_LCD  (*(unsigned *)(_ISR_STARTADDRESS+0x60))
#define pISR_DMA0  (*(unsigned *)(_ISR_STARTADDRESS+0x64))
#define pISR_DMA1  (*(unsigned *)(_ISR_STARTADDRESS+0x68))
#define pISR_DMA2  (*(unsigned *)(_ISR_STARTADDRESS+0x6c))
#define pISR_DMA3  (*(unsigned *)(_ISR_STARTADDRESS+0x70))
#define pISR_SDI  (*(unsigned *)(_ISR_STARTADDRESS+0x74))
#define pISR_SPI0  (*(unsigned *)(_ISR_STARTADDRESS+0x78))
#define pISR_UART1  (*(unsigned *)(_ISR_STARTADDRESS+0x7c))
#define pISR_NFCON  (*(unsigned *)(_ISR_STARTADDRESS+0x80))  // Added for 2440.
#define pISR_USBD  (*(unsigned *)(_ISR_STARTADDRESS+0x84))
#define pISR_USBH  (*(unsigned *)(_ISR_STARTADDRESS+0x88))
#define pISR_IIC  (*(unsigned *)(_ISR_STARTADDRESS+0x8c))
#define pISR_UART0  (*(unsigned *)(_ISR_STARTADDRESS+0x90))
#define pISR_SPI1  (*(unsigned *)(_ISR_STARTADDRESS+0x94))
#define pISR_RTC  (*(unsigned *)(_ISR_STARTADDRESS+0x98))
#define pISR_ADC  (*(unsigned *)(_ISR_STARTADDRESS+0x9c))

enum INT_OFFSET{
	EINT0_OFFSET =	0,
	EINT1_OFFSET,
	EINT2_OFFSET,
	EINT3_OFFSET,
	EINT4_7_OFFSET,
	EINT8_23_OFFSET,
	CAM_OFFSET,
	BAT_FLT_OFFSET,
	TICK_OFFSET,
	WDT_AC97_OFFSET,
	TIME0_OFFSET,
	TIMER1_OFFSET,
	TIMER2_OFFSET,
	TIMER3_OFFSET,
	TIMER4_OFFSET,
	UART2_OFFSET,
	LCD_OFFSET,
	DMA0_OFFSET,
	DMA1_OFFSET,
	DMA2_OFFSET,
	DMA3_OFFSET,
	SDI_OFFSET,
	SPIO_OFFSET,
	UART1_OFFSET,
	NFCON_OFFSET,
	USBD_OFFSET,
	USBH_OFFSET,
	IIC_OFFSET,
	UART0_OFFSET,
	SPI1_OFFSET,
	RTC_OFFSET,
	ADC_OFFSET,
	TOTAL_OFFSET
};

#define INT_BASE	(0x4a000000)
#define INTMSK		(INT_BASE+0x8)
#define INTOFFSET	(INT_BASE+0x14)
#define INTPND		(INT_BASE+0x10)
#define SRCPND		(INT_BASE+0x0)
void register_irq_handler(int offset, void(*f)());
//typedef int (void *irq_func)(void);