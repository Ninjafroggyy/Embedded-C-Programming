#define LCD_ENABLE_PIN PIN_C2
#define LCD_RS_PIN PIN_C0
#define LCD_RW_PIN PIN_C1
#define LCD_DATA4 PIN_C3
#define LCD_DATA5 PIN_C4
#define LCD_DATA6 PIN_C5
#define LCD_DATA7 PIN_A5

#include <main.h>
#include <lcd.c>

unsigned int value = 0;
unsigned int value1 = 0;
unsigned int val = 0;
unsigned int val1 = 0;

void main() {
   setup_adc_ports(sAN0, sAN1);
   setup_adc(ADC_CLOCK_INTERNAL);

   lcd_init();

   while(TRUE) {
      set_adc_channel(0);		// Select the ADC channel
      delay_us(30);			// Delay 30 uS
      value = read_adc();		// Call the ADC conversion function
      
      set_adc_channel(1);		// Select the ADC channel
      delay_us(30);			// Delay 30 uS
      value1 = read_adc();		// Call the ADC conversion function
      
      lcd_gotoxy(1, 1);			// Set the cursor at position 1,1 in the LCD
      printf(lcd_putc, "Hello %u    ", value);	
	 
      lcd_gotoxy(1, 2);		
      printf(lcd_putc, "Hello %u    ", value1);	

      
      if(input(Pin_a3)) {
	 printf(lcd_putc, "\f");
	 lcd_gotoxy(26, 2);
	 printf(lcd_putc, "This is %u  ", val);
	 val++;
      }
      
      if(input(Pin_a4)) {
	 printf(lcd_putc, "\f");
	 lcd_gotoxy(26, 1);
	 printf(lcd_putc, "This is %u  ", val1);
	 val1++;
      }
   }
}
