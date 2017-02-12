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

unsigned char I2C_Master_Receive();
void I2C_Master_Write(const char data);

void setupPins();
void writeMemory(int address , char data) {
    TRISA &= ~(0xBF);
    TRISB0 = 0;
    LATA = data;
    if((data > 63 && data < 128) || data > 191) {
        LATB0 = 1;
    } else  {
        LATB0 = 0;
    }
    LATC2 = address & 1;
    LATC1 = address & 2;
    LATC0 = address & 4;
    __delay_ms(10);
    LATB4 = 0;
    __delay_ms(2);
    LATB4 = 1;
    __delay_ms(2);
    TRISA |= 0xBF;
    TRISB0 = 1;
}

char readMemory(int address) {
    TRISA |= 0xBF;
    TRISB0 = 1;
    LATC2 = address & 1;
    LATC1 = address & 2;
    LATC0 = address & 4;
    __delay_ms(10);
    LATB3 = 0;
    __delay_ms(2);
    char data = 0;
    data |= PORTAbits.RA0;
    data |= PORTAbits.RA1 << 1;
    data |= PORTAbits.RA2 << 2;
    data |= PORTAbits.RA3 << 3;
    data |= PORTAbits.RA4 << 4;
    data |= PORTAbits.RA5 << 5;
    data |= PORTAbits.RA7 << 7;
    data |= PORTBbits.RB0 << 6;
    __delay_ms(2);
    LATB3 = 1;
    __delay_ms(2);
    return data;
}


int main () {
    // RS232 setup
    setupPins();
    unsigned int setPoint = 0;
    // i2c setup
    I2C_Master_Init(100000);
    short returnVal = 0;
    unsigned int error = 0;
    while(1) {
        // RS 232 receive and sends data from CPU
        char buffer [3] = {0};
        char buffer2[3] = {0};
        buffer[0] = UART_Read();
        buffer[1] = UART_Read();
        writeMemory(0 , buffer[0]);
        __delay_ms(10);
        writeMemory(1 , buffer[1]);
        __delay_ms(10);
        switch(buffer[0]){
            case 'X':
                setPoint = 100;
                break;
            case 'Q':
                setPoint = 0;
                break;
            case 'I':
                setPoint++;
                break;
            case 'D':
                setPoint--;
                break;
            case 'S':
                setPoint = buffer[1];
                break;
        }
        // motor speed
        // error value magnitude

        
        
        if(buffer[0] != 'U') {
            I2C_Master_Write(buffer[0]);
            __delay_ms(4);
            I2C_Master_Write(buffer[1]);
            __delay_ms(4);
        } else {
            returnVal = I2C_Master_Receive();
            __delay_ms(10);
            //sendUARTData(buffer2);
            UART_Write(returnVal);
            UART_Write(' ');
        }
        __delay_ms(10);
    }
    

    return 0;
}


void setupPins() {
    OSCCON = 0b01110000;
    // setup registers for UART on pins RC6 and RC7
    unsigned int x;
    //x = ((16000000 - (9600 * 64)) / (9600 * 64));
    SPBRG1 = 25;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TXEN = 1;
    RCSTA1bits.CREN = 1;
    RCSTA1bits.SPEN = 1;
    ANSELA = 0x00;
    TRISA |= 0xBF;
    ANSELC = 0x00;
    ANSELB = 0x00;
    TRISB4 = 0;
    TRISB3 = 0;
    TRISB0 = 1;
    LATB4 = 1;
    LATB3 = 1;
    TRISC2 = 0;
    TRISC1 = 0;
    TRISC0 = 0;
}

void getUARTData(char * serialData , unsigned int length) {
    unsigned int i;
    for(i = 0; i < length; i++)
        serialData[i] = UART_Read();
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

// configuring I2C port to MSSP1
void I2C_Master_Init(const unsigned long c) {
    SSP1CON1 = 0b00101000;           // SSP1 control register 1
    SSP1CON2 = 0;                    // SSP1 control register 2
    SSP1ADD = (_XTAL_FREQ/(4*c))-1;   // 7-bit baud rate clock divider bits
    SSP1STAT = 0;                     // SSP1 status register
    TRISCbits.TRISC3 = 1;            // Setting as input given in datasheet
    TRISCbits.TRISC4 = 1;            // Setting as input given in datasheet
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
}

unsigned short I2C_Master_Read(unsigned short a) {
    unsigned char data;
    I2C_Master_Wait();
    SSP1CON2bits.RCEN = 1; // set RCEN (enables receive mode)
    I2C_Master_Wait();
    SSP1CON2bits.RCEN = 1;
    data = SSP1BUF;
    I2C_Master_Wait();
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
    return data;
}


unsigned char I2C_Master_Receive() {
    unsigned char data;
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN == 1);
    SSP1BUF = 0x31;
    while(SSP1IF == 0);
    SSP1IF = 0;
    while(SSP1IF == 0);
    SSP1IF = 0;
    SSP1CON2bits.RCEN = 1;
    while(!(SSP1IF == 1 && SSP1STATbits.BF == 1));
    SSP1IF = 0;
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1IF == 0);
    SSP1IF = 0;
    data = SSP1BUF;
    SSP1IF = 0;
    SSP1CON2bits.PEN = 1;
    return data;
}

void I2C_Master_Write(const char data) {
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN == 1);
    SSP1BUF = 0x30;
    //while(SSP1IF == 0);
    SSP1IF = 0;
    while(SSP1IF == 0);
    SSP1BUF = data;
    SSP1IF = 0;
    while(SSP1IF == 0);
    SSP1IF = 0;
    SSP1CON2bits.PEN = 1;
    while(SSP1IF == 0);
    SSP1IF = 0;
}