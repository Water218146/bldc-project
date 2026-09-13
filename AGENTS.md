# Repository Guidelines

## Project Structure & Module Organization

- `Source/APP`: application entry points and board definitions; `Source/Bsp`: clock, GPIO, and interrupt setup.
- `Driver/Motor` and `Driver/AD56xx`: motor-control and AD56xx drivers. Keep each module's `.c` and `.h` files together.
- `Libraries/Lib/n32g43x_std_periph_driver`: N32G43x standard peripheral library; `Libraries/SysCore`, `SysConfig`, and `Stratup`: CMSIS, device configuration, and startup code.
- `Project/motor_bldc.uvprojx`: Keil uVision project targeting `N32G435RB`. Treat `.o`, `.d`, `.axf`, `.map`, and similar files in `Project` as generated output.
- `Document`: pin assignments and hardware records. Put new schematics, captures, and test notes here with the relevant revision.

## Build, Test, and Development Commands

Open `Project/motor_bldc.uvprojx` in Keil uVision, select `Motor_BLDC`, and run **Build** or **Rebuild**. With Keil installed, the equivalent command is `UV4.exe -b Project\motor_bldc.uvprojx -j0`; configure its path locally. Before flashing, confirm debugger, device, clock, and Flash settings match the board. There is no standalone unit-test framework or CI configuration. Produce a warning-free build and exercise startup, PWM, commutation, protection, and communications on the target board.

## Coding Style & Naming Conventions

Follow the existing embedded C style: use four-space indentation and avoid reformatting unrelated legacy tabs. Preserve uppercase/underscore names for public APIs and macros (for example, `RCC_Configuration` and `DMA_SetPerMemAddr`); keep local-variable style consistent within each file. Pair modules as `.c`/`.h`, expose public interfaces in headers, mark private symbols `static`, and use `volatile` for register or interrupt-shared state where required. Do not block inside interrupt handlers.

## Testing Guidelines

No coverage threshold is defined. Record the build configuration, test board, power conditions, and observed results in the change description. Motor-control changes should include current limiting, fault handling, and reset recovery checks. Store logs, oscilloscope captures, and firmware version details under `Document`.

## Commit & Pull Request Guidelines

The repository has no existing commit history to follow. Use a short, imperative subject such as `Add BLDC overcurrent protection`, and keep each commit focused. Pull requests should explain purpose, affected modules, hardware assumptions, and build/board validation results. Include screenshots or waveforms for pin, timing, or signal changes and link the related issue or requirement. Do not commit secrets, personal debugger settings, or large build artifacts.

## Hardware & Configuration Notes

Before changing pin assignments, startup files, scatter-loading scripts, or clock configuration, verify the N32G435RB datasheet and schematic. Any PWM, ADC, DMA, interrupt-priority, or protection-threshold change must update interface comments and the corresponding board-validation record.
