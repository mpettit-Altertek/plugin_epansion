# plugin_epansion

Minimal scaffold for STM32L496AGI3 post-compile expansion plugins.

## What this adds

- Four independent plugin slots mapped to `EXP_BOARD_0` through `EXP_BOARD_3`
- Required plugin entry points:
  - `bool init(exp_stack_t* stk, uint8_t expid)`
  - `task_status_t task(exp_stack_t* stk, uint8_t expid, task_ix_t self)`
  - `void UART_IRQHandler(USART_TypeDef* huart)`
- Access from each plugin to the main application symbols:
  - `database_t database`
  - `eeprom_save_data_t eeprom_save_data`

## Repository layout

- `/home/runner/work/plugin_epansion/plugin_epansion/include/plugin_expansion_app_types.h`
  - Template type bridge between the main application and plugin code
- `/home/runner/work/plugin_epansion/plugin_epansion/include/plugin_expansion.h`
  - Plugin ABI, section macros, and descriptor declaration helpers
- `/home/runner/work/plugin_epansion/plugin_epansion/linker/plugin_expansion_sections.ld`
  - Linker `SECTIONS` fragment for the four `EXP_BOARD_*` memory regions

## Main application integration

Include the linker fragment from the STM32 application linker script after the
`MEMORY` block that defines `EXP_BOARD_0` through `EXP_BOARD_3`.

```ld
INCLUDE "linker/plugin_expansion_sections.ld"
```

The expansion blocks must be loadable sections, not `NOLOAD`, so the plugin
descriptors and plugin code are emitted into flash.

In the STM32 application, make the shared symbols visible to plugin code:

```c
database_t database;
eeprom_save_data_t eeprom_save_data;
```

If the application already defines `exp_stack_t`, `task_status_t`, `task_ix_t`,
`database_t`, `eeprom_save_data_t`, and `USART_TypeDef`, include those headers
before `plugin_expansion.h` and define `PLUGIN_EXPANSION_HAS_APP_TYPES` to
disable the fallback typedefs in `plugin_expansion_app_types.h`.

## Declaring a plugin

```c
#define PLUGIN_EXPANSION_HAS_APP_TYPES
#include "app_types.h"
#include "plugin_expansion.h"

static bool exp0_init(exp_stack_t *stk, uint8_t expid);
static task_status_t exp0_task(exp_stack_t *stk, uint8_t expid, task_ix_t self);
static void exp0_uart_irq(USART_TypeDef *huart);

EXP_PLUGIN_DECLARE(0, exp0_init, exp0_task, exp0_uart_irq);
```

Optional placement helpers are available for slot-specific code and data:

```c
EXP_PLUGIN_CODE(0) static void exp0_helper(void);
EXP_PLUGIN_CONST(0) static const uint8_t exp0_table[] = { 0x01, 0x02 };
EXP_PLUGIN_DATA(0) static uint8_t exp0_state;
```

This keeps each plugin image isolated in its corresponding `EXP_BOARD_*` block.
