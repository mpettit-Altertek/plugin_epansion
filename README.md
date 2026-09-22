# plugin_expansion

Minimal scaffold for STM32L496AGI3 post-compile expansion plugins.

## What this adds

- Four independent plugin slots mapped to `EXP_BOARD_0` through `EXP_BOARD_3`
- Required plugin entry points:
  - `bool init(exp_stack_t* stk, uint8_t expid)`
  - `task_status_t task(exp_stack_t* stk, uint8_t expid, task_ix_t self)`
  - `void UART_IRQHandler(USART_TypeDef* usart)`
- Access from each plugin to the main application objects through the exported
  host API:
  - `EXP_PLUGIN_DATABASE`
  - `EXP_PLUGIN_EEPROM_SAVE_DATA`

## Repository layout

- `include/plugin_expansion_app_types.h`
  - Template type bridge between the main application and plugin code
- `include/plugin_expansion.h`
  - Plugin ABI, section macros, and descriptor declaration helpers
- `linker/plugin_expansion_sections.ld`
  - Linker `SECTIONS` fragment for the four `EXP_BOARD_*` memory regions

## Main application integration

Include the linker fragment from the STM32 application linker script after the
`MEMORY` block that defines `EXP_BOARD_0` through `EXP_BOARD_3`.

```ld
INCLUDE "linker/plugin_expansion_sections.ld"
```

The expansion blocks must be loadable sections, not `NOLOAD`, so the plugin
descriptors and plugin code are emitted into flash.

The provided placement helpers isolate the symbols you explicitly annotate.
Unannotated helper functions or `const` objects will remain in the compiler's
default sections unless your host project adds its own sectioning rules.
Annotated symbols are also marked `used`, but helpers still only control the
symbols they decorate rather than an entire translation unit automatically.

In the STM32 application, provide one host API definition that points at the
application-owned objects:

```c
const exp_plugin_host_api_t exp_plugin_host_api = {
    .database = &database,
    .eeprom_save_data = &eeprom_save_data,
};
```

If the application already has its own concrete stack, task, database,
EEPROM-save, and USART types, include those headers first and then declare the
exact ABI aliases expected by `plugin_expansion.h` before defining
`PLUGIN_EXPANSION_HAS_APP_TYPES`. For example:

```c
#include "app_types.h"
#include "stm32l4xx.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#include "plugin_expansion.h"
```

`USART_TypeDef` should come from the STM32 CMSIS/HAL device header (for example
`stm32l4xx.h`) and must still be visible when `plugin_expansion.h` is included.

The fallback declarations in `plugin_expansion_app_types.h` are only a generic
placeholder for isolated syntax checks. Real STM32 integration should include
the application/CMSIS headers first so the exact project definitions of
`exp_stack_t`, `task_status_t`, `task_ix_t`, `database_t`, and
`eeprom_save_data_t` are used. `USART_TypeDef` must always come from the STM32
device header and should not be supplied by the fallback bridge.

If the host wants a different exported symbol name, define
`EXP_PLUGIN_HOST_API_SYMBOL` before including `plugin_expansion.h`.

The descriptor stores a reference to `EXP_PLUGIN_HOST_API_SYMBOL`, so the final
plugin module is intended to be emitted as a relocatable post-compile object
that is linked or loaded with a host-provided definition of that symbol (or
with a test stub that provides the same `exp_plugin_host_api_t` object during
standalone object validation). This scaffold does not produce a fully
self-contained flash image with no host symbol-resolution step.

## Declaring a plugin

```c
#include "app_types.h"
#include "stm32l4xx.h"

typedef app_exp_stack_t exp_stack_t;
typedef app_task_status_t task_status_t;
typedef app_task_ix_t task_ix_t;
typedef app_database_t database_t;
typedef app_eeprom_save_data_t eeprom_save_data_t;

#define PLUGIN_EXPANSION_HAS_APP_TYPES
#include "plugin_expansion.h"

static bool exp0_init(exp_stack_t *stk, uint8_t expid);
static task_status_t exp0_task(exp_stack_t *stk, uint8_t expid, task_ix_t self);
static void exp0_uart_irq(USART_TypeDef *usart);

EXP_PLUGIN_DECLARE_SLOT_0(exp0_descriptor, exp0_init, exp0_task, exp0_uart_irq);
```

Use `EXP_PLUGIN_DECLARE_WITH_LINKAGE_SLOT_0(extern, ...)` if a descriptor
definition for slot 0 must be emitted with external linkage and be visible
outside its defining translation unit.

Optional placement helpers are available for slot-specific code and flash-resident
constants:

```c
EXP_PLUGIN_CODE(0) static void exp0_helper(void)
{
}

EXP_PLUGIN_CONST(0) static const uint8_t exp0_table[] = { 0x01, 0x02 };
```

This keeps each plugin image isolated in its corresponding `EXP_BOARD_*` block.
Writable runtime state should stay in the main application or in an
application-owned RAM structure that is passed through `exp_stack_t`.
This scaffold does not emit plugin-owned writable `.data` or `.bss` into the
`EXP_BOARD_*` blocks.
