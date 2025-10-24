/* ============================================================
   File:        ADC_LED_Motor_Display.c
   Target MCU:  PIC16F877A
   Description: 
      - Reads three analogue inputs from potentiometers (AN0–AN2)
      - Displays their values on an LCD
      - Button 1 (RA4): shows analogue values on LEDs sequentially
      - Button 2 (RA5): cycles motor through CW ? Stop ? CCW ? Stop
   ============================================================ */

#include <main.h>

/* ---------------- LCD Pin Configuration ---------------- */
#define LCD_ENABLE_PIN PIN_C0
#define LCD_RS_PIN     PIN_C2
#define LCD_RW_PIN     PIN_C1
#define LCD_DATA4      PIN_C4
#define LCD_DATA5      PIN_C5
#define LCD_DATA6      PIN_C6
#define LCD_DATA7      PIN_C7
#include <lcd.c>

/* ---------------- Function Prototypes ---------------- */
void welcome(void);
void display_conversion(void);
void led_display(void);
void rotates_motor(void);

/* ---------------- Global Variables ------------------- */
unsigned int value0 = 0, value1 = 0, value2 = 0;  // ADC readings
unsigned int counter = 0;                         // Motor control state counter


/* ============================================================
   Main Program Loop
   ============================================================ */
void main()
{
   // Configure analogue inputs on AN0–AN2
   setup_adc_ports(sAN0 | sAN1 | sAN2, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);

   lcd_init();   // Initialise LCD display

   while(TRUE)
   {
      // Task 1 – Display potentiometer values on LCD
      display_conversion();

      // Task 2 – When Button 1 (RA4) is pressed, show values on LEDs
      if (input(PIN_A4)) 
      {
         led_display();
      }

      // Task 3 – When Button 2 (RA5) is pressed, control motor direction
      if (input(PIN_A5)) 
      {
         rotates_motor();
      }
   }
}


/* ============================================================
   Function: welcome()
   Purpose:  Displays a welcome message (optional start screen)
   ============================================================ */
void welcome(void)
{
   output_low(LED);
   delay_ms(DELAY);
   output_high(LED);
   delay_ms(DELAY);

   printf(lcd_putc, "\f");            // Clear LCD
   lcd_gotoxy(4,1);
   printf(lcd_putc, "Welcome to the");
   lcd_gotoxy(4,2);
   printf(lcd_putc, "Embedded C Demo");
   lcd_gotoxy(26,1);
   printf(lcd_putc, "Practical");
   lcd_gotoxy(23,2);
   printf(lcd_putc, "Assessment 22/23");

   while(TRUE);  // Stay on welcome screen
}


/* ============================================================
   Function: display_conversion()
   Purpose:  Reads the three potentiometers (A, B, C) 
             and displays their ADC values on the LCD.
   ============================================================ */
void display_conversion(void)
{
   // Read Potentiometer A (AN0)
   set_adc_channel(0);
   delay_us(30);
   value0 = read_adc();

   // Read Potentiometer B (AN1)
   set_adc_channel(1);
   delay_us(30);
   value1 = read_adc();

   // Read Potentiometer C (AN2)
   set_adc_channel(2);
   delay_us(30);
   value2 = read_adc();

   // Display readings on LCD
   lcd_gotoxy(5, 1);
   printf(lcd_putc, "Value0 = %3u", value0);

   lcd_gotoxy(5, 2);
   printf(lcd_putc, "Value1 = %3u", value1);

   lcd_gotoxy(25, 1);
   printf(lcd_putc, "Value2 = %3u", value2);
}


/* ============================================================
   Function: led_display()
   Purpose:  When Button 1 (RA4) is pressed, display each 
             potentiometer value on LEDs (RB0–RB7) 
             for 3 seconds each, sequentially.
   ============================================================ */
void led_display(void)
{
   // Read Potentiometer A (AN0)
   set_adc_channel(0);
   delay_us(30);
   value0 = read_adc();

   // Read Potentiometer B (AN1)
   set_adc_channel(1);
   delay_us(30);
   value1 = read_adc();

   // Read Potentiometer C (AN2)
   set_adc_channel(2);
   delay_us(30);
   value2 = read_adc();

   // Display each value in binary on LEDs (PORTB)
   output_b(value0);       // Pot A ? LEDs
   delay_ms(3000);         // Show for 3 seconds
   output_b(0x00);         // Clear LEDs

   output_b(value1);       // Pot B ? LEDs
   delay_ms(3000);
   output_b(0x00);

   output_b(value2);       // Pot C ? LEDs
   delay_ms(3000);
   output_b(0x00);
}


/* ============================================================
   Function: rotates_motor()
   Purpose:  Toggles motor state each time Button 2 (RA5) is pressed.
             Cycle: Clockwise ? Stop ? Anti-clockwise ? Stop ? repeat
   ============================================================ */
void rotates_motor(void)
{
   counter += 1;   // Increment state each button press

   switch(counter)
   {
      case 1:  // Clockwise rotation
         output_low(PIN_A6);
         output_high(PIN_A7);
         break;

      case 2:  // Stop motor
         output_low(PIN_A6);
         output_low(PIN_A7);
         break;

      case 3:  // Anti-clockwise rotation
         output_high(PIN_A6);
         output_low(PIN_A7);
         break;

      case 4:  // Stop again + reset counter
         output_low(PIN_A6);
         output_low(PIN_A7);
         counter = 0;
         break;

      default:
         break;
   }
}
