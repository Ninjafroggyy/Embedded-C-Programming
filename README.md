# PIC Microcontroller Projects (PIC18F45K50)

A curated collection of embedded C programs and Proteus simulations designed for the PIC18F and PIC16F microcontroller families.
Each project demonstrates a specific control, timing, or interfacing concept — including ADC integration, motor control, LED drivers, LCD interfacing, and interrupt-based software timing.

Developed and tested using CCS C and Proteus 8 Professional.

---

## Tools

- CCS C Compiler 
- Proteus 8 Professional (schematic and simulation)
- Target: PIC18F45K50 unless otherwise stated in the file header

---

## Files and Summaries

| Project Name | Microcontroller | Description |
|---------------|----------------|--------------|
| **ADC5_Timer_LCD_Counter.c** | PIC18F26K20 | Timer2-driven round-robin ADC scan (AN0–AN4) with LCD output and event counter. Demonstrates periodic sampling and interrupt control. |
| **ADC_LED_Motor_Display.c** | PIC18F25K22 | Three-pot LCD readout, LED bar display, and button-cycled motor state machine. Shows ADC scaling and user input handling. |
| **Analog_LED_LCD_Motor_Controller.c** | PIC18F45K50 | Multifunction controller: reads AN0–AN2, displays on LCD and LEDs, includes button-driven motor modes and Knight Rider LED sequence. |
| **Dual_ADC_Dual_Button_LCD.c** | PIC16F616 | Two ADC channels displayed on LCD with two buttons triggering separate functions and counters. Simple dual-input demonstration. |
| **LCD_ADC_BUTTON.c** | PIC16F616 | Basic demonstration of ADC input and button-based LCD interaction. Ideal for introductory testing of input/output flow. |
| **Mode_Switch_LCD_Motor_ADC.c** | PIC18F26K20 | Four operational modes via input switches: show ADC, drive motor CW, drive motor CCW, and Knight Rider LED animation. |
| **Motor_Direction_Switch_LCD.c** | PIC18F4550 | Reads RB4–RB7 as a 4-bit command to drive motor clockwise, anticlockwise, or stop, with LCD displaying the current state. |
| **MultiFunction_Traffic_LCD_Motor_ADC.c** | PIC18F4550 | Combined system with traffic light logic, motor control, and multiple ADC readings displayed on LCD. |
| **MultiIO_Controller_LCD_ADC_Motor_Buzzer.c** | PIC18F45K50 | Multi-I/O controller integrating LCD display, ADC inputs, motor drive, and buzzer output — a full system demonstration. |
| **ScaledProduct_LCD_Pot.c** | PIC18F26K20 | Demonstrates arithmetic and function parameter passing: calculates (16 × 15 × pot value) and displays the result on LCD. |
| **Timer2_MultiClock_LED_Driver.c** | PIC18F24K20 | Timer2 ISR updates eight independent “software clocks” toggling RC0–RC7 at unique frequencies. Example of multitasking via interrupts. |
| **TrafficControl_Analog_LCD_Motor.c** | PIC18F4550 | Simulates traffic control sequence using dual light sets, LCD status display, ADC input, and motor drive. |

> Each source file includes its own LCD pin definitions, LED mappings, and motor/buzzer wiring details.  
> Refer to the top comments in each `.c` file for schematic references and specific hardware pinouts.

---

Embedded systems projects for coursework, simulation, and portfolio use.  
