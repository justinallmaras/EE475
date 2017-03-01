
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
