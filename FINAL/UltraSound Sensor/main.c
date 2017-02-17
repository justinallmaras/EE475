#include <msp430.h> 






#define TRIGGER_PIN     BIT4
#define ECHO_PIN        BIT3
#define MEAS_INTERVAL   60000

unsigned int up_counter;
unsigned int distance_cm;


/* Timer A0 Capture Interrupt routine
 P1.1 (echo) causes this routine to be called */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void) {
    if (TA0CCTL2 & CCI) {          // Raising edge

        up_counter = TA0CCR2;   // Copy counter to variable

    } else {                    // Falling edge

        // Formula: Distance in cm = (Time in uSec)/58
        distance_cm = (TA0CCR2 - up_counter)/58;
    }
    TA0CTL &= ~TAIFG;           // Clear interrupt flag - handled
}








/*
 * main.c
 */

void usSensorInit(void) {

    // initialize trigger pin low to start
    P1DIR |= TRIGGER_PIN;
    P1OUT &= ~TRIGGER_PIN;


    // initialize echo pin to capture input
    P1DIR &= ~ECHO_PIN;
    P1SEL |= ECHO_PIN;


    // initialize TA0 to capture in CCR2 on both edges
    TA0CCTL2 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;


    // initialize TA0 to compare CCR0
    TA0CTL |= TASSEL_2 + MC_2 + ID_0;


    //global interrupt
    _BIS_SR(GIE);


}

void triggerMeas(void) {

    // start trigger
    P1OUT ^= TRIGGER_PIN;

    // wait for 10us for trigger pulse
    __delay_cycles(10);

    // end trigger
    P1OUT ^= TRIGGER_PIN;

}


void usSensorMain(void) {
    usSensorInit();

    for(;;) {
        triggerMeas();

        // Adjust to fit measurement cycles
        __delay_cycles(MEAS_INTERVAL);
    }

}


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    usSensorMain();






    return 0;
}

