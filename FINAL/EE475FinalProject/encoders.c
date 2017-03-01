/*
 * encoders.c
 *
 *  Created on: Feb 20, 2017
 *      Author: ee475
 */

#include "encoders.h"

// Left encoder (PORT 1) interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1IFG &= ~LEFT_PIN;
    //P1IES ^= LEFT_PIN;
    leftCount++;
}

// Right encoder (PORT 2) interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    P2IFG &= ~RIGHT_PIN;
    //P2IES ^= RIGHT_PIN;
    rightCount++;
}


void encodersInit(void){
    // Setup left encoder pin
    //P1DIR &= ~LEFT_PIN;
    //P1SEL |= LEFT_PIN;
    P1IE |= LEFT_PIN;
    P1IES |= BIT0;
    //P1IES &= ~BIT0;
    // P1IFG &= ~LEFT_PIN;

    // setup right encoder pin
    //P2DIR &= ~RIGHT_PIN;
    //P2SEL |= RIGHT_PIN;
    P2IE |= RIGHT_PIN;
    P2IES |= BIT0;
    // P2IES &= ~BIT0;
    // P2IFG &= ~RIGHT_PIN;

    // initialize counter values to zero
    leftCount = 0;
    rightCount = 0;
}

// Check distance left returns distance in millimeters
// that left wheel has turned since previous check.
int checkDistLeft(void){
    //int distanceLeft = (leftCount * WHEEL_CIRC) / ENCODER_POLES;
    //printf("left count: %d\n", leftCount);
    int distanceLeft = leftCount;
    return distanceLeft;
}

void resetLeftEncoder(void) {
    leftCount = 0;
}

// Check distance right returns distance in millimeters
// that right  wheel has turned since previous check.
int checkDistRight(void){
    //int distanceRight = (rightCount * WHEEL_CIRC)  / ENCODER_POLES;
    //printf("right count: %d\n", rightCount);
    int distanceRight = rightCount;
    return distanceRight;
}

void resetRightEncoder(void) {
    rightCount = 0;
}
