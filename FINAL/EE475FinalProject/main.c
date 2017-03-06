
#include "main.h"


/*
 * main.c
 */


void main(void)
{
    // Disable watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // MASTER AND SYSTEM CLOCK SETTINGS
    // Setup SMCLK output on P2.2
    P2DIR |= BIT2;
    P2SEL |= BIT2;
    // Setup MCLK output on P7.7
    P7DIR |= BIT7;
    P7SEL |= BIT7;
    // Port select XT2
    P5SEL |= BIT2 + BIT3;
    // Enable XT2
    UCSCTL6 &= ~XT2OFF;
    UCSCTL3 |= SELREF_2;
    UCSCTL4 |= SELA_2;
    do {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1&OFIFG);
    UCSCTL6 &= ~XT2DRIVE0;
    UCSCTL4 |= SELS_5 + SELM_5;

    // Bot Select Input Pins Setup
    // Bit 0: P1.0, Bit 1: P1.1, Bit2: P1.2
    // Bots numbered 1-6
    P1DIR &= ~BIT0;
    P1DIR &= ~BIT1;
    P1DIR &= ~BIT2;
    botNumber |= (P1IN & BIT0);
    botNumber |= (P1IN & BIT1);
    botNumber |= (P1IN & BIT2);

    // init all sensor data buffers
    memset(transmitData, 0, 10);
    memset(IRFrontBack, 0, 2);
    memset(IRLeft, 0, 3);
    memset(IRRight, 0, 3);

    usSensorInit();             // initialize ultrasonic sensors
    irSensorInit();             // initialize IR sensors
    //motorsInit(botNumber);

    //__enable_interrupt();   // enable interrupts
    __bis_SR_register(GIE); // enable interrupts

    //int run = 1;
    for (;;) {
        //usSensorLoop();         // run ultrasonic sensor loop
        USdata = usTrigMeas();
        //irSensorLoop();
        //topDiodeData = readTopDiode();
        //checkIRFrontBack(IRFrontBack);
        checkIRLeft(IRLeft);
        checkIRRight(IRRight);

        // printf("front: %d, back: %d\n", IRFrontBack[0], IRFrontBack[1]);
        printf("back right: %d, right: %d, front right: %d\n", IRRight[0], IRRight[1], IRRight[2]);
        printf("back left: %d, left: %d, front left: %d\n", IRLeft[0], IRLeft[1], IRLeft[2]);
        // printf("Top diode: %d\n", topDiodeData);
        // printf("US Data: %d\n", USdata);
        // update transmit data packet
        transmitData[0] = botStatus;
        transmitData[1] = IRFrontBack[0];   // front
        transmitData[2] = IRFrontBack[1];   // back
        transmitData[3] = IRRight[0];       // back right
        transmitData[4] = IRRight[1];       // right
        transmitData[5] = IRRight[2];       // front right
        transmitData[6] = IRLeft[0];        // back left
        transmitData[7] = IRLeft[1];        // left
        transmitData[8] = IRLeft[2];        // front left
        transmitData[9] = topDiodeData;     // top photodiode
        transmitData[10] = USdata;          // ultrasonic data

        /*
        if(run == 1) { // run IR sensor loop
            motorFunctionsLoop();
            run--;
        }
        */

        __delay_cycles(3000000);
    }
    return 1;
}
