#include "example_app_types.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#define PLUGIN_EXPANSION_USART_TYPE USART_TypeDef
#include "plugin_expansion.h"

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

static exp_stack_t slot0_stack;
static USART_TypeDef slot0_usart;

extern const exp_plugin_descriptor_t example_plugin_descriptor;

bool example_application_init(uint8_t expid)
{
	return example_plugin_descriptor.init(&slot0_stack, expid);
}

task_status_t example_application_task(uint8_t expid)
{
	return example_plugin_descriptor.task(&slot0_stack, expid, 0u);
}

void example_application_uart_irq(void)
{
	example_plugin_descriptor.UART_IRQHandler(&slot0_usart);
}
