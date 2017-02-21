
/*
 *
 *
 *
 *
 *
 *
 */

#include "usSensor.h"
/* Timer A0 Capture Interrupt routine P1.3 (echo) causes this routine to be called
 */
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void) {
    if (TA0CCTL2 & CCI) {           // Raising edge
        up_counter = TA0CCR2;       // Copy counter to variable
    } else {                        // Falling edge
        // Formula: Distance in cm = (Time in uSec)/58
        distance = (TA0CCR2 - up_counter) / DISTANCE_CONST;
        printf("Ultrasonic Distance: %d\n", distance);
    }
    // Clear interrupt flag
    TA0IV = 0;
    _BIS_SR_IRQ(GIE);
}

/* Triggers the measurement on the ultrasonic sensor using 10us pulse on P2.5
 */
void usTrigMeas(void) {
    // start trigger
    P1OUT |= TRIGGER_PIN;
    __delay_cycles(10);
    // end trigger
    P1OUT &= ~TRIGGER_PIN;
}

/* Initializes the pins and configures timer TA0 to capture echo pulses
 */
void usSensorInit(void) {
    // initialize trigger pin low to start
    P1DIR |= TRIGGER_PIN;
    P1OUT &= ~TRIGGER_PIN;

    // initialize echo pin to capture input
    P1DIR &= ~ECHO_PIN;
    P1SEL |= ECHO_PIN;

    // Timer A0 configuration to read the echo signal
    TA0CTL |= TASSEL_2 + ID_0 + MC_2 + TACLR;

    // Timer TA0.2 control configuration
    TA0CCTL2 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;

    // Global interrupt disable
    _BIS_SR(GIE);
}


/* Looped commands
 */
void usSensorLoop(void) {
    //send trigger
    usTrigMeas();
    // wait measurement interval
    __delay_cycles(60000);
}


/*

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // initialize ultrasonic sensors
    usSensorInit();

    for(;;){
        //send trigger
        triggerMeas();

        // wait measurement interval
        __delay_cycles(60000);
    }
    return 0;
}
*/
