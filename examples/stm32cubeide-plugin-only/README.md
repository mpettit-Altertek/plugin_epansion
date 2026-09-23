# Plugin-only STM32CubeIDE project

This directory is a standalone STM32CubeIDE/Eclipse Makefile project for
building only the plugin example.

## Files

- `.project`
  - importable STM32CubeIDE project metadata
- `Makefile`
  - builds the plugin example as a linked relocatable object
- `plugin_layout.ld`
  - local linker script that defines the four expansion-board memory regions and
    includes the shared plugin section layout

## Build configurations

The Makefile supports four plugin-slot configurations:

- `slot0`
- `slot1`
- `slot2`
- `slot3`

Each configuration sets `EXAMPLE_PLUGIN_SLOT` to the matching slot index and
produces:

`build/<config>/plugin_image_<config>.o`

Examples:

```sh
make slot0
make slot1
make CONFIG=slot2
```

## CubeIDE usage

1. Import `examples/stm32cubeide-plugin-only` from your local checkout as an
   existing STM32CubeIDE/Eclipse project.
2. Ensure the toolchain commands used by `Makefile` are available, or override
   `CC` / `AR` in the build environment.
3. Build the desired target (`slot0`..`slot3`) from the project.

This project compiles the shared `examples/slot0_example_plugin.c` source only;
it does not compile the host application example.
The generated linked object remains a relocatable plugin artifact and still expects
host-side resolution of `EXP_PLUGIN_HOST_API_SYMBOL` during final integration.
