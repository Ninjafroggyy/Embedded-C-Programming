/* =============================================================
   File:        PIC16F877A_Analog_LED_LCD_Motor_Controller.c
   Target MCU:  PIC16F877A
   Description:
      Multifunction embedded C program integrating ADC input,
      motor control, LED display, and LCD/terminal feedback.

      Functional overview:
      - Reads analogue input (RV1) using ADC channel AN0.
      - Displays ADC values and motor states on an LCD.
      - Controls LEDs and motor based on switch settings (SW1).
      - Sends real-time status messages to the virtual terminal.

      System Modes (SW1 selector):
      -----------------------------------------------------------
      | SW1-A | Run motor direction based on potentiometer (ADC) |
      | SW1-B | Display ADC value on LEDs in binary              |
      | SW1-C | Motor rotates while button (A1) is pressed       |
      | SW1-D | Knight Rider LED light sequence ("Kitt mode")    |
      -----------------------------------------------------------
   ============================================================= */

#include <main.h>

/* ---------------- LCD Pin Configuration ---------------- */
#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN     PIN_D0
#define LCD_RW_PIN     PIN_D1
#define LCD_DATA4      PIN_D4
#define LCD_DATA5      PIN_D5
#define LCD_DATA6      PIN_D6
#define LCD_DATA7      PIN_D7
#include <lcd.c>

/* ---------------- Constants ---------------- */
#define delay 200    // LED delay time for Knight Rider (ms)

/* ---------------- Function Prototypes ---------------- */
void analog(void);           // Reads the ADC input
void run_motor(void);        // Runs motor based on ADC input
void lcd_lights(void);       // Displays ADC binary output on LEDs
void button_press(void);     // Runs motor while button pressed
void button_off(void);       // Handles "button not pressed" message
void button_on(void);        // Handles "button pressed" message
void kitt_text(void);        // Prints "Kitt mode" once to terminal
void kitt_mode(void);        // Knight Rider LED light animation

/* ---------------- Global Variables ---------------- */
unsigned int adc, remainder, new_adc;
unsigned int knightrider[14] = 
   {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02};
static int off = 0, on = 0;   // Track button state changes for terminal output


/* =============================================================
   Main Program
   ============================================================= */
void main()
{
   setup_adc_ports(sAN0);                            // Initialise ADC on AN0
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);    // Set ADC timing mode
   lcd_init();                                       // Initialise LCD

   while(TRUE)
   {
      analog();   // Continuously read analogue value from RV1
      
      // Mode 1: Potentiometer controls motor speed/direction
      if (input(PIN_A2))
         run_motor();
      
      // Mode 2: Display ADC output as binary pattern on LEDs
      if (input(PIN_A3))
         lcd_lights();
      
      // Mode 3: Motor rotates while button (A1) is pressed
      if (input(PIN_A4))
         button_press();
      
      // Mode 4: Knight Rider light sequence ("Kitt mode")
      if (input(PIN_A5))
      {
         kitt_text();
         kitt_mode();
      }
   }
}


/* =============================================================
   Function: analog()
   Purpose:  Reads analogue value from RV1 (AN0)
   ============================================================= */
void analog(void)
{
   set_adc_channel(0);     // Read ADC channel AN0
   delay_us(30);           // Allow ADC to settle
   adc = read_adc();       // Store result in global variable
}


/* =============================================================
   Function: run_motor()
   Purpose:  Controls motor direction/speed based on ADC reading.
             Displays ADC and motor state on LCD.
   ============================================================= */
void run_motor(void)
{
   if (adc < 75)  // ADC < 75 ? Anti-clockwise
   {
      output_low(PIN_C0);
      output_high(PIN_C1);
      lcd_gotoxy(4,2);
      printf(lcd_putc, "Anti-clockwise");
   }
   else if ((adc > 76) && (adc < 174))  // Mid-range ? Stop motor
   {
      output_low(PIN_C0);
      output_low(PIN_C1);
      lcd_gotoxy(4,2);
      printf(lcd_putc, "Motor Stopped ");
   }
   else if (adc > 175)  // ADC > 175 ? Clockwise
   {
      output_high(PIN_C0);
      output_low(PIN_C1);
      lcd_gotoxy(4,2);
      printf(lcd_putc, "  Clockwise   ");
   }

   // Display ADC reading on LCD
   lcd_gotoxy(6,1);
   printf(lcd_putc, "adc = %3u", adc);

   // Clear unused LCD lines
   lcd_gotoxy(22,1); printf(lcd_putc, "                  ");
   lcd_gotoxy(26,2); printf(lcd_putc, "         ");
}


