/*
 * usSensor.h
 *
 *  Created on: Feb 18, 2017
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

// i.h header
#ifndef INTRINSICS_H_
#define INTRINSICS_H_
#include <intrinsics.h>
#endif

// sdtint.h header
#ifndef STDINT_H_
#define STDINT_H_
#include <stdint.h>
#endif

// stdio.h header
#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif


//
// Ultrasonic Sensor Macros, Variables, and Functions
//
#ifndef USSENSOR_H_
#define USSENSOR_H_

// Ultrasonic Sensor Macros
#define TRIGGER_PIN     BIT5
#define ECHO_PIN        BIT3
#define MEAS_INTERVAL   60000
#define DISTANCE_CONST  58

// Ultrasonic Sensor Global Variables
unsigned int up_counter;
unsigned int distance;

// Ultrasonic Sensor Functions
void usTrigMeas(void);
void usSensorInit(void);
void usSensorLoop(void);

#endif
