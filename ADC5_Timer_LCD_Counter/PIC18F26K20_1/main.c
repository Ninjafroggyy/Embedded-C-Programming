#include <main.h>

/* ======================= Globals ===========================
   - counter: increments on button press (RA6)
   - channel: current ADC channel in round-robin 0..4
   - values[5]: latest readings for AN0..AN4
   ========================================================= */
unsigned long counter = 0;
unsigned int  channel  = 0;
unsigned int  values[5] = {0};   // five channels: AN0..AN4

/* ================= Timer2 ISR ===============================
   Fires every ~5 ms (per setup below). Each interrupt:
   1) Stores the last completed ADC result for the current channel
   2) Advances to the next channel (0..4)
   3) Starts a new conversion on that next channel
   This creates a continuous, timer-driven scan of AN0..AN4.
   ========================================================= */
#INT_TIMER2
void TIMER2_isr(void)
{
    values[channel] = read_adc(ADC_READ_ONLY);  // fetch result just finished
    channel++;
    if (channel > 4) channel = 0;               // wrap 0..4
    set_adc_channel(channel);                   // select next channel
    delay_us(10);                               // acquisition time
    read_adc(ADC_START_ONLY);                   // start next conversion
}

/* ================= LCD wiring (4-bit on PORTC/D) ============ */
#define LCD_ENABLE_PIN PIN_C2
#define LCD_RS_PIN     PIN_C0
#define LCD_RW_PIN     PIN_C1
#define LCD_DATA4      PIN_C4
#define LCD_DATA5      PIN_C5
#define LCD_DATA6      PIN_C6
#define LCD_DATA7      PIN_C7
#include <lcd.c>

void main(void)
{
    /* --- ADC: enable AN0..AN4, internal clock --- */
    setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4);
    setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);

    /* --- Timer2: ~5 ms interrupt period --- 
       T2_DIV_BY_16, PR2=252, postscaler=10 ? ~5.06 ms per ISR */
    setup_timer_2(T2_DIV_BY_16, 252, 10);

    /* --- Kick off first conversion on channel 0 --- */
    set_adc_channel(0);
    delay_us(10);
    read_adc(ADC_START_ONLY);

    /* --- Interrupts on --- */
    enable_interrupts(INT_TIMER2);
    enable_interrupts(GLOBAL);

    /* --- LCD init --- */
    lcd_init();

    while (TRUE)
    {
        /* Simple edge-hold: increment when RA6 goes high, wait for release */
        if (input(PIN_A6))
        {
            while (input(PIN_A6)) { }   // wait for button release (basic debounce)
            counter++;
        }

        /* --- Display counter --- */
        lcd_gotoxy(2, 2);
        printf(lcd_putc, "counter = %4ld ", counter);

        /* --- Display ADC values (hex for compactness) --- */
        lcd_gotoxy(21, 1);
        printf(lcd_putc, "vals %x %x %x ", values[0], values[1], values[2]);

        lcd_gotoxy(21, 2);
        printf(lcd_putc, "vals %x %x   ", values[3], values[4]);

        /* --- Stream to UART (optional) --- */
        printf("values = %x %x %x %x %x\r",
               values[0], values[1], values[2], values[3], values[4]);
    }
}
