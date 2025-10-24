/*
===========================================================
   Project Title : Traffic Light, ADC Display & Motor Control
   Microcontroller : PIC18F4550
   Compiler        : CCS C Compiler
   Simulation Tool : Proteus
   Description     :
       This program demonstrates the use of multiple peripherals:
       - Reads three analog sensors (AN0–AN2)
       - Controls traffic light LEDs on PORTA and PORTE
       - Drives a motor output on PORTC based on analog input
       - Displays ADC readings on a 20x4 LCD
       - Responds to button inputs to trigger various functions
===========================================================
*/

// -------------------- LCD Pin Configuration --------------------
#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN     PIN_D0
#define LCD_RW_PIN     PIN_D1
#define LCD_DATA4      PIN_D4
#define LCD_DATA5      PIN_D5
#define LCD_DATA6      PIN_D6
#define LCD_DATA7      PIN_D7

// -------------------- Library Includes --------------------
#include <main.h>
#include <lcd.c>

// -------------------- Function Prototypes --------------------
void analogs(void);
void printanalogs(void);
void rotate(void);

// -------------------- Global Variables --------------------
unsigned int count;
unsigned int value0, value1, value2;   // ADC readings
unsigned int lights_A[9] = {0x01, 0x03, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01}; // Sequence for Set A (RA4–RA6)
unsigned int lights_B[9] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x04, 0x02}; // Sequence for Set B (RE0–RE2)

// -------------------- Interrupt Service Routines --------------------
#INT_TIMER2
void TIMER2_isr(void) {
   count = 1;        // Timer interrupt sets flag
}

#INT_EXT
void EXT_isr(void) {
   // Reserved for external interrupt on INT0
}

#INT_EXT1
void EXT1_isr(void) {
   count = 2;        // External interrupt 1 flag
}

#INT_EXT2
void EXT2_isr(void) {
   count = 3;        // External interrupt 2 flag
}

// -------------------- MAIN PROGRAM --------------------
void main() {
   // --- Setup ADC and Timer ---
   setup_adc_ports(AN0_TO_AN2);                     // Enable analog inputs AN0–AN2
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);   // Internal ADC clock
   setup_timer_2(T2_DIV_BY_16, 249, 15);            // Timer2: 2ms overflow, 30ms interrupt
      
   // --- Enable Interrupts ---
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_EXT2);
   enable_interrupts(GLOBAL);

   // --- Initialize LCD ---
   lcd_init();

   while(TRUE) {
      // --- Example LED blink (heartbeat indicator) ---
      output_low(LED);
      delay_ms(DELAY);
      output_high(LED);
      delay_ms(DELAY);   

      // --- Button RB4: Read and print analog values ---
      if (input(PIN_B4)) {
         analogs();
         printanalogs();
      }
      
      // --- Button RB5: Read analogs and control motor rotation ---
      if (input(PIN_B5)) {
         analogs();
         rotate();
      }
      
      // --- Button RB7: Simulate traffic light sequence ---
      if (input(PIN_B7)) {
         for (int q = 0; q <= 7; q++) {
            // ----- Lights Set A (PORTA) -----
            switch(lights_A[q]) {
               case 0x01:  output_high(PIN_A4); output_low(PIN_A5); output_low(PIN_A6); break; // Red
               case 0x02:  output_low(PIN_A4);  output_high(PIN_A5); output_low(PIN_A6); break; // Yellow
               case 0x03:  output_high(PIN_A4); output_high(PIN_A5); output_low(PIN_A6); break; // Red + Yellow
               case 0x04:  output_low(PIN_A4);  output_low(PIN_A5);  output_high(PIN_A6); break; // Green
               default: break;
            }

            // ----- Lights Set B (PORTE) -----
            switch(lights_B[q]) {
               case 0x01:  output_high(PIN_E0); output_low(PIN_E1); output_low(PIN_E2); break;
               case 0x02:  output_low(PIN_E0);  output_high(PIN_E1); output_low(PIN_E2); break;
               case 0x03:  output_high(PIN_E0); output_high(PIN_E1); output_low(PIN_E2); break;
               case 0x04:  output_low(PIN_E0);  output_low(PIN_E1);  output_high(PIN_E2); break;
               default: break;
            }
            delay_ms(2000);   // Step delay between state transitions
         }
      }
   }
}

// -------------------- Read Analog Inputs --------------------
void analogs(void) {
   set_adc_channel(0); delay_us(30); value0 = read_adc();  // AN0
   set_adc_channel(1); delay_us(30); value1 = read_adc();  // AN1
   set_adc_channel(2); delay_us(30); value2 = read_adc();  // AN2
}

// -------------------- Motor Rotation Control --------------------
void rotate(void) {
   if (value0 < 120) {                 // Rotate one direction
      output_high(PIN_C0);
      output_low(PIN_C1);
   }
   else if (value0 > 170) {            // Rotate opposite direction
      output_high(PIN_C1);
      output_low(PIN_C0);
   }
   else {                              // Stop motor
      output_low(PIN_C0);
      output_low(PIN_C1);
   }
}

// -------------------- Print ADC Values to LCD --------------------
void printanalogs(void) {
   lcd_gotoxy(1, 1);
   printf(lcd_putc, "Value0 = %3u", value0);
   
   lcd_gotoxy(1, 2);
   printf(lcd_putc, "Value1 = %3u", value1);
   
   lcd_gotoxy(21, 1);
   printf(lcd_putc, "Value2 = %3u", value2);
}
