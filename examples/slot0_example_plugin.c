#include "example_app_types.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#include "plugin_expansion.h"

enum
{
	EXAMPLE_TASK_IDLE = 0,
	EXAMPLE_TASK_READY = 1
};

enum
{
	EXAMPLE_EEPROM_DIRTY_BIT = 0x01u,
	EXAMPLE_SLOT_ENABLED_BIT = 0x01u
};

EXP_PLUGIN_CONST(0) static const uint8_t slot0_banner[] = "slot0-example";

EXP_PLUGIN_CODE(0) static bool slot0_enabled(uint8_t expid)
{
	return (EXP_PLUGIN_DATABASE->enabled_slots & (uint8_t)(EXAMPLE_SLOT_ENABLED_BIT << expid)) != 0u;
}

static bool slot0_init(exp_stack_t *stk, uint8_t expid)
{
	(void)slot0_banner;

	if (stk == 0)
	{
		return false;
	}

	stk->cycle_count = 0u;
	stk->uart_activity_seen = false;

	return slot0_enabled(expid);
}

static task_status_t slot0_task(exp_stack_t *stk, uint8_t expid, task_ix_t self)
{
	(void)self;

	if ((stk == 0) || !slot0_enabled(expid))
	{
		return EXAMPLE_TASK_IDLE;
	}

	stk->cycle_count++;
	EXP_PLUGIN_DATABASE->plugin_runs++;
	EXP_PLUGIN_EEPROM_SAVE_DATA->dirty_mask |= EXAMPLE_EEPROM_DIRTY_BIT;

	return EXAMPLE_TASK_READY;
}

static void slot0_uart_irq(USART_TypeDef *usart)
{
	if (usart == 0)
	{
		return;
	}

	usart->isr_snapshot++;
}

const exp_plugin_host_api_t exp_plugin_host_api = {
	.database = &(database_t){
		.plugin_runs = 0u,
		.enabled_slots = EXAMPLE_SLOT_ENABLED_BIT,
	},
	.eeprom_save_data = &(eeprom_save_data_t){
		.dirty_mask = 0u,
	},
};

EXP_PLUGIN_DECLARE_SLOT_0(slot0_example_descriptor, slot0_init, slot0_task, slot0_uart_irq)
