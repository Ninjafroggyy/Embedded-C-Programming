#include <main.h>

/* ================= LCD wiring (20x4, 4-bit) ================= */
#define LCD_ENABLE_PIN PIN_C2
#define LCD_RS_PIN     PIN_C0
#define LCD_RW_PIN     PIN_C1
#define LCD_DATA4      PIN_C4
#define LCD_DATA5      PIN_C5
#define LCD_DATA6      PIN_C6
#define LCD_DATA7      PIN_C7
#include <lcd.c>

/* ======================= Globals ============================
   (Removed unused flags/arrays from earlier versions)
   ============================================================ */

/* ---------- Prototypes ---------- */
unsigned int8  read_pot_value(void);                  // AN0 ? 0..255
unsigned int32 compute_scaled_product(
                    unsigned int8 A,
                    unsigned int8 B,
                    unsigned int8 C);                 // (A*B)*C (32-bit)

/* ======================== MAIN ============================= */
void main(void)
{
    setup_adc_ports(sAN0);                             // Potentiometer on AN0
    setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);

    lcd_init();
    printf(lcd_putc, "\f");                            // Clear LCD once

    while(TRUE)
    {
        unsigned int8  pot = read_pot_value();
        unsigned int32 result = compute_scaled_product(16, 15, pot);

        lcd_gotoxy(2, 1);
        printf(lcd_putc, "  result = %5lu", result);

        lcd_gotoxy(21, 1);
        printf(lcd_putc, "values 16 15 %3u", pot);

        delay_ms(1000);
    }
}

/* ===================== Helpers ============================= */

/* Read potentiometer on AN0 (8-bit result by CCS default) */
unsigned int8 read_pot_value(void)
{
    set_adc_channel(0);
    delay_us(20);                                      // acquisition time
    return read_adc();                                  // 0..255
}

/* Multiply three bytes safely into 32-bit to avoid overflow
   (CCS 'int' is 8-bit; promote before multiply). */
unsigned int32 compute_scaled_product(unsigned int8 A,
                                      unsigned int8 B,
                                      unsigned int8 C)
{
    unsigned int32 acc = (unsigned int32)A * (unsigned int32)B;
    acc *= (unsigned int32)C;
    return acc;
}
