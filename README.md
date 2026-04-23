# Deterministic Embedded Control Loop with Fault Detection & Recovery

## Overview
This project implements a bare-metal embedded control loop using a fixed-period scheduler driven by SysTick. It simulates a real-world embedded system that must operate under strict timing constraints while handling unreliable hardware inputs.

The system demonstrates:
- Deterministic scheduling
- Deadline (overrun) detection
- Fault injection and validation
- Safe recovery using fallback logic

## Key Features

### 1. Fixed-Period Scheduler
- Uses SysTick as a 1ms time base
- Executes a control loop every 10ms
- Maintains deterministic timing under normal conditions

### 2. Overrun Detection
- Measures how late the system executes relative to its scheduled time
    ```c
    lateness = g_ms_ticks - next_run;
    ```
- Detects when the control loop exceeds its time budget
    ```c
    if (lateness_ms > 0) 
    ```
- Tracks total overruns
- Tracks worst-case lateness

### 3. Recovery Policy
Two scheduling behaviors are demonstrated:

- **Deterministic Mode**  
  Maintains a fixed schedule and preserves timing alignment, but may remain behind if overloaded.

- **Resync Mode**  
  Resets the schedule after an overrun to prevent accumulated drift, trading strict periodicity for stability.

### 4. Simulated Sensor System
- Uses memory-mapped style sensor registers
- Injects controlled faults at defined intervals
- Validates readings using status and range checks

### 5. Fault-Tolerant Behavior
When invalid data is detected:
- The system rejects the reading
- Falls back to the last known good value
- Continues operation safely

### 6. Controlled Overrun Testing
- Injects intentional CPU load to trigger deadline misses
- Demonstrates how excessive work affects real-time scheduling

## Project Structure
```.
├── main.c        # Scheduler, timing, overrun detection
├── sensor.c      # Control loop, fault injection, validation
├── sensor.h      # Sensor module interface
├── logger.c      # USART logging implementation
├── logger.h      # Logging interface
├── hardware.h    # Register mappings and MCU definitions
├── global.h      # Shared macros and utilities
```

## How It Works
1. SysTick generates a 1ms interrupt and increments a global tick counter.
2. The scheduler checks whether the next release time has been reached.
3. The control loop executes on a fixed period.
4. Sensor data is read and validated.
5. Invalid data triggers recovery using the last known good value.
6. If the loop misses its deadline, the system detects and reports an overrun.

## Example Output
```sh
[OK] value=42
[OK] value=43
[FAULT] invalid reading (999)
[RECOVERY] using last_known_good=43

*** [OVERRUN] late by 5 ms (count=1 worst=5)
```

## What This Demonstrates
This project focuses on practical embedded systems concepts:
- Designing around time constraints
- Handling unreliable hardware inputs
- Building systems that fail safely
- Understanding tradeoffs between determinism and recovery

## Design Tradeoffs
- **Fixed Schedule**  
  Preserves deterministic timing, but can remain behind after missed deadlines.

- **Resync**  
  Restores stability after overruns, but sacrifices strict timing alignment for that cycle.

This project demonstrates both approaches and the tradeoffs between them.

## Build & Run
- Toolchain: ARM GNU Toolchain (arm-none-eabi-gcc) must be available in PATH
- Target: STM32 bare-metal environment
- Build:
  - Run `make` to generate ELF and binary outputs in the `out/` directory
- Flash:
  - Run `make flash` (requires OpenOCD installed)
  - Update `OPENOCD` and `OPENOCD_SCRIPTS` paths in the Makefile if needed, or override via:
    - `make flash OPENOCD=/path/to/openocd OPENOCD_SCRIPTS=/path/to/scripts`
- Debug:
  - Run `make debug` to start OpenOCD server

## Future Improvements
- Non-blocking logging using DMA or a ring buffer
- RTOS-based scheduler comparison
- Additional sensor types and validation strategies
- Hardware-backed fault inputs instead of software-only injection

## Contact

Edwin J Martinez <br/>
**Software/Firmware Engineer | Embedded Systems** <br/>
[GitHub](https://github.com/jodadev ) 
[LinkedIn](https://linkedin.com/in/jodadev)
