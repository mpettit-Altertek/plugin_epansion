#ifndef PLUGIN_EXPANSION_H
#define PLUGIN_EXPANSION_H

#include <stdbool.h>
#include <stdint.h>

#ifndef PLUGIN_EXPANSION_HAS_APP_TYPES
#include "plugin_expansion_app_types.h"
#endif

#ifdef PLUGIN_EXPANSION_DECLARE_USART_TYPE
PLUGIN_EXPANSION_DECLARE_USART_TYPE
#endif

#ifndef PLUGIN_EXPANSION_USART_TYPE
typedef struct plugin_expansion_usart_stub plugin_expansion_usart_stub_t;
#define PLUGIN_EXPANSION_USART_TYPE plugin_expansion_usart_stub_t
#endif

#define EXP_PLUGIN_ABI_VERSION 0x00010000u
#define EXP_PLUGIN_SLOT_COUNT 4u

typedef bool (*exp_plugin_init_fn)(exp_stack_t *stk, uint8_t expid);
typedef task_status_t (*exp_plugin_task_fn)(exp_stack_t *stk, uint8_t expid, task_ix_t self);
typedef void (*exp_plugin_uart_irq_fn)(PLUGIN_EXPANSION_USART_TYPE *usart);

typedef struct
{
	database_t *database;
	eeprom_save_data_t *eeprom_save_data;
} exp_plugin_host_api_t;

#ifndef EXP_PLUGIN_HOST_API_SYMBOL
#define EXP_PLUGIN_HOST_API_SYMBOL exp_plugin_host_api
#endif

/*
 * Plugin descriptors reference a host-provided API object. This scaffold is
 * intended for relocatable post-compile plugin modules that are linked or
 * loaded alongside the host application, not for fully standalone plugin flash
 * images with no host symbol resolution step.
 */
#ifndef EXP_PLUGIN_NO_HOST_API_ONLY
extern const exp_plugin_host_api_t EXP_PLUGIN_HOST_API_SYMBOL;
#endif

#define EXP_PLUGIN_DATABASE (EXP_PLUGIN_HOST_API_SYMBOL.database)
#define EXP_PLUGIN_EEPROM_SAVE_DATA (EXP_PLUGIN_HOST_API_SYMBOL.eeprom_save_data)

typedef struct
{
	uint32_t abi_version;
	uint32_t slot;
	exp_plugin_init_fn init;
	exp_plugin_task_fn task;
	exp_plugin_uart_irq_fn UART_IRQHandler;
} exp_plugin_descriptor_t;

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

#define EXP_PLUGIN_DESCRIPTOR_SECTION_VALUE(slot) EXP_PLUGIN_DESCRIPTOR_SECTION_##slot
#define EXP_PLUGIN_CODE_SECTION_VALUE(slot) EXP_PLUGIN_CODE_SECTION_##slot
#define EXP_PLUGIN_CONST_SECTION_VALUE(slot) EXP_PLUGIN_CONST_SECTION_##slot

#define EXP_PLUGIN_DESCRIPTOR_SECTION(slot) EXP_PLUGIN_DESCRIPTOR_SECTION_VALUE(slot)
#define EXP_PLUGIN_CODE_SECTION(slot) EXP_PLUGIN_CODE_SECTION_VALUE(slot)
#define EXP_PLUGIN_CONST_SECTION(slot) EXP_PLUGIN_CONST_SECTION_VALUE(slot)

/*
 * These placement helpers only affect the symbols explicitly annotated with
 * them. Unannotated helper functions or const objects remain in the toolchain's
 * default sections unless the host project adds its own sectioning rules.
 */
#define EXP_PLUGIN_CODE(slot) __attribute__((used, section(EXP_PLUGIN_CODE_SECTION(slot))))
#define EXP_PLUGIN_CONST(slot) __attribute__((used, section(EXP_PLUGIN_CONST_SECTION(slot))))

#define EXP_PLUGIN_DEFINE_STATIC_IN_SECTION(name, slot_value, section_name, init_fn, task_fn, irq_fn) \
	__attribute__((used, section(section_name)))                                                \
	static const exp_plugin_descriptor_t name =                                                 \
	{                                                                                           \
		.abi_version = EXP_PLUGIN_ABI_VERSION,                                                  \
		.slot = (slot_value),                                                                   \
		.init = (init_fn),                                                                      \
		.task = (task_fn),                                                                      \
		.UART_IRQHandler = (irq_fn),                                                            \
	};

#define EXP_PLUGIN_DEFINE_IN_SECTION(name, slot_value, section_name, init_fn, task_fn, irq_fn) \
	__attribute__((used, section(section_name)))                                                \
	const exp_plugin_descriptor_t name =                                                        \
	{                                                                                           \
		.abi_version = EXP_PLUGIN_ABI_VERSION,                                                  \
		.slot = (slot_value),                                                                   \
		.init = (init_fn),                                                                      \
		.task = (task_fn),                                                                      \
		.UART_IRQHandler = (irq_fn),                                                            \
	};

#define EXP_PLUGIN_DECLARE_SLOT_0(name, init_fn, task_fn, irq_fn)                               \
	EXP_PLUGIN_DEFINE_STATIC_IN_SECTION(name, 0u, EXP_PLUGIN_DESCRIPTOR_SECTION_0, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DECLARE_SLOT_1(name, init_fn, task_fn, irq_fn)                               \
	EXP_PLUGIN_DEFINE_STATIC_IN_SECTION(name, 1u, EXP_PLUGIN_DESCRIPTOR_SECTION_1, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DECLARE_SLOT_2(name, init_fn, task_fn, irq_fn)                               \
	EXP_PLUGIN_DEFINE_STATIC_IN_SECTION(name, 2u, EXP_PLUGIN_DESCRIPTOR_SECTION_2, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DECLARE_SLOT_3(name, init_fn, task_fn, irq_fn)                               \
	EXP_PLUGIN_DEFINE_STATIC_IN_SECTION(name, 3u, EXP_PLUGIN_DESCRIPTOR_SECTION_3, init_fn, task_fn, irq_fn)

#define EXP_PLUGIN_DEFINE_SLOT_0(name, init_fn, task_fn, irq_fn)                                \
	EXP_PLUGIN_DEFINE_IN_SECTION(name, 0u, EXP_PLUGIN_DESCRIPTOR_SECTION_0, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DEFINE_SLOT_1(name, init_fn, task_fn, irq_fn)                                \
	EXP_PLUGIN_DEFINE_IN_SECTION(name, 1u, EXP_PLUGIN_DESCRIPTOR_SECTION_1, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DEFINE_SLOT_2(name, init_fn, task_fn, irq_fn)                                \
	EXP_PLUGIN_DEFINE_IN_SECTION(name, 2u, EXP_PLUGIN_DESCRIPTOR_SECTION_2, init_fn, task_fn, irq_fn)
#define EXP_PLUGIN_DEFINE_SLOT_3(name, init_fn, task_fn, irq_fn)                                \
	EXP_PLUGIN_DEFINE_IN_SECTION(name, 3u, EXP_PLUGIN_DESCRIPTOR_SECTION_3, init_fn, task_fn, irq_fn)

#endif
