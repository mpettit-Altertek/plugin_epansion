# Examples

This directory contains a minimal example plugin built against the
`plugin_expansion.h` ABI.

- `example_app_types.h`
  - standalone host-side type aliases used only for the example
- `slot0_example_plugin.c`
  - a simple plugin showing:
    - slot selection with `EXAMPLE_PLUGIN_SLOT=0..3`
    - host type aliasing
    - standalone host API stub override for local builds
    - descriptor declaration
    - slot-local code placement
    - slot-local constant placement
- `example_host_application.c`
  - application-side example showing host symbol export and descriptor use
- `stm32cubeide/.project`
  - importable STM32CubeIDE/Eclipse project metadata for the example

The example is intentionally simple and keeps writable runtime state out of the
plugin image. Any mutable state should live in the host application and be
reached through `exp_stack_t` or the exported host API object.
