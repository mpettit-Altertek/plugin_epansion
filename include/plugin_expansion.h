#ifndef PLUGIN_EXPANSION_H
#define PLUGIN_EXPANSION_H

#include <stdbool.h>
#include <stdint.h>

#include "plugin_expansion_app_types.h"

#define EXP_PLUGIN_ABI_VERSION 0x00010000u
#define EXP_PLUGIN_SLOT_COUNT 4u

typedef bool (*exp_plugin_init_fn)(exp_stack_t *stk, uint8_t expid);
typedef task_status_t (*exp_plugin_task_fn)(exp_stack_t *stk, uint8_t expid, task_ix_t self);
typedef void (*exp_plugin_uart_irq_fn)(USART_TypeDef *huart);

typedef struct
{
	uint32_t abi_version;
	uint32_t slot;
	exp_plugin_init_fn init;
	exp_plugin_task_fn task;
	exp_plugin_uart_irq_fn UART_IRQHandler;
	database_t *database;
	eeprom_save_data_t *eeprom_save_data;
} exp_plugin_descriptor_t;

extern database_t database;
extern eeprom_save_data_t eeprom_save_data;

#define EXP_PLUGIN_DESCRIPTOR_SECTION_0 ".EXP_BOARD_0_SECT"
#define EXP_PLUGIN_DESCRIPTOR_SECTION_1 ".EXP_BOARD_1_SECT"
#define EXP_PLUGIN_DESCRIPTOR_SECTION_2 ".EXP_BOARD_2_SECT"
#define EXP_PLUGIN_DESCRIPTOR_SECTION_3 ".EXP_BOARD_3_SECT"

#define EXP_PLUGIN_CODE_SECTION_0 ".EXP_BOARD_0_TEXT"
#define EXP_PLUGIN_CODE_SECTION_1 ".EXP_BOARD_1_TEXT"
#define EXP_PLUGIN_CODE_SECTION_2 ".EXP_BOARD_2_TEXT"
#define EXP_PLUGIN_CODE_SECTION_3 ".EXP_BOARD_3_TEXT"

#define EXP_PLUGIN_CONST_SECTION_0 ".EXP_BOARD_0_RODATA"
#define EXP_PLUGIN_CONST_SECTION_1 ".EXP_BOARD_1_RODATA"
#define EXP_PLUGIN_CONST_SECTION_2 ".EXP_BOARD_2_RODATA"
#define EXP_PLUGIN_CONST_SECTION_3 ".EXP_BOARD_3_RODATA"

#define EXP_PLUGIN_DATA_SECTION_0 ".EXP_BOARD_0_DATA"
#define EXP_PLUGIN_DATA_SECTION_1 ".EXP_BOARD_1_DATA"
#define EXP_PLUGIN_DATA_SECTION_2 ".EXP_BOARD_2_DATA"
#define EXP_PLUGIN_DATA_SECTION_3 ".EXP_BOARD_3_DATA"

#define EXP_PLUGIN_DESCRIPTOR_SECTION(slot) EXP_PLUGIN_DESCRIPTOR_SECTION_##slot
#define EXP_PLUGIN_CODE_SECTION(slot) EXP_PLUGIN_CODE_SECTION_##slot
#define EXP_PLUGIN_CONST_SECTION(slot) EXP_PLUGIN_CONST_SECTION_##slot
#define EXP_PLUGIN_DATA_SECTION(slot) EXP_PLUGIN_DATA_SECTION_##slot

#define EXP_PLUGIN_CODE(slot) __attribute__((section(EXP_PLUGIN_CODE_SECTION(slot))))
#define EXP_PLUGIN_CONST(slot) __attribute__((section(EXP_PLUGIN_CONST_SECTION(slot))))
#define EXP_PLUGIN_DATA(slot) __attribute__((section(EXP_PLUGIN_DATA_SECTION(slot))))

#define EXP_PLUGIN_DECLARE(slot_id, init_fn, task_fn, irq_fn)                                   \
	__attribute__((used, section(EXP_PLUGIN_DESCRIPTOR_SECTION(slot_id))))                      \
	const exp_plugin_descriptor_t exp_plugin_descriptor_##slot_id =                             \
	{                                                                                           \
		.abi_version = EXP_PLUGIN_ABI_VERSION,                                                  \
		.slot = (slot_id),                                                                      \
		.init = (init_fn),                                                                      \
		.task = (task_fn),                                                                      \
		.UART_IRQHandler = (irq_fn),                                                            \
		.database = &database,                                                                  \
		.eeprom_save_data = &eeprom_save_data,                                                  \
	}

#endif
