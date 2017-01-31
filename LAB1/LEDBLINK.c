/* 
 * File:   main.c
 * Author: ee475 Bryan Bednarski, Justin Allmaras, Nick Lopez
 *
 * Created on January 11, 2017, 4:59 PM
 */

// CONFIG1H
#pragma config FOSC = INTIO7    // Oscillator Selection bits (Internal oscillator block, CLKOUT function on OSC2)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTB5   // ECCP2 B output mux bit (P2B is on RB5)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//TODO:  Fix oscillator freq
#define _XTAL_FREQ 8000000

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>



// function prototypes
void setupGPIOs();
int getState();
void countClock();
void resetCounter();
void setReadWriteControl(int state);
void setOutputEnableControl(int state);
void setBufferEnable(int state);
void writeData(int data);
void readData();

int main(int argc, char** argv) {
    setupGPIOs();
    // reset counter
    // 
    
    resetCounter();
    int state = 0;
    int previousState = 0;
    int data = 0;
    while (1) {
        // get mode
        state = getState();
        
        switch (state) {
            //read mode
            case 0:
                countClock();
                if(state != previousState) {
                    resetCounter();
                    setBufferEnable(0);
                    previousState = state;
                }
                readData();
            break;
            // write mode
            case 1:
                if(state != previousState) {
                    resetCounter();
                    setBufferEnable(1);
                    data = 0;
                    previousState = state;
                }
                writeData(data);
                countClock();
                data++;
            break;
            // address mode
            case 2:
                if(state != previousState) {
                    resetCounter();
                    setBufferEnable(0);
                    previousState = state;
                }
                countClock();
            break;
            // idle mode
            case 3:
                if(state != previousState) {
                    resetCounter();
                    //setReadWriteControl(1);
                    //setOutputEnableControl(1);
                    setBufferEnable(0);
                    previousState = state;
                }
                __delay_ms(25);
            break;
        }
        __delay_ms(90);

    }
        
        
    return (EXIT_SUCCESS);
}


void setupGPIOs() {
    // set I/O GPIOs
    TRISA &= 0xC0;
    TRISB &= 0xF8;
    TRISB |= 0x18;
    TRISC &= 0x38;
    __delay_ms(90);
    // setup initial values for GPIOS
    LATA &= 0xC0;
    LATA |= 0x1F;
    __delay_ms(90);
    LATB &= 0xFB;
    LATB |= 0x03;
    LATC |= 0xC7;

}


// state 11 both off (idle)
// state 10 Switch 3 on , switch 2 off (address))
// state 01 Switch 3 off , switch 2 on  (write))
// state 00 both on (read)

int getState() {
    return (PORTBbits.RB4 << 1) + PORTBbits.RB3;
}

void setBufferEnable(int state) {
    LATC = LATC & ~(1 << 2) | (state << 2);
}

void resetCounter() {
    LATB |= (1 << 1);
    __delay_ms(2);
    LATB &= ~(1 << 1);
    __delay_ms(2);
}

void setReadWriteControl(int state) {
    LATC = LATC & ~(1 << 7) | (state << 7);
}

void setOutputEnableControl(int state) {
    LATC = LATC & ~(1 << 6) | (state << 6);
}

void countClock() {
    LATB |= (1 << 2);
    __delay_ms(2);
    LATB &= ~(1 << 2);
    __delay_ms(2);
}

void writeData(int data) {
    int dataLatA = (data << 2) >> 2;
    int dataLatC = data >> 6;
    LATA = (LATA & ~(0x3F)) | dataLatA;
    LATC = (LATC & ~(0x03)) | dataLatC;
    __delay_ms(2);
    setReadWriteControl(0);
    __delay_ms(2);
    setReadWriteControl(1);
    __delay_ms(2);
}

void readData() {
    setOutputEnableControl(0);
    __delay_ms(30);
    setOutputEnableControl(1);
    __delay_ms(2);
}
