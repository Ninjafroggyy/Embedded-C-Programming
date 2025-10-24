/*
===========================================================
   Project Title : Multi-Function Traffic, LCD, Motor & ADC System
   Microcontroller : PIC18F4550
   Compiler        : CCS C Compiler
   Simulation Tool : Proteus
   Description     :
       This project integrates multiple hardware features:
         • Reads three analog inputs (AN0–AN2) via potentiometers
         • Displays ADC readings on a 20x4 LCD
         • Controls a DC motor’s rotation direction using analog input
         • Runs a traffic light sequence for two intersections (A & B)
         • Responds to three push buttons for triggering modes:
              - RB4: Display ADC values
              - RB5: Control motor rotation
              - RB7: Run traffic light simulation
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
unsigned int value0, value1, value2;     // ADC readings for AN0–AN2

// Traffic light patterns
unsigned int lights_A[9] = {0x01, 0x03, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01};
unsigned int lights_B[9] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x04, 0x02};

// -------------------- Interrupt Service Routines --------------------
#INT_TIMER2
void TIMER2_isr(void) {
   count = 1;   // Timer2 interrupt flag
}

#INT_EXT
void EXT_isr(void) {
   // Reserved for external interrupt INT0
}

#INT_EXT1
void EXT1_isr(void) {
   count = 2;   // External interrupt 1
}

#INT_EXT2
void EXT2_isr(void) {
   count = 3;   // External interrupt 2
}

// -------------------- MAIN PROGRAM --------------------
void main() {
   // --- ADC and Timer Setup ---
   setup_adc_ports(AN0_TO_AN2);                    // Enable analog inputs AN0–AN2
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);  // Internal ADC clock
   setup_timer_2(T2_DIV_BY_16, 249, 15);           // Timer2: 2 ms overflow, 30 ms interrupt
      
   // --- Enable Interrupts ---
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_EXT2);
   enable_interrupts(GLOBAL);

   // --- Initialize LCD ---
   lcd_init();

   while(TRUE) {
      // --- LED Blinking (heartbeat indicator) ---
      output_low(LED);
      delay_ms(DELAY);
      output_high(LED);
      delay_ms(DELAY);   

      // --- RB4 Button: Read and print analog values on LCD ---
      if (input(PIN_B4)) {
         analogs();
         printanalogs();
      }
      
      // --- RB5 Button: Read analogs and control motor direction ---
      if (input(PIN_B5)) {
         analogs();
         rotate();
      }
      
      // --- RB7 Button: Run traffic light sequence ---
      if (input(PIN_B7)) {
         for (int q = 0; q <= 7; q++) {

            // ----- Traffic Light Set A (PORTA pins RA4–RA6) -----
            switch(lights_A[q]) {
               case 0x01:  output_high(PIN_A4); output_low(PIN_A5); output_low(PIN_A6); break; // Red
               case 0x02:  output_low(PIN_A4);  output_high(PIN_A5); output_low(PIN_A6); break; // Yellow
               case 0x03:  output_high(PIN_A4); output_high(PIN_A5); output_low(PIN_A6); break; // Red+Yellow
               case 0x04:  output_low(PIN_A4);  output_low(PIN_A5);  output_high(PIN_A6); break; // Green
               default: break;
            }

            // ----- Traffic Light Set B (PORTE pins RE0–RE2) -----
            switch(lights_B[q]) {
               case 0x01:  output_high(PIN_E0); output_low(PIN_E1); output_low(PIN_E2); break;
               case 0x02:  output_low(PIN_E0);  output_high(PIN_E1); output_low(PIN_E2); break;
               case 0x03:  output_high(PIN_E0); output_high(PIN_E1); output_low(PIN_E2); break;
               case 0x04:  output_low(PIN_E0);  output_low(PIN_E1);  output_high(PIN_E2); break;
               default: break;
            }

            delay_ms(2000);  // Wait before changing to next state
         }
      }
   }
}

// -------------------- ADC Reading Function --------------------
void analogs(void) {
   set_adc_channel(0); delay_us(30); value0 = read_adc();  // AN0
   set_adc_channel(1); delay_us(30); value1 = read_adc();  // AN1
   set_adc_channel(2); delay_us(30); value2 = read_adc();  // AN2
}

// -------------------- Motor Control Based on Analog Input --------------------
void rotate(void) {
   if (value0 < 120) {                   // Rotate forward
      output_high(PIN_C0);
      output_low(PIN_C1);
   }
   else if (value0 > 170) {              // Rotate reverse
      output_high(PIN_C1);
      output_low(PIN_C0);
   }
   else {                                // Stop motor
      output_low(PIN_C0);
      output_low(PIN_C1);
   }
}

// -------------------- LCD Display for ADC Values --------------------
void printanalogs(void) {
   lcd_gotoxy(1, 1);
   printf(lcd_putc, "Value0 = %3u", value0);
   
   lcd_gotoxy(1, 2);
   printf(lcd_putc, "Value1 = %3u", value1);
   
   lcd_gotoxy(21, 1);
   printf(lcd_putc, "Value2 = %3u", value2);
}
