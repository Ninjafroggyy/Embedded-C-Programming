/* =============================================================
   File:        PIC18F45K50_Timer2_MultiClock_LED_Driver.c
   Target MCU:  PIC18F45K50
   Description:
      This program demonstrates the use of Timer2 interrupts 
      to create multiple independent software clocks.
      Each clock channel toggles a corresponding LED on PORTC
      at a different rate based on its reset value.

   Functional Overview:
      - Timer2 generates periodic interrupts (~8ms).
      - Eight virtual "soft clocks" are maintained in an array.
      - Each clock increments until it reaches its reset value,
        then toggles its corresponding LED (C0–C7).
      - The result: eight LEDs blink at different frequencies.

   Hardware Setup:
      -----------------------------------------------------------
      | Component | MCU Pin(s) | Description                   |
      |------------|------------|-------------------------------|
      | LED0–LED7  | RC0–RC7    | Each LED shows one soft clock |
      | Pushbutton | RA0        | (Optional, not used here)     |
      -----------------------------------------------------------

   Verified in Proteus using PIC18F45K50.
   ============================================================= */

#include <main.h>

/* ---------------- Function Prototypes ---------------- */
void up_counter(unsigned int ident);

/* ---------------- Data Structure Definition ---------------- */
/* Each 'soft_clock' stores:
   - count: current timer count
   - reset_val: overflow threshold
   - flag: indicates when to toggle LED
*/
struct basic_struct
{
   unsigned long int count;
   unsigned long int reset_val;
   unsigned int1 flag;
};

/* ---------------- Global Variables ---------------- */
unsigned int counter = 0;             // General Timer2 counter
struct basic_struct soft_clock[8];    // Array for 8 software clocks


/* =============================================================
   Interrupt Service Routine: TIMER2_isr
   Purpose:
      Executes on each Timer2 overflow.
      Increments software clocks periodically.
   ============================================================= */
#INT_TIMER2
void TIMER2_isr(void)
{
   counter++;

   if (counter >= 10)  // Run every 10 timer ticks (~80ms)
   {
      counter = 0;

      // Update all 8 virtual clocks
      up_counter(0);
      up_counter(1);
      up_counter(2);
      up_counter(3);
      up_counter(4);
      up_counter(5);
      up_counter(6);
      up_counter(7);
   }
}


/* =============================================================
   Main Program
   ============================================================= */
void main()
{
   // Initialize reset values for each soft clock
   soft_clock[0].reset_val = 3;
   soft_clock[1].reset_val = 5;
   soft_clock[2].reset_val = 7;
   soft_clock[3].reset_val = 11;
   soft_clock[4].reset_val = 13;
   soft_clock[5].reset_val = 17;
   soft_clock[6].reset_val = 19;
   soft_clock[7].reset_val = 23;

   // Clear PORTC LEDs initially
   output_c(0x00);

   // Optional ADC setup (not used in this circuit)
   setup_adc_ports(sAN0 | sAN1);
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);

   // Timer2 setup:
   // Prescaler 1:16, period = 250, postscaler = 16 ? ~8ms interrupts
   setup_timer_2(T2_DIV_BY_16, 250, 16);

   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);

   while (TRUE)
   {
      // Check flags for each soft clock and toggle LEDs accordingly
      if (soft_clock[0].flag) { output_toggle(PIN_C0); soft_clock[0].flag = 0; }
      if (soft_clock[1].flag) { output_toggle(PIN_C1); soft_clock[1].flag = 0; }
      if (soft_clock[2].flag) { output_toggle(PIN_C2); soft_clock[2].flag = 0; }
      if (soft_clock[3].flag) { output_toggle(PIN_C3); soft_clock[3].flag = 0; }
      if (soft_clock[4].flag) { output_toggle(PIN_C4); soft_clock[4].flag = 0; }
      if (soft_clock[5].flag) { output_toggle(PIN_C5); soft_clock[5].flag = 0; }
      if (soft_clock[6].flag) { output_toggle(PIN_C6); soft_clock[6].flag = 0; }
      if (soft_clock[7].flag) { output_toggle(PIN_C7); soft_clock[7].flag = 0; }
   }
}


/* =============================================================
   Function: up_counter()
   Purpose:
      Increments each software counter.
      When a counter reaches its reset value:
      - It resets to 0
      - Sets its flag to trigger an LED toggle
   ============================================================= */
void up_counter(unsigned int ident)
{
   soft_clock[ident].count++;

   if (soft_clock[ident].count >= soft_clock[ident].reset_val)
   {
      soft_clock[ident].count = 0;
      soft_clock[ident].flag = 1;
   }
}
