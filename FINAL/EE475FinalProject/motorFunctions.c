/*
 * motorFunctions.c
 *
 *  Created on: Feb 19, 2017
 *      Author: ee475
*/

#include "motorFunctions.h"

void motorFunctionsLoop(void){
    turnRight(50);
    __delay_cycles(1000000);
    turnLeft(50);
    __delay_cycles(100000);
    moveForwards(50);
    __delay_cycles(1000000);
    moveBackwards(50);
    __delay_cycles(1000000);
}

void motorsInit(void){
    // PWMA: P2.4, PWMB: 2.5, timer TA2.1 TA2.2
    P2DIR |= 0x30;
    P2SEL |= 0x30; // set P4 pins to peripheral mode function
    // set base functionality for PWM pins

    TA2CCR0 = 1000 - 1;         // PWM Period
    TA2CCTL1 = OUTMOD_7;        // CCR1 reset/set
    TA2CCR1 = 0;                // CCR1 PWM duty cycle
    TA2CCTL2 = OUTMOD_7;
    TA2CCR2 = 0;                // initialize duty cycle to 0%
    // can try MC_1 for SMCLK
    TA2CTL = TASSEL_2 + MC_1;   // SMCLK, up mode

    //_BIS_SR(LPM0_bits);       // Enter LPM0

    // AIN1: P3.2, AIN2: P3.3, BIN1: P3.4, BIN2: P3.5
    // Initialize pins as outputs and in stop mode. all low.
    P3DIR |= 0x3F;
    P3OUT &= 0xC3;

    // STBY: P2.7, H-bridge not active until initialized to high state
    // Will not alter standby pin after this line
    P3DIR |= BIT7;
    P3OUT |= BIT7;
}

void turnRight(int pwm) {
    setA(0, 1, pwm);
    setB(1, 0, pwm);
}

void turnLeft(int pwm) {
    setA(1, 0, pwm);
    setB(0, 1, pwm);
}

void moveForwards(int pwm) {
    setA(1, 0, pwm);
    setB(1, 0, pwm);
}

void moveBackwards(int pwm) {
    setA(0, 1, pwm);
    setB(0, 1, pwm);
}

void stop(void) {
    setA(0,0,0);
    setB(0,0,0);
}

// AIN1: P3.2, AIN2: P3.3
void setA(int a1, int a2, int duty) {
    setPWM(duty);
    // stop/halt
    if(a1 == 1 && a2 == 1) {
        P3OUT &= ~BIT2 + ~BIT3;
    // rotate CCW
    } else if (a1 == 1 && a2 == 0) {
        P3OUT &= ~BIT3;
        P3OUT |= BIT2;
     // rotate CW
    } else if (a1 == 0 && a2 == 1) {
        P3OUT &= ~BIT2;
        P3OUT |= BIT3;
    // stop
    } else if (a1 == 0 && a2 == 0) {
        P3OUT &= ~BIT2 + ~BIT3;
    }
}

// BIN1: P3.4, BIN2: P3.5
void setB(int b1, int b2, int duty) {
    setPWM(duty);
    //stop/halt
    if(b1 == 1 && b2 == 1) {
        P3OUT &= ~BIT4 + ~BIT5;
    // rotate CCW
    } else if (b1 == 1 && b2 == 0) {
        P3OUT &= ~BIT5;
        P3OUT |= BIT4;
    // rotate CW
    } else if (b1 == 0 && b2 == 1) {
        P3OUT &= ~BIT4;
        P3OUT |= BIT5;
    // stop
    } else if (b1 == 0 && b2 == 0) {
        P3OUT &= ~BIT4 + ~BIT5;
    }
}

// setPWM sets duty cycle for both motors input 0-100
void setPWM(int duty) {
    if(duty > 100) {
        duty = 100;
    } else if (duty < 0) {
        duty = 0;
    }
    int pwm = (duty * 10);
    TA2CCR1 = pwm;              // CCR1 PWM duty cycle
    TA2CCR2 = pwm;
}


