#include "example_app_types.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#define PLUGIN_EXPANSION_USART_TYPE USART_TypeDef
#include "plugin_expansion.h"

#ifndef EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL
#define EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL example_plugin_descriptor
#endif

database_t database = {
	.plugin_runs = 0u,
	.enabled_slots = 0x0Fu,
};

eeprom_save_data_t eeprom_save_data = {
	.dirty_mask = 0u,
};

const exp_plugin_host_api_t exp_plugin_host_api = {
	.database = &database,
	.eeprom_save_data = &eeprom_save_data,
};

static exp_stack_t slot_stacks[EXP_PLUGIN_SLOT_COUNT];
static USART_TypeDef slot_usarts[EXP_PLUGIN_SLOT_COUNT];

extern const exp_plugin_descriptor_t EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL;

static bool example_descriptor_slot_valid(void)
{
	return example_plugin_descriptor.slot < EXP_PLUGIN_SLOT_COUNT;
}

bool example_application_init(uint8_t expid)
{
	if (!example_descriptor_slot_valid() || (EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.init == 0))
	{
		return false;
	}

	return EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.init(&slot_stacks[EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.slot], expid);
}

task_status_t example_application_task(uint8_t expid)
{
	if (!example_descriptor_slot_valid() || (EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.task == 0))
	{
		return 0;
	}

	return EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.task(&slot_stacks[EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.slot], expid, 0u);
}

void example_application_uart_irq(void)
{
	if (!example_descriptor_slot_valid() || (EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.UART_IRQHandler == 0))
	{
		return;
	}

	EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.UART_IRQHandler(&slot_usarts[EXAMPLE_PLUGIN_DESCRIPTOR_SYMBOL.slot]);
}
