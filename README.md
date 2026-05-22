# STM32F4 A6 GSM Module Project

A bare-metal embedded C project for the STM32F4 microcontroller that interfaces with the AI-Thinker A6 GSM/GPRS module to send and receive SMS messages. Built without HAL — direct register access only.

## Hardware

| Component | Details |
|-----------|---------|
| MCU | STM32F4 (HSI 16 MHz) |
| GSM Module | AI-Thinker A6 |
| A6 UART | USART1 — PA9 (TX), PA10 (RX) |
| Debug UART | USART2 — PA2 (TX), PA3 (RX) |
| LED | PA5 |
| Button | PC13 (active low, onboard) |
| ADC Input | PA1 (Channel 1) |

## Project Structure

```
├── a6_driver.c / .h      # Low-level USART1 & USART2 init, SysTick delay
├── a6_lib.c / .h         # A6 AT command logic, SMS send/receive, IRQ handler
├── fifo.c / .h           # Circular TX/RX FIFO buffers (1024 bytes each)
├── hardware_modules.c/.h # LED, button, ADC drivers
├── rtc.c / .h            # RTC init using LSI clock (bare-metal)
├── main.c                # Entry point
├── syscalls.c            # Newlib syscall stubs (printf support)
└── sysmem.c              # Heap allocator (_sbrk)
```

## Features

- **SMS sending** via AT commands (`AT+CMGF`, `AT+CMGS`)
- **SMS receiving** via USART1 interrupt + FIFO buffering
- **Caller number extraction** from raw `+CMT` response
- **ADC** continuous conversion on PA1
- **RTC** initialization with LSI oscillator and AM/PM format
- **Debug output** over USART2 using `printf` (retargeted via `__io_putchar`)

## How It Works

The A6 module communicates over USART1 at 115200 baud. Incoming data is captured in the `USART1_IRQHandler`, stored in a temp buffer, and scanned for known response strings (like `"OK"` or `"+CMT"`). A simple substring search (`wait_response` + `search_check`) is used to detect when the module has acknowledged a command.

SMS messages are parsed from the raw `+CMT` frame:
- `get_raw_sms()` — extracts the full CMT block
- `get_sender_number()` — finds the `+381...` number
- `get_sms_msg()` — extracts the message body after the timestamp




### UART Pins

Connect the A6 module as follows:

```
STM32 PA9  (USART1 TX) --> A6 RX
STM32 PA10 (USART1 RX) --> A6 TX
STM32 PA2  (USART2 TX) --> USB-UART adapter (for debug)
```

## Status

> ⚠️ **Work in progress.** Core SMS send/receive and peripheral drivers are functional. The RTC module is initialized but time-setting and reading functions are not yet implemented. The `main.c` currently only runs the ADC loop — SMS and A6 initialization calls are present but commented out or pending integration.

