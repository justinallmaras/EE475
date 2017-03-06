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

#define EIGTH_TURN_CONST 41         //36mm
//  1.93 pulses per mm traveled
#define LATERAL_VERTEX_DISTANCE 115  //10cm
#define DIAGONAL_VERTEX_DISTANCE 149 // 14.14 cm
#define PID_TIME 0.01                  // 10 milliseconds

#define K_P    0.01
#define K_I    0.005
#define K_D    0.000

unsigned int state;
unsigned int updatePWM;
unsigned int PIDleftCountCurr;
unsigned int PIDrightCountCurr;
unsigned int PIDleftCountPrev;
unsigned int PIDrightCountPrev;
unsigned int logicLeftCount;
unsigned int logicRightCount;
int leftPWMadj;
int rightPWMadj;

// Motor control function declarations
void motorFunctionsLoop(void);
void motorsInit(int botNumber);
int traverseVertices(int targetVertex, int duty);
int moveForwards(int pos, int duty);
int turnLeft(int pos, int duty);
int turnRight(int pos, int duty);
int motorFeedbackLoop(int targetDistance, int8_t leftDir, int8_t rightDir, int startDuty);
//int motorFeedbackLoop(int targetLeft, int leftDir, int targetRight, int rightDir, int duty);
void leftControl(int dir, int duty);
void rightControl(int dir, int duty);

//void turnRight(int pwm);
void justForwards(double pwm);
//void moveBackwards(int pwm);
//void stop(void);

void setA(int a1, int a2);
void setB(int b1, int b2);
void setPWMA(double duty);
void setPWMB(double duty);

#endif /* MOTORFUNCTIONS_H_ */

