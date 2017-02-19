
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
    usSensorInit();             // initialize ultrasonic sensors
    irSensorInit();             // initialize IR sensors

    //
    // Loop
    //
    for (;;) {
        usSensorLoop();         // run ultrasonic sensor loop
        irSensorLoop();         // run IR sensor loop

        __delay_cycles(10000);
    }

}
