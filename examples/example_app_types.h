#ifndef EXAMPLES_APP_TYPES_H
#define EXAMPLES_APP_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
	uint32_t cycle_count;
	bool uart_activity_seen;
} app_exp_stack_t;

typedef int32_t app_task_status_t;
typedef uint32_t app_task_ix_t;

typedef struct
{
	uint32_t plugin_runs;
	uint8_t enabled_slots;
	uint8_t current_expid_by_slot[4];
	bool uart_activity_seen[4];
} app_database_t;

typedef struct
{
	uint32_t dirty_mask;
} app_eeprom_save_data_t;

typedef struct
{
	volatile uint32_t isr_snapshot;
} USART_TypeDef;

#endif
