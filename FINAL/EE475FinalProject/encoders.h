/*
 * encoders.h
 *
 *  Created on: Feb 20, 2017
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


#ifndef ENCODERS_H_
#define ENCODERS_H_

// Encoder Sensor Macros
#define LEFT_PIN     BIT2
#define RIGHT_PIN    BIT6

// Ultrasonic Sensor Global Variables
volatile unsigned int leftCount;
volatile unsigned int rightCount;

// Motor control function declarations
void encodersInit(void);
int checkDistLeft(void);
int checkDistRight(void);


#endif /* ENCODERS_H_ */
