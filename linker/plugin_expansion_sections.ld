SECTIONS
{
	/* Writable plugin-owned .data/.bss is intentionally not collected here. */
	/* This scaffold only emits descriptors, code, and flash-resident constants. */
	.EXP_BOARD_0_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_0_start__ = .;
		KEEP(*(.EXP_BOARD_0_SECT))
		KEEP(*(.EXP_BOARD_0_TEXT*))
		KEEP(*(.EXP_BOARD_0_RODATA*))
		. = ALIGN(8);
		__exp_board_0_end__ = .;
	} > EXP_BOARD_0

	.EXP_BOARD_1_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_1_start__ = .;
		KEEP(*(.EXP_BOARD_1_SECT))
		KEEP(*(.EXP_BOARD_1_TEXT*))
		KEEP(*(.EXP_BOARD_1_RODATA*))
		. = ALIGN(8);
		__exp_board_1_end__ = .;
	} > EXP_BOARD_1

	.EXP_BOARD_2_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_2_start__ = .;
		KEEP(*(.EXP_BOARD_2_SECT))
		KEEP(*(.EXP_BOARD_2_TEXT*))
		KEEP(*(.EXP_BOARD_2_RODATA*))
		. = ALIGN(8);
		__exp_board_2_end__ = .;
	} > EXP_BOARD_2

	.EXP_BOARD_3_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_3_start__ = .;
		KEEP(*(.EXP_BOARD_3_SECT))
		KEEP(*(.EXP_BOARD_3_TEXT*))
		KEEP(*(.EXP_BOARD_3_RODATA*))
		. = ALIGN(8);
		__exp_board_3_end__ = .;
	} > EXP_BOARD_3
}