/* =============================================================
   Function: lcd_lights()
   Purpose:  Displays ADC value as an 8-bit binary pattern 
             on LEDs RB0–RB7. Also prints ADC to LCD & terminal.
   ============================================================= */
void lcd_lights(void)
{
   lcd_gotoxy(6,1);
   printf(lcd_putc, "adc = %3u", adc);
   lcd_gotoxy(4,2);  printf(lcd_putc, "              ");
   lcd_gotoxy(22,1); printf(lcd_putc, "                  ");
   lcd_gotoxy(26,2); printf(lcd_putc, "         ");
   printf("adc = %3u \n\r", adc);   // Print ADC to terminal

   // Display ADC value on LEDs by converting to binary
   unsigned int temp = adc;
   for (int i = 0; i < 8; i++)
   {
      if (temp % 2)  output_high(PIN_B0 + i);  // Turn ON LED if bit = 1
      else            output_low(PIN_B0 + i);  // Turn OFF LED if bit = 0
      temp /= 2;  // Shift right
   }
}


/* =============================================================
   Function: button_press()
   Purpose:  Motor runs clockwise while button (A1) is pressed.
             Stops when released. Updates LCD and terminal.
   ============================================================= */
void button_press(void)
{
   if (input(PIN_A1))  // Button pressed
   {
      output_high(PIN_C0);
      output_low(PIN_C1);
      lcd_gotoxy(22,1);
      printf(lcd_putc, "  Button pressed  ");
      button_on();
   }
   else  // Button released
   {
      output_low(PIN_C0);
      output_low(PIN_C1);
      lcd_gotoxy(22,1);
      printf(lcd_putc, "Button not pressed");
      button_off();
   }

   // Clear other LCD lines
   lcd_gotoxy(6,1);  printf(lcd_putc, "         ");
   lcd_gotoxy(4,2);  printf(lcd_putc, "              ");
   lcd_gotoxy(26,2); printf(lcd_putc, "         ");
}


/* =============================================================
   Function: button_off()
   Purpose:  Prints "Button not pressed" to terminal once.
   ============================================================= */
void button_off(void)
{
   if (!off)
   {
      printf("Button not pressed \n\r");
      off = 1;
      on = 0;
   }
}


/* =============================================================
   Function: button_on()
   Purpose:  Prints "Button pressed" to terminal once.
   ============================================================= */
void button_on(void)
{
   if (!on)
   {
      printf("Button pressed \n\r");
      on = 1;
      off = 0;
   }
}


/* =============================================================
   Function: kitt_text()
   Purpose:  Prints "Kitt mode" to terminal once when SW1-D is selected.
   ============================================================= */
void kitt_text(void)
{
   static int kitt;
   if (!kitt)
   {
      printf("Kitt mode \n\r");
      kitt = 1;
   }
}


/* =============================================================
   Function: kitt_mode()
   Purpose:  Creates Knight Rider LED light animation on PORTB.
   ============================================================= */
void kitt_mode(void)
{
   // Clear LCD and display "Kitt mode"
   lcd_gotoxy(6,1);  printf(lcd_putc, "         ");
   lcd_gotoxy(4,2);  printf(lcd_putc, "              ");
   lcd_gotoxy(22,1); printf(lcd_putc, "                  ");
   lcd_gotoxy(26,2); printf(lcd_putc, "Kitt mode");

   // Ensure all LEDs off before animation
   output_b(0x00);

   // Run LED sweep pattern
   for (int q = 0; q < 14; q++)
   {
      switch(knightrider[q])
      {
         case 0x01: output_high(PIN_B7); output_low(PIN_B6); delay_ms(delay); break;
         case 0x02: output_low(PIN_B7); output_high(PIN_B6); output_low(PIN_B5); delay_ms(delay); break;
         case 0x03: output_low(PIN_B6); output_high(PIN_B5); output_low(PIN_B4); delay_ms(delay); break;
         case 0x04: output_low(PIN_B5); output_high(PIN_B4); output_low(PIN_B3); delay_ms(delay); break;
         case 0x05: output_low(PIN_B4); output_high(PIN_B3); output_low(PIN_B2); delay_ms(delay); break;
         case 0x06: output_low(PIN_B3); output_high(PIN_B2); output_low(PIN_B1); delay_ms(delay); break;
         case 0x07: output_low(PIN_B2); output_high(PIN_B1); output_low(PIN_B0); delay_ms(delay); break;
         case 0x08: output_low(PIN_B1); output_high(PIN_B0); delay_ms(delay); break;
         default: break;
      }
   }
}
