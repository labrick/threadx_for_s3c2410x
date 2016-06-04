OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
/*OUTPUT_FORMAT("elf32-arm", "elf32-arm", "elf32-arm")*/
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x30000000;
	.text      :
	{
	  out/start.o	(.text)
	  out/*(.text)
	}
	.rodata : { out/*(.rodata) }

	. = ALIGN(8192);

	.data : {out/*(.data)}

	. = ALIGN(4);
	__bss_start = .;
	.bss : { out/*(.bss) }
	_end = .;

}
