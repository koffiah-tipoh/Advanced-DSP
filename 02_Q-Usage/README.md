# STM32F429ZI Bare-Metal LED Blink

A minimal bare-metal (register-level, no HAL) LED blink example for the
**Nucleo-F429ZI** board, written directly against CMSIS register definitions.

## Overview

This project toggles the on-board user LED (LD1, green) by directly
manipulating the STM32F429's GPIO and RCC peripheral registers — no
STM32Cube HAL, no external libraries. It's intended as a first bring-up /
sanity-check program to confirm toolchain, flashing, and board setup are
all working correctly before moving on to more complex peripheral or DSP
work on this board.

## Hardware

- **Board:** ST Nucleo-F429ZI (Nucleo-144 form factor)
- **MCU:** STM32F429ZIT6 — ARM Cortex-M4 with FPU, 180 MHz, 2MB Flash, 256KB SRAM
- **LED used:** LD1 (green), connected to **PB0**

> Note: this differs from smaller Nucleo-64 boards (e.g. Nucleo-F411RE),
> where the user LED is on PA5 instead. Port-pin mapping matters — verify
> against your specific board's schematic before reusing this code
> elsewhere.

## Code

```c
#include "stm32f4xx.h"

#define GPIOBEN    (1U<<1)
#define PIN0       (1U<<0)
#define LED_PIN    PIN0

int main() {
    RCC->AHB1ENR |= GPIOBEN;

    GPIOB->MODER |= (1U<<0);
    GPIOB->MODER &= ~(1U<<1);

    while(1){
        GPIOB->ODR ^= LED_PIN;
        for(int i = 0; i < 100000; i++) {}
    }
}
```

### How it works

1. **Enable the GPIOB peripheral clock** — `RCC->AHB1ENR` bit 1 corresponds
   to GPIOB (bit 0 = GPIOA, bit 1 = GPIOB, bit 2 = GPIOC, etc.). Peripherals
   are clock-gated by default on STM32 to save power, so this step is
   required before any GPIOB register access will take effect.
2. **Configure PB0 as general-purpose output** — the `MODER` register uses
   2 bits per pin to select its mode (`00`=input, `01`=output,
   `10`=alternate function, `11`=analog). For pin *n*, the relevant bits
   sit at position `2n` and `2n+1`. For PB0, that's bits 0–1: setting bit 0
   and clearing bit 1 selects output mode (`01`).
3. **Toggle the pin in a loop** — XOR-ing `GPIOB->ODR` with the LED pin
   mask flips its output state each iteration, with a crude busy-wait delay
   loop controlling the blink rate.

## Build & Flash

### Toolchain
- **STM32CubeIDE** (includes ARM GCC toolchain, ST-LINK GDB server, and
  debugger — no separate installs needed)
- On-board ST-LINK on the Nucleo-F429ZI (USB connection to the ST-LINK port,
  not the USB OTG port)

### Steps

1. Open **STM32CubeIDE** and create a new **STM32 project**, selecting the
   **NUCLEO-F429ZI** board (this auto-generates the correct CMSIS device
   headers, startup file, and linker script for the board).
2. Replace the generated `main.c` contents inside the
   `USER CODE BEGIN` / `END` markers with the code above (or replace the
   file entirely if not using CubeMX-generated peripheral init).
3. Build the project: **Project → Build Project** (or the hammer icon).
4. Flash and run: **Run → Run** (or the green Run button), which builds,
   flashes over the on-board ST-LINK, and starts execution automatically.

No manual linker script, startup file, or OpenOCD command-line invocation
is needed — CubeIDE generates and manages all of this internally as part of
project creation.


## Expected Result

LD1 (green LED) on the Nucleo-F429ZI blinks continuously once flashed.

## Next Steps

This bring-up program is a starting point for further bare-metal or
CMSIS-DSP work on this board (UART drivers, timers, ADC sampling, FFT/filter
pipelines, etc.).
