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

#ifndef ENCODERS_C_
#define ENCODERS_C_
#include "encoders.c"
#endif


#ifndef MOTORFUNCTIONS_H_
#define MOTORFUNCTIONS_H_

#define EIGTH_TURN_CONST 36         //36mm
#define LATERAL_VERTEX_DISTANCE 200 //15mm
#define DIAGONAL_VERTEX_DISTANCE 210 // 21.2 mm
#define RIGHT_ADJUST 1
#define LEFT_ADJUST  0

#define K_P     1
#define K_I     0.5
#define K_D     0.25

unsigned int state;

// Motor control function declarations
void motorFunctionsLoop(void);
void motorsInit(void);
int traverseVertices(int targetVertex, int duty);
int moveForwards(int pos, int duty);
int turnLeft(int pos, int duty);
int turnRight(int pos, int duty);
int motorFeedbackLoop(int targetLeft, int leftDir, int targetRight, int rightDir, int duty);
void leftControl(int dir, int duty);
void rightControl(int dir, int duty);

//void turnRight(int pwm);
//void justForwards(int pwm);
//void moveBackwards(int pwm);
//void stop(void);

void setA(int a1, int a2);
void setB(int b1, int b2);
void setPWMA(int duty);
void setPWMB(int duty);

#endif /* MOTORFUNCTIONS_H_ */

