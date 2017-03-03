
#include "main.h"

/*
 * main.c
 */

unsigned int state;
unsigned int botNumber;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // read three pins to determine robot number for motor control and radio comm.
    botNumber = 1;

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

    //usSensorInit();             // initialize ultrasonic sensors
    //irSensorInit();             // initialize IR sensors
    motorsInit(botNumber);

    //__enable_interrupt();   // enable interrupts
    __bis_SR_register(GIE); // enable interrupts

    int run = 1;

    //
    // Loop
    //
    for (;;) {
        //usSensorLoop();         // run ultrasonic sensor loop
        //irSensorLoop();
        if(run == 1) { // run IR sensor loop
            motorFunctionsLoop();
            run--;
        }
        break;
        //int leftDistance = checkDistLeft();
        //int rightDistance = checkDistRight();
        //printf("Left Distance: %d mm, Right Distance: %d mm\n", leftDistance, rightDistance);
        __delay_cycles(50000);
    }
    return;
}
