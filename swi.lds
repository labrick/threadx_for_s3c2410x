OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
/*OUTPUT_FORMAT("elf32-arm", "elf32-arm", "elf32-arm")*/
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x30000000;
	.text      :
	{
	  start.o	(.text)
	  *(.text)
	}
	.rodata : { *(.rodata) }

	. = ALIGN(8192);

	.data : {*(.data)}
	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	_end = .;

}
