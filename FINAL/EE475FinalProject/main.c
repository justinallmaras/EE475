
#include "main.h"

/*
 * main.c
 */

unsigned int state;

void main(void)
{

    //
    // Initialization
    //

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    // setup system clock on P5 oscillator 6Mhz?
    P5SEL |= BIT2 + BIT3;
    UCSCTL6 &= ~0x1000;
    UCSCTL6 &= ~0x100;
    do {UCSCTL7 &= ~XT2OFFG;}               // Clear XT2 fault flags
    while ((UCSCTL7 & XT2OFFG) != 0);       // Test XT2 fault flag
    UCSCTL6 &= ~0xC000;
    UCSCTL4 |= 0x05;


    P7DIR |= BIT7;
    P7SEL |= BIT7;


    //usSensorInit();             // initialize ultrasonic sensors
    //irSensorInit();             // initialize IR sensors
    motorsInit();
    __enable_interrupt();
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
        //int leftDistance = checkDistLeft();
        //int rightDistance = checkDistRight();
        //printf("Left Distance: %d mm, Right Distance: %d mm\n", leftDistance, rightDistance);
        __delay_cycles(50000);
    }

}
