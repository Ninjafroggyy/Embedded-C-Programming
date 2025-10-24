/*
===========================================================
   Project Title : Dual ADC and Dual Button LCD Interface
   Microcontroller : PIC16F876
   Compiler        : CCS C Compiler
   Simulation Tool : Proteus
   Description     : 
       This program reads two analog inputs (from potentiometers)
       and displays their ADC values on a 16x2 LCD.
       Two push buttons are also connected:
         - Button 1 (RA3) displays an incrementing counter on line 2
         - Button 2 (RA4) displays another counter on line 1
===========================================================
*/

// -------------------- LCD Pin Configuration --------------------
#define LCD_ENABLE_PIN PIN_C2   // LCD Enable connected to RC2
#define LCD_RS_PIN     PIN_C0   // Register Select pin connected to RC0
#define LCD_RW_PIN     PIN_C1   // Read/Write pin connected to RC1
#define LCD_DATA4      PIN_C3   // LCD Data line D4 connected to RC3
#define LCD_DATA5      PIN_C4   // LCD Data line D5 connected to RC4
#define LCD_DATA6      PIN_C5   // LCD Data line D6 connected to RC5
#define LCD_DATA7      PIN_A5   // LCD Data line D7 connected to RA5

// -------------------- Library Includes --------------------
#include <main.h>
#include <lcd.c>                // Include LCD driver for 4-bit operation

// -------------------- Variable Declarations --------------------
unsigned int value = 0;         // ADC result from channel AN0 (RV1)
unsigned int value1 = 0;        // ADC result from channel AN1 (RV2)
unsigned int val = 0;           // Counter for button 1 (RA3)
unsigned int val1 = 0;          // Counter for button 2 (RA4)

// -------------------- MAIN PROGRAM --------------------
void main() 
{
   // --- Configure ADC ports ---
   setup_adc_ports(sAN0 | sAN1);     // Enable analog inputs on AN0 and AN1
   setup_adc(ADC_CLOCK_INTERNAL);    // Use internal ADC clock

   // --- Initialize LCD ---
   lcd_init();                       // Prepare LCD for use

   while(TRUE) 
   {
      // --- Read first potentiometer (RV1) ---
      set_adc_channel(0);            // Select channel AN0
      delay_us(30);                  // Wait for acquisition
      value = read_adc();            // Read 10-bit ADC value (0–1023)
      
      // --- Read second potentiometer (RV2) ---
      set_adc_channel(1);            // Select channel AN1
      delay_us(30);
      value1 = read_adc();

      // --- Display both ADC readings ---
      lcd_gotoxy(1, 1);              // Cursor: line 1, column 1
      printf(lcd_putc, "Hello %u    ", value);   // Print first ADC value
	 
      lcd_gotoxy(1, 2);              // Cursor: line 2, column 1
      printf(lcd_putc, "Hello %u    ", value1);  // Print second ADC value

      // --- Button 1 (RA3): show counter on line 2 ---
      if (input(PIN_A3)) 
      {
         printf(lcd_putc, "\f");     // Clear LCD
         lcd_gotoxy(26, 2);          // Move cursor (position wraps around)
         printf(lcd_putc, "This is %u  ", val);
         val++;                      // Increment counter
      }
      
      // --- Button 2 (RA4): show counter on line 1 ---
      if (input(PIN_A4)) 
      {
         printf(lcd_putc, "\f");     
         lcd_gotoxy(26, 1);          
         printf(lcd_putc, "This is %u  ", val1);
         val1++;
      }
   }
}
