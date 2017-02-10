/*
 *EE 475 Lab2 RemoteNode.c
 * Bryan Bednarski, Justin Allmaras, Nick Lopez
 */

#include "remoteNode.h"

#define _XTAL_FREQ          16000000


short input = 0;
void interrupt I2C_Slave_Read() {
    if(SSP1IF == 1) { // master control interrupt flag
        SSP1CON1bits.CKP = 0;
        LATA0 = 1;
        // if overflow or write collision
        if ((SSP1CON1bits.SSPOV) || (SSP1CON1bits.WCOL)) {
            input = SSP1BUF;            // read the previous value to clear the buffer
            SSP1CON1bits.SSPOV = 0; // clear the overflow flag
            SSP1CON1bits.WCOL = 0;  // clear write collision flag
            SSP1CON1bits.CKP = 1;   // reset clock polarity
        }
        // if there is data and it has been written to
        if(!SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
          input = SSP1BUF;
          // while the buffer is not clear
          while(!SSP1STATbits.BF);
          //input = SSP1BUF; // set output ports to the value
          // WRITE TO THE MEMORY???
          SSP1CON1bits.CKP = 1; // set clock polarity
        // if there is data but it has not been written
        } else if(!SSP1STATbits.D_nA && SSP1STATbits.R_nW) {
          input = SSP1BUF;              // clear buffer
          SSP1STATbits.BF = 0;      // clear buffer flag
          SSP1BUF = input + 1;
          SSP1CON1bits.CKP = 1;
          while(SSP1STATbits.BF);
        }
        SSP1IF = 0;
      }
    }

    void I2C_Slave_Init(short address)
    {
        SSP1STAT = 0x80;
        SSP1ADD = address;
        SSP1CON1 = 0x36;
        SSP1CON2 = 0x01;
        TRISCbits.TRISC3 = 1;   //Setting as input as given in datasheet
        TRISCbits.TRISC4 = 1;   //Setting as input as given in datasheet
        ANSELCbits.ANSC3 = 0;
        ANSELCbits.ANSC4 = 0;
        INTCONbits.GIE = 1;
        INTCONbits.PEIE = 1;
        PIR1bits.SSPIF = 0;  // master synchronous serial port 1 interrupt flag
        PIE1bits.SSP1IE = 1; // master synchronous serial port 1 interrupt enable
    }

    void main()
    {
        OSCCON = 0b01110000;
        nRBPU = 0;            //Enables PORTB internal pull up resistors
        TRISB = 0x00;
        LATB0 = 1;
        TRISA = 0x00;
        ANSELAbits.ANSA1 = 0;
        LATA = 0xFF;
        I2C_Slave_Init(0x30); //Initialize as a I2C Slave with address 0x30
        while(1);
    }