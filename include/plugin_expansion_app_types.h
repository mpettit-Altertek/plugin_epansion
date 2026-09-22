#ifndef PLUGIN_EXPANSION_APP_TYPES_H
#define PLUGIN_EXPANSION_APP_TYPES_H

#include <stdint.h>

#ifndef PLUGIN_EXPANSION_HAS_APP_TYPES
typedef struct exp_stack exp_stack_t;
typedef int32_t task_status_t;
typedef uint32_t task_ix_t;
typedef struct database database_t;
typedef struct eeprom_save_data eeprom_save_data_t;
typedef struct USART_TypeDef USART_TypeDef;
#endif

#endif
