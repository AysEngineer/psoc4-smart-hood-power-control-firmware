# PSoC 4 Based Smart Motor Control Unit

This repository contains the firmware components developed for a PSoC 4 based power control board. The project focuses on industrial motor control, power management, and secure communication.

## Key Highlights
- **Safe Motor Switching:** Managed 5-speed motor relay logic with safety transitions.
- **Secure Communication:** Implemented a UART-based protocol with **CRC-16 (CCITT)** for EMI resistance.
- **Power Management:** Configured Deep-Sleep modes and wake-up routines for high energy efficiency.
- **Data Integrity:** Real-time logging of filter status and working hours to EEPROM.

## Hardware Environment
- **MCU:** Cypress PSoC 4 (ARM Cortex-M0)
- **IDE:** PSoC Creator
- **Peripherals:** UART, EEPROM, Watchdog Timer, GPIO/Relay Drivers
