#include <main.h>

/* ================= LCD wiring (4-bit on PORTD) ================= */
#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN     PIN_D0
#define LCD_RW_PIN     PIN_D1
#define LCD_DATA4      PIN_D4
#define LCD_DATA5      PIN_D5
#define LCD_DATA6      PIN_D6
#define LCD_DATA7      PIN_D7
#include <lcd.c>

/* ========================= Globals ============================ */
unsigned int8  buttons        = 0;   // current button mask
unsigned int8  prev_buttons   = 0;   // previous button mask

unsigned int8  adc0, adc1, adc2;     // AN0, AN1, AN2 latest readings

/* ===================== Prototypes ============================= */
unsigned int8  read_adc_channel(unsigned int8 chan);
unsigned int8  read_buttons_mask(void);
void           reset_outputs(void);

void           mode_buzzer_on(void);           // BUT0
void           mode_knight_rider(void);        // BUT1
void           mode_show_adc(void);            // BUT2
void           mode_dual_bicolor_flash(void);  // BUT3
void           mode_motor_cw(void);            // BUT0 + BUT1
void           mode_motor_ccw(void);           // BUT2 + BUT3

unsigned int32 sum_three(unsigned int8 A, unsigned int8 B, unsigned int8 C);

/* ============================ MAIN ============================ */
void main(void)
{
    setup_adc_ports(sAN0 | sAN1 | sAN2);
    setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);

    lcd_init();
    printf(lcd_putc, "\f");

    while (TRUE)
    {
        // Read buttons and clear everything when the combination changes
        buttons = read_buttons_mask();
        if (buttons != prev_buttons) {
            reset_outputs();
        }

        // Dispatch by combination
        switch (buttons)
        {
            case 0x01:  mode_buzzer_on();          break;  // BUT0
            case 0x02:  mode_knight_rider();       break;  // BUT1
            case 0x04:  mode_show_adc();           break;  // BUT2
            case 0x08:  mode_dual_bicolor_flash(); break;  // BUT3
            case 0x03:  mode_motor_cw();           break;  // BUT0 + BUT1
            case 0x0C:  mode_motor_ccw();          break;  // BUT2 + BUT3
            default: /* idle */                    break;
        }

        prev_buttons = buttons;
    }
}

/* ========================= Modes ============================== */

// BUT0: buzzer on (A6=1, A7=0) + LCD message
void mode_buzzer_on(void)
{
    output_high(PIN_A6);
    output_low (PIN_A7);

    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fSounder ON");
    }
}

// BUT1: LEDs RB0..RB7 run knight-rider sweep once
void mode_knight_rider(void)
{
    static const unsigned int8 kr[15] =
        {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x00};

    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fKnight Rider");
    }

    for (unsigned int8 i = 0; i <= 13; i++) {
        output_b(kr[i]);            // show step
        delay_ms(100);
    }
}

// BUT2: show ADC values on LCD and their sum on line 3/4 area
void mode_show_adc(void)
{
    adc0 = read_adc_channel(0);
    adc1 = read_adc_channel(1);
    adc2 = read_adc_channel(2);

    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fADC values");
    }

    lcd_gotoxy(1, 2);
    printf(lcd_putc, "  %3u   %3u   %3u", adc0, adc1, adc2);

    lcd_gotoxy(21, 1);  // right side (20x4)
    printf(lcd_putc, "sum=%5lu", sum_three(adc0, adc1, adc2));
}

// BUT3: flash two bi-colour LEDs alternately (E0/E1 and E2/E3)
void mode_dual_bicolor_flash(void)
{
    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fFlash dual colour");
        lcd_gotoxy(5, 2);
        printf(lcd_putc, "LEDs");
    }

    // LED pair 1 (E0/E1)
    output_high(PIN_E0); output_low (PIN_E1);
    delay_ms(550);
    output_high(PIN_E1); output_low (PIN_E0);
    delay_ms(550);
    output_low (PIN_E0); output_low (PIN_E1);

    // LED pair 2 (E2/E3)
    output_high(PIN_E2); output_low (PIN_E3);
    delay_ms(550);
    output_high(PIN_E3); output_low (PIN_E2);
    delay_ms(550);
    output_low (PIN_E2); output_low (PIN_E3);
}

// BUT0+BUT1: drive motor clockwise (A4=1)
void mode_motor_cw(void)
{
    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fMotor Clockwise");
    }
    output_high(PIN_A4);
}

// BUT2+BUT3: drive motor anti-clockwise (A5=1)
void mode_motor_ccw(void)
{
    if (prev_buttons != buttons) {
        printf(lcd_putc, "\fMotor Anti-clockwise");
    }
    output_high(PIN_A5);
}

/* ====================== Utilities ============================= */

// Clear outputs when mode changes
void reset_outputs(void)
{
    output_low(PIN_A6);   // buzzer A
    output_low(PIN_A7);   // buzzer B
    output_low(PIN_A4);   // motor CW
    output_low(PIN_A5);   // motor CCW
    output_b(0x00);       // LEDs off
    printf(lcd_putc, "\f");
}

// Read buttons: C0..C2 are BUT0..BUT2, D3 is BUT3
unsigned int8 read_buttons_mask(void)
{
    unsigned int8 mask = 0;
    mask  = input_c() & 0x07;      // C0..C2
    if (input(PIN_D3)) mask |= 0x08;
    return mask;                   // bits: 0..3 correspond to BUT0..BUT3
}

// Read specified ADC channel (AN0..AN2)
unsigned int8 read_adc_channel(unsigned int8 chan)
{
    set_adc_channel(chan);
    delay_us(20);
    return read_adc();             // 0..255 (CCS default 8-bit)
}

// Sum three bytes safely into 32-bit for printing
unsigned int32 sum_three(unsigned int8 A, unsigned int8 B, unsigned int8 C)
{
    return (unsigned int32)A + (unsigned int32)B + (unsigned int32)C;
}
