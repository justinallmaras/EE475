/*
 * irSensor.h
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
// IR Macros, Variables, and Functions
//
#ifndef IRSENSOR_H_
#define IRSENSOR_H_
// IR sensor macros
#define num_of_results  8

// IR sensor global variables
//volatile unsigned int ADCdata1[num_of_results];
//volatile unsigned int ADCdata2[num_of_results];
//volatile unsigned int ADCdata3[num_of_results];
//volatile unsigned int ADCdata4[num_of_results];
unsigned int ADCdata1;
unsigned int ADCdata2;
unsigned int ADCdata3;
unsigned int ADCdata4;

// IR Sensor Functions
void setupGPIO(void);
void setupADC(void);
void updateADC(void);
#endif /* IRSENSOR_H_ */
