//  EE 475 Lab 2 localNode.c

//  Justin Allmaras, Bryan Bednarski, Nick Lopez



// Configuration necessary for local node:

// enable RS232
// enable I2C
// enable ADC input
// enable internal oscillator
// disable watchdog timer

#include "localNode.h"

#define _XTAL_FREQ          16000000



void setupPins();

int main () {
    setupPins();
    char data;
    while(1) {
        LATB = 1;
        char buffer [40] = {0};
        getUARTData(buffer , 40);
        data = UART_Read();
        __delay_ms(10);
        LATB0 = 0;        
        sendUARTData(buffer);
        __delay_ms(10);
        LATB0 = 1;
    }
    return 0;
}


void setupPins() {
    TRISB0 = 0;

    OSCCON = 0b01110000;
    // setup registers for UART on pins RC6 and RC7
    unsigned int x;
    x = ((16000000 - (9600 * 64)) / (9600 * 64));
    SPBRG1 = 25;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TXEN = 1;
    RCSTA1bits.CREN = 1;
    RCSTA1bits.SPEN = 1;
}



void getUARTData(char * serialData , unsigned int length) {
    unsigned int i;
    for(i = 0; i < length; i++)
    serialData[i] = UART_Read();
}

void readADC(int * ADCData) {

}

void sendUARTData(const char * serialData) {
    int i;
    for(i = 0; serialData[i] != '\0'; i++) {
        UART_Write(serialData[i]);
    }
}

char UART_Read() {
    while(PIR1bits.RC1IF == 0);
    return RCREG1;
}

void UART_Write(char data) {
    while(!(TX1STAbits.TRMT1));
    TXREG1 = data;
}

