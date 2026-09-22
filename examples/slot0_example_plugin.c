#include "example_app_types.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#define PLUGIN_EXPANSION_USART_TYPE USART_TypeDef
#define EXP_PLUGIN_HOST_API_SYMBOL example_host_api_stub
#include "plugin_expansion.h"

#ifndef EXAMPLE_PLUGIN_SLOT
#define EXAMPLE_PLUGIN_SLOT 0
#endif

#if (EXAMPLE_PLUGIN_SLOT < 0) || (EXAMPLE_PLUGIN_SLOT > 3)
#error "EXAMPLE_PLUGIN_SLOT must be 0, 1, 2, or 3"
#endif

#define EXAMPLE_SLOT_BIT(slot) ((uint8_t)(1u << (slot)))
#define EXAMPLE_SLOT_NAME_VALUE(slot) #slot
#define EXAMPLE_SLOT_NAME(slot) EXAMPLE_SLOT_NAME_VALUE(slot)

#if EXAMPLE_PLUGIN_SLOT == 0
#define EXAMPLE_PLUGIN_DECLARE(name, init_fn, task_fn, irq_fn) EXP_PLUGIN_DECLARE_SLOT_0(name, init_fn, task_fn, irq_fn)
#define EXAMPLE_PLUGIN_CODE EXP_PLUGIN_CODE(0)
#define EXAMPLE_PLUGIN_CONST EXP_PLUGIN_CONST(0)
#elif EXAMPLE_PLUGIN_SLOT == 1
#define EXAMPLE_PLUGIN_DECLARE(name, init_fn, task_fn, irq_fn) EXP_PLUGIN_DECLARE_SLOT_1(name, init_fn, task_fn, irq_fn)
#define EXAMPLE_PLUGIN_CODE EXP_PLUGIN_CODE(1)
#define EXAMPLE_PLUGIN_CONST EXP_PLUGIN_CONST(1)
#elif EXAMPLE_PLUGIN_SLOT == 2
#define EXAMPLE_PLUGIN_DECLARE(name, init_fn, task_fn, irq_fn) EXP_PLUGIN_DECLARE_SLOT_2(name, init_fn, task_fn, irq_fn)
#define EXAMPLE_PLUGIN_CODE EXP_PLUGIN_CODE(2)
#define EXAMPLE_PLUGIN_CONST EXP_PLUGIN_CONST(2)
#else
#define EXAMPLE_PLUGIN_DECLARE(name, init_fn, task_fn, irq_fn) EXP_PLUGIN_DECLARE_SLOT_3(name, init_fn, task_fn, irq_fn)
#define EXAMPLE_PLUGIN_CODE EXP_PLUGIN_CODE(3)
#define EXAMPLE_PLUGIN_CONST EXP_PLUGIN_CONST(3)
#endif

enum
{
	EXAMPLE_TASK_IDLE = 0,
	EXAMPLE_TASK_READY = 1
};

enum
{
	EXAMPLE_EEPROM_DIRTY_BIT = 0x01u,
	EXAMPLE_SLOT_ENABLED_BIT = EXAMPLE_SLOT_BIT(EXAMPLE_PLUGIN_SLOT)
};

EXAMPLE_PLUGIN_CONST static const uint8_t example_banner[] = "slot" EXAMPLE_SLOT_NAME(EXAMPLE_PLUGIN_SLOT) "-example";

static database_t example_database = {
	.plugin_runs = 0u,
	.enabled_slots = EXAMPLE_SLOT_ENABLED_BIT,
};

static eeprom_save_data_t example_eeprom_save_data = {
	.dirty_mask = 0u,
};

/* The host API container is const; the host-owned objects it points to are mutable. */
const exp_plugin_host_api_t example_host_api_stub = {
	.database = &example_database,
	.eeprom_save_data = &example_eeprom_save_data,
};

EXAMPLE_PLUGIN_CODE static bool example_slot_enabled(uint8_t expid)
{
	return (EXP_PLUGIN_DATABASE->enabled_slots & EXAMPLE_SLOT_BIT(expid)) != 0u;
}

EXAMPLE_PLUGIN_CODE static bool example_slot_init(exp_stack_t *stk, uint8_t expid)
{
	(void)example_banner;

	if (stk == 0)
	{
		return false;
	}

	stk->cycle_count = 0u;
	stk->uart_activity_seen = false;

	return example_slot_enabled(expid);
}

EXAMPLE_PLUGIN_CODE static task_status_t example_slot_task(exp_stack_t *stk, uint8_t expid, task_ix_t self)
{
	(void)self;

	if ((stk == 0) || !example_slot_enabled(expid))
	{
		return EXAMPLE_TASK_IDLE;
	}

	stk->cycle_count++;
	EXP_PLUGIN_DATABASE->plugin_runs++;
	EXP_PLUGIN_EEPROM_SAVE_DATA->dirty_mask |= EXAMPLE_EEPROM_DIRTY_BIT;

	return EXAMPLE_TASK_READY;
}

EXAMPLE_PLUGIN_CODE static void example_slot_uart_irq(USART_TypeDef *usart)
{
	if (usart == 0)
	{
		return;
	}

	usart->isr_snapshot++;
}

EXAMPLE_PLUGIN_DECLARE(example_plugin_descriptor, example_slot_init, example_slot_task, example_slot_uart_irq)
