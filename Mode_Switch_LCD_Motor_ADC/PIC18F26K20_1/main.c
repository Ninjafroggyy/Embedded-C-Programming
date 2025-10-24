#include <main.h>

/* ================= LCD Wiring (20x4, 4-bit mode) ================= */
#define LCD_ENABLE_PIN PIN_C2
#define LCD_RS_PIN     PIN_C0
#define LCD_RW_PIN     PIN_C1
#define LCD_DATA4      PIN_C4
#define LCD_DATA5      PIN_C5
#define LCD_DATA6      PIN_C6
#define LCD_DATA7      PIN_C7
#include <lcd.c>

/* ======================= Globals ================================
   switches     : stores masked switch states from PORTA
   mode_flags   : track which mode is currently active to avoid reprinting
   led_pattern  : LED sequence for Knight Rider effect on PORTB
   ================================================================ */
unsigned int switches;
unsigned int mode_led_display = 0, mode_motor_ccw = 0, mode_motor_cw = 0, mode_knight = 0;
unsigned int led_pattern[8] = {0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00};

/* ===================== Function Prototypes ====================== */
void display_pot_value(void);          // Mode 1: Show ADC value on LCD
void motor_anticlockwise_mode(void);   // Mode 2: Drive motor anti-clockwise
void motor_clockwise_mode(void);       // Mode 3: Drive motor clockwise
void knight_rider_mode(void);          // Mode 4: LED sweep controlled by pot
unsigned int read_pot_value(void);     // Helper: Read potentiometer value

/* ============================ MAIN ============================== */
void main()
{
    setup_adc_ports(sAN0);                        // Potentiometer on AN0
    setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_0);
    lcd_init();
    output_b(0x00);                               // Clear PORTB (LEDs/motor)

    while(TRUE)
    {
        /* --- Read and decode switches on PORTA (RA1–RA4) --- */
        switches = input_a();
        switches = (switches & 0x1E) / 2;         // Mask bits 1–4 and normalize

        switch (switches)
        {
            case 1:  display_pot_value();      break;   // Mode 1
            case 2:  motor_anticlockwise_mode(); break; // Mode 2
            case 4:  motor_clockwise_mode();   break;   // Mode 3
            case 8:  knight_rider_mode();      break;   // Mode 4
            default: break;
        }
    }
}

/* ========================= MODE 4 ===============================
   Knight Rider LED sweep on PORTB.
   Sweep speed is controlled by potentiometer input.
   Displays “Knight Rider” on LCD when active.
   =============================================================== */
void knight_rider_mode(void)
{
    unsigned int delay_value, step;
    output_b(0xF0);                               // Light all LEDs briefly

    if (!mode_knight)
    {
        printf(lcd_putc, "\f");
        lcd_gotoxy(21, 2);
        printf(lcd_putc, "Knight Rider");
        mode_led_display = 0;
        mode_motor_ccw = 0;
        mode_motor_cw = 0;
        mode_knight = 1;
    }

    for (step = 0; step < 8; step++)
    {
        delay_value = read_pot_value();           // Variable speed from pot
        output_b(led_pattern[step]);
        delay_ms(delay_value);
    }
}

/* ========================= MODE 3 ===============================
   Drive motor clockwise.
   Display message on LCD and activate LED3.
   =============================================================== */
void motor_clockwise_mode(void)
{
    output_b(0x40);                               // Turn on LED3 (RB6)

    if (!mode_motor_cw)
    {
        printf(lcd_putc, "\f");
        lcd_gotoxy(21, 1);
        printf(lcd_putc, "Motor Clockwise");
        mode_led_display = 0;
        mode_motor_ccw = 0;
        mode_motor_cw = 1;
        mode_knight = 0;
    }

    output_low(PIN_B1);                           // Motor direction: CW
    output_high(PIN_B0);
}

/* ========================= MODE 2 ===============================
   Drive motor anti-clockwise.
   Display message on LCD and activate LED2.
   =============================================================== */
void motor_anticlockwise_mode(void)
{
    output_b(0x20);                               // Turn on LED2 (RB5)

    if (!mode_motor_ccw)
    {
        printf(lcd_putc, "\f");
        lcd_gotoxy(1, 2);
        printf(lcd_putc, "Motor Anti-Clockwise");
        mode_led_display = 0;
        mode_motor_ccw = 1;
        mode_motor_cw = 0;
        mode_knight = 0;
    }

    output_low(PIN_B0);                           // Motor direction: CCW
    output_high(PIN_B1);
}

/* ========================= MODE 1 ===============================
   Display current ADC (potentiometer) reading on LCD line 1.
   Turns on LED1 to indicate active mode.
   =============================================================== */
void display_pot_value(void)
{
    output_b(0x10);                               // Turn on LED1 (RB4)

    if (!mode_led_display)
    {
        printf(lcd_putc, "\fPot Value = ");
        mode_led_display = 1;
        mode_motor_ccw = 0;
        mode_motor_cw = 0;
        mode_knight = 0;
    }

    lcd_gotoxy(13, 1);
    printf(lcd_putc, "%u  ", read_pot_value());
}

/* ======================== ADC HELPER ============================
   Reads potentiometer voltage via AN0 and returns value (0–255).
   =============================================================== */
unsigned int read_pot_value(void)
{
    unsigned int value;
    set_adc_channel(0);
    delay_us(20);
    value = read_adc();                            // 8-bit ADC default in CCS
    return value;
}
