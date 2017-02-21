/*
 * motorFunctions.h
 *
 *  Created on: Feb 19, 2017
 *      Author: ee475
*/


//
// Supporting Libraries
//


// msp430 header
#ifndef MSP430_H_
#define MSP430_H_
#include <msp430.h>
#endif

// stdio.h header
#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif


#ifndef MOTORFUNCTIONS_H_
#define MOTORFUNCTIONS_H_

// Motor control function declarations
void motorFunctionsLoop(void);
void motorsInit(void);
void turnRight(int pwm);
void turnLeft(int pwm);
void moveForwards(int pwm);
void moveBackwards(int pwm);
void stop(void);
void setA(int a1, int a2, int pwm);
void setB(int b1, int b2, int pwm);
void setPWM(int dutyCycle);

#endif /* MOTORFUNCTIONS_H_ */

