/*
 *EE 475 Lab2 RemoteNode.c
 * Bryan Bednarski, Justin Allmaras, Nick Lopez
 */

#include "remoteNode.h"

#define _XTAL_FREQ          16000000
#define RXBUFFER_SIZE       4

volatile short input;
volatile int setPoint;
volatile unsigned int dataReadADC;
volatile unsigned char dataToSend;
volatile int confirmation;
volatile char dataIsDirty;

void setupADC() {
    TRISCbits.TRISC5 = 1; // set TRISx bit to disable the digital output driver
    TRISCbits.TRISC6 = 0;
    ANSELCbits.ANSC5 = 1; // set ANSx bit to disable digital input buffer
    ADCON0 = 0x45;
    ADCON1 = 0x00;
    ADCON2 = 0xA2;
}

void readADC() {
    // set dataToSend
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE == 1){};
    dataReadADC = (ADRESH << 8) + ADRESL;
    if(dataReadADC >  512) {
        LATC6 = 1;
    } else {
        LATC6 = 0;
    }
    dataToSend = (unsigned char) (dataReadADC >> 2);
}

void updatePWM(char dutyCycle) {
    CCPR1L = (int) (159.0 * dutyCycle / 200);
}


void interrupt I2C_Slave_Read() { 
    LATCbits.LATC2 = 0;
    unsigned char address;
    if(SSP1STATbits.RW == 1) { // Read/Write Flag
        PIR1bits.SSP1IF = 0;        
        address = SSP1BUF;
        while(SSP1STATbits.BF == 1);
        SSP1BUF = dataToSend;
        SSP1CON1bits.CKP = 1;
        while(SSP1IF == 0);
        SSP1IF = 0;
        if(SSP1CON2bits.ACKSTAT == 0) {
            SSP1CON1bits.CKP = 1;
        } else {
            SSP1CON1bits.CKP = 1;
        }
        PIR1bits.SSP1IF = 0;
    } else {
        while(SSP1IF == 0);
        SSP1IF = 0;
        address = SSP1BUF;
        SSP1CON1bits.CKP = 1;
        while(SSP1STATbits.BF == 0);
        input = SSP1BUF;
        while(SSP1IF == 0);
        SSP1IF = 0;
        SSP1CON1bits.CKP = 1;
    }
    SSP1IF = 0;
    dataIsDirty = 1;
}

 /*   input = SSP1BUF;
    SSP1STATbits.BF = 0;
    SSP1CON1bits.CKP = 1;
    
    if(input == 'S') {
        setPoint = SSP1BUF;
        SSP1STATbits.BF = 0;
        SSP1CON1bits.CKP = 1;
        setPoint = 200;
        CCPR1L = (int) (159.0 * setPoint / 200);
    } else if(input == 'X') {
        setPoint = 100;
        CCPR1L = (int) (159.0 * setPoint / 200);
    } else if(input == 'Q') {
        setPoint = 0;
        CCPR1L = (int) (159.0 * setPoint / 200);
    } else if(input == 'I') {
        setPoint = 150;
        CCPR1L = (int) (159.0 * setPoint / 200);
    } else if (input == 'D') {
        setPoint = 50;
        CCPR1L = (int) (159.0 * setPoint / 200);
    }
   */ 
    
//}


    void I2C_Slave_Init(short address)
    {
        SSP1CON1 = 0x36;
        SSP1CON2 = 0x00;
        SSP1STAT = 0x00;
        SSP1ADD = address;
        TRISCbits.TRISC3 = 1;   //Setting as input as given in datasheet
        TRISCbits.TRISC4 = 1;   //Setting as input as given in datasheet
        ANSELCbits.ANSC3 = 0;
        ANSELCbits.ANSC4 = 0;
        INTCONbits.GIE = 1;
        INTCONbits.PEIE = 1;
        PIR1bits.SSP1IF = 0;  // master synchronous serial port 1 interrupt flag
        PIE1bits.SSP1IE = 1; // master synchronous serial port 1 interrupt enable
    }

    
    void setupPWM() {
        PR2 = 0b10011111 ;
        T2CON = 0b00000100;
        CCPR1L = 0;
        CCP1CON = 0b00111100 ;
    }
   
void main()
{
    input = 0;
    setPoint = 0;
    confirmation = 0;
    dataToSend = '#';
    dataReadADC = 0;
    OSCCON = 0b01110000;
    TRISA = 0x00;
    ANSELBbits.ANSB3 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB0 = 0;
    TRISCbits.TRISC2 = 0;
    ANSELC = 0x00;
    LATA = 0xFF;
    I2C_Slave_Init(0x30); //Initialize as a I2C Slave with address 0x30
    setupPWM();
    setupADC();
    short temp = 0;
    while(1) {
        if(dataIsDirty) {
            temp = input;
            if(confirmation == 0) {
                switch(temp){
                    case('X'):
                        setPoint = 100;
                        updatePWM(setPoint);
                        confirmation = 1;
                    break;
                    case('S'):
                        confirmation = 2;
                    break;
                    case('I'):
                        setPoint++;
                        updatePWM(setPoint);
                        confirmation = 1;
                    break;
                    case('D'):
                        setPoint--;
                        updatePWM(setPoint);
                        confirmation = 1;
                    break;
                    case('Q'):
                        setPoint = 0;
                        updatePWM(setPoint);
                        confirmation = 1;
                }
            } else if (confirmation == 1) {
                if(temp != ' ') {
                    setPoint = 0;
                }
                updatePWM(setPoint);
                confirmation = 0;
            } else if (confirmation == 2) {
                setPoint = (int) temp;
                updatePWM(setPoint);
                confirmation = 0;
            }
            // set data to read as error from analog reading
            readADC();
            dataIsDirty = 0;
        }
        __delay_ms(2);
        
    }
}