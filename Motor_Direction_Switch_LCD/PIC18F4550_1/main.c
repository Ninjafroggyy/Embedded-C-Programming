/*
===========================================================
   Project Title : Motor Direction Control via Switch & LCD
   Microcontroller : PIC18F4550
   Compiler        : CCS C Compiler
   Simulation Tool : Proteus
   Description     :
       This program reads four switches connected to PORTB (RB4–RB7)
       to determine motor control commands and display their status
       on an LCD. The switches determine the motor direction or stop
       state as follows:

         SW1 (case 1): Clockwise rotation
         SW2 (case 2): Anti-clockwise rotation
         SW3 (case 4): Stop motor
         SW4 (case 8): Display warning ("gone too far")

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

// -------------------- Global Variables --------------------
unsigned int swt_way = 0;   // Stores switch pattern from PORTB

// -------------------- Function Prototypes --------------------
void read_switch(void);      // Reads switch states and updates LCD

// -------------------- MAIN PROGRAM --------------------
void main() 
{
   unsigned int q = 0;       // Local counter variable (unused)
   lcd_init();               // Initialize the LCD

   while(TRUE) 
   {
      read_switch();         // Continuously check switch inputs
      
      // --- Control Motor and Display Actions Based on Switch Input ---
      switch(swt_way) 
      {
         // Motor rotates clockwise
         case 1:  
            output_high(PIN_C0);
            output_low(PIN_C1);
            break;

         // Motor rotates anti-clockwise
         case 2:  
            output_low(PIN_C0);
            output_high(PIN_C1);
            break;

         // Motor stops
         case 4:  
            output_low(PIN_C0);
            output_low(PIN_C1);
            break;

         // Limit or error condition
         case 8:  
            lcd_gotoxy(1, 3);
            printf(lcd_putc, " gone too far");
            break;

         // No valid switch pressed
         default: 
            break;
      }
   }
}

// -------------------- SWITCH READING FUNCTION --------------------
void read_switch(void) 
{
   unsigned int swt = 0;

   swt = input_b();     // Read the entire PORTB
   swt = swt & 0xF0;    // Mask lower nibble (keep bits RB4–RB7)
   swt /= 16;           // Shift right by 4 bits (convert to 0–15 range)
   
   swt_way = swt;       // Store processed switch value

   lcd_gotoxy(1, 1);    // Display result on LCD
   printf(lcd_putc, "swt_way = %x", swt_way);
}
