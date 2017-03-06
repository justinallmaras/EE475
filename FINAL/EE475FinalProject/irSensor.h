/*
 * irSensor.h
 *
 *  Created on: Feb 18, 2017
 *      Author: ee475
 */

//
// Supporting Libraries
//


//
// IR Macros, Variables, and Functions
//
#ifndef IRSENSOR_H_
#define IRSENSOR_H_
// IR sensor macros
#define num_of_results  8

// IR sensor global variables
volatile unsigned int topDiodeRead;
volatile unsigned int ADCdistance1;
volatile unsigned int ADCdistance2;
volatile unsigned int ADCdistance3;
volatile unsigned int ADCdistance4;
volatile unsigned int ADCdistance5;
volatile unsigned int ADCdistance6;
volatile unsigned int ADCdistance7;
volatile unsigned int ADCdistance8;
volatile unsigned int topDiode;

// IR Sensor Functions
void setupGPIO(void);
void setupADC(void);
void irSensorInit(void);
void updateADC(void);
void irSensorLoop(void);
uint8_t readTopDiode(void);
void checkIRFrontBack(uint8_t* frontBackData);
void checkIRLeft(uint8_t* leftData);
void checkIRRight(uint8_t* rightData);
#endif /* IRSENSOR_H_ */
