MEMORY
{
	FALLBACK_FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x00008000
	FALLBACK_RAM   (rw) : ORIGIN = 0x20000000, LENGTH = 0x00002000
	EXP_BOARD_0 (rx) : ORIGIN = 0x08080000, LENGTH = 0x00002000
	EXP_BOARD_1 (rx) : ORIGIN = 0x08082000, LENGTH = 0x00002000
	EXP_BOARD_2 (rx) : ORIGIN = 0x08084000, LENGTH = 0x00002000
	EXP_BOARD_3 (rx) : ORIGIN = 0x08086000, LENGTH = 0x00002000
}

SECTIONS
{
	.text :
	{
		*(.text*)
	} > FALLBACK_FLASH

	.rodata :
	{
		*(.rodata*)
		*(.eh_frame*)
		*(.note.gnu.property*)
	} > FALLBACK_FLASH

	.data :
	{
		*(.data*)
		*(.data.*)
		*(.bss*)
		*(.bss.*)
	} > FALLBACK_RAM
}

INCLUDE "../../linker/plugin_expansion_sections.ld"
