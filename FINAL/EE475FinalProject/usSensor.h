/*
 * usSensor.h
 *
 *  Created on: Feb 18, 2017
 *      Author: ee475
 */

//
// Ultrasonic Sensor Macros, Variables, and Functions
//
#ifndef USSENSOR_H_
#define USSENSOR_H_

// Ultrasonic Sensor Macros
#define TRIGGER_PIN     BIT5
#define ECHO_PIN        BIT3
#define MEAS_INTERVAL   60000
#define DISTANCE_CONST  348  //58

// Ultrasonic Sensor Global Variables
unsigned int up_counter;
uint8_t distance;

// Ultrasonic Sensor Functions
uint8_t usTrigMeas(void);
void usSensorInit(void);
//void usSensorLoop(void);

#endif
