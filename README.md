# psoc4-smart-hood-power-control-firmware

This repository contains the production-grade embedded firmware developed for the Power Control Board of a smart cooker hood and air purification appliance. Built on the **Cypress PSoC 4 (ARM Cortex-M0)** architecture, the codebase focuses on high-power load handling, low-power state transitions, and real-time data integrity under electromagnetic interference (EMI).

## 📌 Project Context
The appliance utilizes a two-board split architecture: a **Control/Display UI Panel** and a master **Power Control Board**. This firmware drives the Power Control Board, translating user UI inputs into physical actions while ensuring hardware safety and energy efficiency compliance

## Key Highlights
- **Safe Multi-Speed Motor Switching:** Manages a 5-speed inductive fan motor relay matrix. Implements strict software interlocking and transition delays to eliminate sudden inrush current spikes, protecting the physical circuitry.
- **EMI-Resilient Communication (UART + CRC-16):** High-power AC motor operations inject electrical noise into communication lines. To protect data sent between the UI panel and the power board, I designed a custom UART protocol sealed with a **CRC-16 (CCITT)** error-checking mechanism.
- **Smart Power Management:** Configures the PSoC 4’s **Deep-Sleep modes** to dramatically lower standby energy consumption. Features bounce-free wakeup routines triggered instantly by user touch or remote control signals.
- **Emulated EEPROM Data Persistence:** Since the PSoC 4 silicon lacks a physical on-chip EEPROM, this firmware utilizes the `CyFlash` system library for **EEPROM Emulation**. It safely logs critical metrics like total working hours and filter saturation status directly into Flash memory, preventing data loss during power cuts.
- **System Reliability (Watchdog Timer):** Integrates a hardware Watchdog Timer (WDT) to automatically detect and recover from any unpredictable software stalls, ensuring continuous 24/7 operation.

## Hardware Environment
- **MCU:** Cypress PSoC 4 (ARM Cortex-M0)
- **IDE:** PSoC Creator
- **Peripherals:** UART (with CRC), Emulated EEPROM (Flash), Watchdog Timer, GPIO/Relay Drivers
- **Programming Language:** Embedded C

