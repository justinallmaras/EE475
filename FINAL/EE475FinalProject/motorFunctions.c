/*
 * motorFunctions.c
 *
 *  Created on: Feb 19, 2017
 *      Author: ee475
*/

#include "motorFunctions.h"

void motorFunctionLoop(void){
    printf("Motor loop entered");
    moveForwards(50);
}

void initMotors(void){
    // PWMA: P1.4, PWMB: 1.5, timer TA0.3, TA0.4
    P1DIR |= 0x30;
    P1SEL |= 0x30; // set P4 pins to peripheral mode function
    // set base functionality for PWM pins

    TA0CCR0 = 512 - 1;           // PWM Period
    TA0CCTL3 = OUTMOD_7;        // CCR1 reset/set
    TA0CCR3 = 255;              // CCR1 PWM duty cycle
    TA0CCTL4 = OUTMOD_7;
    TA0CCR4 = 255;
    // can try MC_1 for SMCLK, used MC_2 to match IR sensors
    TA0CTL = TASSEL_2 + MC_2;   // SMCLK, up mode

    //_BIS_SR(LPM0_bits);       // Enter LPM0

    // AIN1: P3.2, AIN2: P3.3, BIN1: P3.4, BIN2: P3.5
    // Initialize pins as outputs and in stop mode. all low.
    P3DIR |= 0x3F;
    P3OUT &= 0xC3;

    // STBY: P2.7, H-bridge not active until initialized to high state
    // Will not alter standby pin after this line
    P2DIR |= 0x80;
    P2OUT |= 0x80;
}

void turnRight(int pwm) {
    setA(0, 1, pwm);
    setB(1, 0, pwm);
}

void turnLeft(int pwm) {
    setA(1, 0, pwm);
    setB(0, 1, pwm);
}

void moveFowards(int pwm) {
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
    if(a1 == 1 && a2 == 1) {
        P3OUT |= 0x0C;
    } else if (a1 == 1 && a2 == 0) {
        P3OUT &= ~BIT3;
        P3OUT |= BIT2;
    } else if (a1 == 0 && a2 == 1) {
        P3OUT &= ~BIT2;
        P3OUT |= BIT3;
    } else if (a1 == 0 && a2 == 0) {
        P3OUT &= ~BIT2 + ~BIT3;
    }
}

// BIN1: P3.4, BIN2: P3.5
void setB(int b1, int b2, int duty) {
    setPWM(duty);
    if(b1 == 1 && b2 == 1) {
        P3OUT |= 0x30;
    } else if (b1 == 1 && b2 == 0) {
        P3OUT &= ~BIT5;
        P3OUT |= BIT4;
    } else if (b1 == 0 && b2 == 1) {
        P3OUT &= ~BIT4;
        P3OUT |= BIT5;
    } else if (b1 == 0 && b2 == 0) {
        P3OUT &= ~BIT4 + ~BIT5;
    }
}

// setPWM sets duty cycle for both motors input 0-100
void setPWM(int duty) {
    if(duty <= 100) {
        int pwm = (duty / 100) * 511;
        TA0CCR3 = pwm;              // CCR1 PWM duty cycle
        TA0CCR4 = pwm;
    }
}


