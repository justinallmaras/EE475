#include <intrinsics.h>
#include <stdint.h>
#include <msp430.h>
#include <stdio.h>

//reminder turn off additional clocks to reduce power consumption
#define TRIGGER_PIN     BIT4
#define ECHO_PIN        BIT3
#define LED_PIN         BIT3
#define MEAS_INTERVAL  60000
#define DISTANCE_CONST 58
unsigned int up_counter;
unsigned int distance;

/* Timer A0 Capture Interrupt routine P1.3 (echo) causes this routine to be called
 */
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void) {
    if (TA0CCTL2 & CCI) {           // Raising edge
        up_counter = TA0CCR2;       // Copy counter to variable
        P2OUT &= ~LED_PIN;
    } else {                        // Falling edge
        // Formula: Distance in cm = (Time in uSec)/58
        distance = (TA0CCR2 - up_counter) / DISTANCE_CONST;
        P2OUT |= LED_PIN;
        printf("distance: %d\n", distance);
    }


    //TA0CTL &= ~TAIFG;           // Clear interrupt flag - handled
    TA0IV = 0;
    _BIS_SR_IRQ(GIE);
}

/* Triggers the measurement on the ultrasonic sensor using 10us pulse on P1.4
 */
void triggerMeas(void) {
    // start trigger
    P1OUT |= TRIGGER_PIN;

    __delay_cycles(10);

    // end trigger
    P1OUT &= ~TRIGGER_PIN;
}



// Initializes the pins and configures timer TA0 to capture echo pulses
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



int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // tester led
    P2DIR |= LED_PIN;
    P2OUT &= ~LED_PIN;

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

