/*
 * main.h
 *
 *  Created on: Feb 18, 2017
 *      Author: ee475
 */

// msp430 header
#ifndef MSP430_H_
#define MSP430_H_
#include <msp430.h>
#endif

#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif

#ifndef STDINT_H_
#define STDINT_H_
#include <stdint.h>
#endif

#ifndef STRING_H_
#define STRING_H_
#include <string.h>
#endif

#ifndef INTRINSICS_H_
#define INTRINSICS_H_
#include <intrinsics.h>
#endif

#ifndef USSENSOR_C_
#define USSENSOR_C_
#include "usSensor.c"
#endif

#ifndef IRSENSOR_C_
#define IRSENSOR_C_
#include "irSensor.c"
#endif

#ifndef MOTORFUNCTIONS_C_
#define MOTORFUNCTIONS_C_
#include "motorFunctions.c"
#endif

/*
#ifndef MSPRF24_H_
#define MSPRF24_H_
#include "msprf24.h"
#endif

#ifndef NRFUSERCONFIG_H_
#define NRFUSERCONFIG_H_
#include "nrf_userconfig.h"
#endif

#ifndef RADIOINTERFACE_C_
#define RADIOINTERFACE_C_
#include "radioInterface.c"
#endif
*/

// Main.c global variable declarations
unsigned int state;
uint8_t botNumber;
uint8_t transmitData[11];
uint8_t IRFrontBack[2];
uint8_t IRLeft[3];
uint8_t IRRight[3];
uint8_t USdata;
uint8_t topDiodeData;
uint8_t botStatus;

