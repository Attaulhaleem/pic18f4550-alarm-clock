

/* File:   Lecture_31.c
 * Author: Memoon
 *
 * Created on May 15, 2020, 1:03 PM
 */


// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define RC1 PORTCbits.RC1
#define RC2 PORTCbits.RC2
#define up PORTCbits.RC4
#define down PORTCbits.RC5
#define on_off PORTCbits.RC6
#define buzz PORTCbits.RC7

void dasecs(void);
void damins(void);
void dahrs(void);

void dmi(void);
void dhr(void);
void time(void);
void mode_change(void);
void delay(unsigned int);

unsigned char tsecs = 0, tmins = 0, thrs = 0;           // time saved
unsigned char asecs = 0, amins = 0, ahrs = 0;           // alarm time saved
unsigned int sel = 0;                                   // selection

unsigned char secs = 0;
unsigned char secsh = 0;
unsigned char secsl = 0;
unsigned char mins = 0;
unsigned char minsh = 0;
unsigned char minsl = 0;
unsigned char hrs = 0;
unsigned char hrsh = 0;
unsigned char hrsl = 0;

void main(void)
{
    TRISA = 0;          // hrs              // OUPUTS
    TRISB = 0;          // sec
    TRISD = 0;          // min
    TRISC7 = 0;         // BUZZER
    
    TRISC1 = 1;         // Setting          // INPUTS
    TRISC2 = 1;         // Mode
    TRISC6 = 1;         // Alarm ON / OFF
    
    PORTA = hrs;
    PORTD = mins;
    PORTB = secs;
    
    while(1)
    {
        if(RC1 == 1)
        {
            mode_change();
        }
        else
        {
            time();
            delay(100);
            if(on_off == 1 && hrs == ahrs && mins == amins && secs == asecs)
            {
                buzz = 1;
                delay(5000);
                secs += 5;;       // to compensate for buzzer delay
            }
            buzz = 0;
        }
    }
}

void time(void)
{
    secs++;
    dasecs();
    if(secs >= 0x60)
    {
        secs = 0;
        mins++;
        damins();
        if(mins >= 0x60)
        {
            mins = 0;
            hrs++;
            dahrs();
            if(hrs >= 0x24)
            {
                hrs = 0;
            }
        }
    }
    PORTA = hrs;
    PORTD = mins;
    PORTB = secs;
}

void mode_change(void)
{
    tsecs = secs; tmins = mins; thrs = hrs;
    secs = 0; mins = 0; hrs = 0;
    while(RC1 == 1)     // if mode is time
    {
        PORTB = secs;
        if(RC2 == 1)
        {
            sel++;
            delay(250);
            if(sel > 4)
            {
                sel = 0;
            }
        }
        if(sel == 1)
        {
            if(up == 1)        // min hand
            {
                if(mins >= 0x60)
                {
                    mins = 0;
                }
                else
                {
                    delay(250);
                    mins++;
                    damins();
                    PORTD = mins;
                }
                amins = mins;
            }
            else if(down == 1)
            {
                if(mins <= 0x00)
                {
                    mins = 0x60;
                }
                else
                {
                    delay(250);
                    mins--;
                    dmi();
                    PORTD = mins;
                }
                amins = mins;
            }
        }
        else if(sel == 2)
        {
            if(up == 1)        // hrs hand
            {
                if(hrs == 0x24)
                {
                    hrs = 0;
                }
                else
                {
                    delay(250);
                    hrs++;
                    dahrs();
                    PORTA = hrs;
                }
                ahrs = hrs;
            }
            else if(down == 1)
            {
                if(hrs <= 0x00)
                    hrs = 0x24;
                else
                    delay(250);
                    hrs--;
                    dhr();
                    PORTA = hrs;
                ahrs = hrs;
            }
        }
        if(sel == 3)
        {
            if(up == 1)        // min hand
            {
                if(mins >= 0x60)
                {
                    mins = 0;
                }
                else
                {
                    delay(250);
                    mins++;
                    damins();
                    PORTD = mins;
                }
                tmins = mins;
            }
            else if(down == 1)
            {
                if(mins <= 0x00)
                {
                    mins = 0x60;
                }
                else
                {
                    delay(250);
                    mins--;
                    dmi();
                    PORTD = mins;
                }
                tmins = mins;
            }
        }
        else if(sel == 4)
        {
            if(up == 1)        // hrs hand
            {
                if(hrs == 0x24)
                {
                    hrs = 0;
                }
                else
                {
                    delay(250);
                    hrs++;
                    dahrs();
                    PORTA = hrs;
                }
                thrs = hrs;
            }
            else if(down == 1)
            {
                if(hrs <= 0x00)
                    hrs = 0x24;
                else
                    delay(250);
                    hrs--;
                    dhr();
                    PORTA = hrs;
                thrs = hrs;
            }
        }
    }
    secs = tsecs; mins = tmins; hrs = thrs;
    PORTA = hrs; PORTD = mins; PORTB = secs;
}

void dasecs(void)
{
    secsl = secs;
    secsh = secs;
    secsl = secsl & 0x0F;
    secsh = secsh & 0xF0;
    if(secsl > 0x09)
        secs = secs + 0x06;
    if(secsh > 0x90)
        secs = secs + 0x60;
}

void damins(void)
{
    minsl = mins;
    minsh = mins;
    minsl = minsl & 0x0F;
    minsh = minsh & 0xF0;
    if(minsl > 0x09)
        mins = mins + 0x06;
    if(minsh > 0x90)
        mins = mins + 0x60;
}

void dmi(void)
{
    minsl = mins;
    minsh = mins;
    minsl = minsl & 0x0F;
    minsh = minsh & 0xF0;
    if(minsl <= 0x00)
        mins = 0x09;
}

void dahrs(void)
{
    hrsl = hrs;
    hrsh = hrs;
    hrsl = hrsl & 0x0F;
    hrsh = hrsh & 0xF0;
    if(hrsl > 0x09)
        hrs = hrs + 0x06;
    if(hrsh > 0x90)
        hrs = hrs + 0x60;
}

void dhr(void)
{
    hrsl = hrs;
    hrsh = hrs;
    hrsl = hrsl & 0x0F;
    hrsh = hrsh & 0xF0;
    if(hrsl > 0x09)
        hrs = 0x09;
}

void delay(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 110; j++);
    }
}