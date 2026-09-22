SECTIONS
{
	.EXP_BOARD_0_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_0_start__ = .;
		KEEP(*(.EXP_BOARD_0_SECT))
		*(.EXP_BOARD_0_TEXT)
		*(.EXP_BOARD_0_TEXT.*)
		*(.EXP_BOARD_0_RODATA)
		*(.EXP_BOARD_0_RODATA.*)
		*(.EXP_BOARD_0_DATA)
		*(.EXP_BOARD_0_DATA.*)
		. = ALIGN(8);
		__exp_board_0_end__ = .;
	} > EXP_BOARD_0

	.EXP_BOARD_1_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_1_start__ = .;
		KEEP(*(.EXP_BOARD_1_SECT))
		*(.EXP_BOARD_1_TEXT)
		*(.EXP_BOARD_1_TEXT.*)
		*(.EXP_BOARD_1_RODATA)
		*(.EXP_BOARD_1_RODATA.*)
		*(.EXP_BOARD_1_DATA)
		*(.EXP_BOARD_1_DATA.*)
		. = ALIGN(8);
		__exp_board_1_end__ = .;
	} > EXP_BOARD_1

	.EXP_BOARD_2_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_2_start__ = .;
		KEEP(*(.EXP_BOARD_2_SECT))
		*(.EXP_BOARD_2_TEXT)
		*(.EXP_BOARD_2_TEXT.*)
		*(.EXP_BOARD_2_RODATA)
		*(.EXP_BOARD_2_RODATA.*)
		*(.EXP_BOARD_2_DATA)
		*(.EXP_BOARD_2_DATA.*)
		. = ALIGN(8);
		__exp_board_2_end__ = .;
	} > EXP_BOARD_2

	.EXP_BOARD_3_BLOCK :
	{
		. = ALIGN(8);
		__exp_board_3_start__ = .;
		KEEP(*(.EXP_BOARD_3_SECT))
		*(.EXP_BOARD_3_TEXT)
		*(.EXP_BOARD_3_TEXT.*)
		*(.EXP_BOARD_3_RODATA)
		*(.EXP_BOARD_3_RODATA.*)
		*(.EXP_BOARD_3_DATA)
		*(.EXP_BOARD_3_DATA.*)
		. = ALIGN(8);
		__exp_board_3_end__ = .;
	} > EXP_BOARD_3
}
