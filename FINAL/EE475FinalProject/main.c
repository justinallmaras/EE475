
#include "main.h"

/*
 * main.c
 */
void main(void)
{

    //
    // Initialization
    //
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    //usSensorInit();             // initialize ultrasonic sensors
    //irSensorInit();             // initialize IR sensors
    //motorsInit();
    encodersInit();
    __enable_interrupt();

    //
    // Loop
    //
    for (;;) {
        //usSensorLoop();         // run ultrasonic sensor loop
        //irSensorLoop();         // run IR sensor loop
        // motorFunctionsLoop();
        int leftDistance = checkDistLeft();
        int rightDistance = checkDistRight();
        printf("Left Distance: %d mm, Right Distance: %d mm\n", leftDistance, rightDistance);
        __delay_cycles(5000000);
    }

}
