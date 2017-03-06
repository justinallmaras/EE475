/*
 * irSensor.c
 *
 *  Created on: Feb 18, 2017
 *      Author: ee475
 */


#include "irSensor.h"

/* sets up pins P2.0 and P1.4 2.4 as output GPIOs
 */
void setupGPIO(void){
    // Front Emitter P7.5, Right 3 Emitters P7.4
    // Left 3 emitters P8.1, back emitter P8.2
    P7DIR |= 0x30;
    P8DIR |= 0x06;
    P7OUT &= ~0x30;
    P8OUT &= ~0x06;
}

// conversion results stored in ADC12MEMx
void setupADC(void){
    // configure ADC control bits
    P6SEL |= 0xFE;
    P7SEL |= 0x01;
    ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_8 + ADC12SHT1_8;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1; // sets sampling from input clock instead of signal

    // configure ADC inputs
    ADC12MCTL0 = ADC12INCH_0;              // A0 -- photodiode P6.0
    ADC12MCTL1 = ADC12INCH_1;              // A1 -- PD front P6.1
    ADC12MCTL2 = ADC12INCH_2;              // A2 -- PD Front Right P6.2
    ADC12MCTL3 = ADC12INCH_3;              // A3 -- PD Front Left P6.3
    ADC12MCTL4 = ADC12INCH_4;              // A4 -- PD Left P6.4
    ADC12MCTL5 = ADC12INCH_5;              // A5 -- PD Back Left P6.5
    ADC12MCTL6 = ADC12INCH_6;              // A6 -- PD Back P6.6
    ADC12MCTL7 = ADC12INCH_7;              // A7 -- PD Back Right P6.7
    ADC12MCTL12 = ADC12INCH_12 + ADC12EOS; // A12 and end seq. PD Right P7.0
    //ADC12IE = 0x08;
    ADC12CTL0 |= ADC12ENC;
    // ADC12CTL0 |= ADC12SC;

    //__bis_SR_register(LPM0_bits + GIE);
    //__no_operation();
}

/* Setup IR sensors and ADC read registers
 */
void irSensorInit(void) {
    setupGPIO();
    setupADC();
}

void updateADC(void){
    ADC12CTL0 |= 0x01; //ADC12SC
    __delay_cycles(1000);
    topDiodeRead = ADC12MEM0;
    ADCdistance1 = ADC12MEM1;
    ADCdistance2 = ADC12MEM2;
    ADCdistance3 = ADC12MEM3;
    ADCdistance4 = ADC12MEM4;
    ADCdistance5 = ADC12MEM5;
    ADCdistance6 = ADC12MEM6;
    ADCdistance7 = ADC12MEM7;
    ADCdistance8 = ADC12MEM12;
    //printf("IR Distances -- Front: %d, FRight: %d, FLeft: %d, Left: %d\n", ADCdistance1, ADCdistance2, ADCdistance3, ADCdistance4);
    //printf("IR Distances -- BLeft: %d, Back: %d, BRight: %d, Right: %d\n", ADCdistance5, ADCdistance6, ADCdistance7, ADCdistance8);
    ADC12CTL0 &= 0xFE; // ADC12SC
    __delay_cycles(100000);

}

uint8_t readTopDiode(){
    ADC12CTL0 |= 0x01; //ADC12SC
    topDiode = ADC12MEM0;
    ADC12CTL0 &= 0xFE; // ADC12SC
    return (uint8_t) (topDiode >> 4);
}

void checkIRFrontBack(uint8_t* frontBackData) {
    // turn on IR emitters for front (P7.5) and back (P8.2)
    P7OUT |= 0x20;
    P8OUT |= 0x04;
    __delay_cycles(60000);
    updateADC();
    // turn off IR emitters for front (P7.5) and back (P8.2)
    P7OUT &= ~0x20;
    P8OUT &= ~0x04;
    frontBackData[0] = (uint8_t) (ADCdistance1 >> 4); // front
    frontBackData[1] = (uint8_t) (ADCdistance6 >> 4); // back
}

void checkIRLeft(uint8_t* leftData) {
    // turn on IR emitter for left three (P8.1)
    P8OUT |= 0x02;
    __delay_cycles(60000);
    updateADC();
    // turn off IR emitters for front (P7.5) and back (P8.2)
    P8OUT &= ~0x02;
    leftData[0] = (uint8_t) (ADCdistance5 >> 4); // back left
    leftData[1] = (uint8_t) (ADCdistance4 >> 4); // left
    leftData[2] = (uint8_t) (ADCdistance3 >> 4); // front left
}

void checkIRRight(uint8_t* rightData) {
    // turn on IR emitter for left three (P8.1)
    P7OUT |= 0x10;
    __delay_cycles(60000);
    updateADC();
    // turn off IR emitters for front (P7.5) and back (P8.2)
    P7OUT &= ~0x10;
    rightData[0] = (uint8_t) (ADCdistance7 >> 4); // back right
    rightData[1] = (uint8_t) (ADCdistance8 >> 4); // right
    rightData[2] = (uint8_t) (ADCdistance2 >> 4); // front right
    //printf("back right: %d, right: %d, front right: %d\n", rightData[0], rightData[1], rightData[2]);
}


/* Update and test IR sensors
 */
void irSensorLoop(void) {
    // Turn IR emitters on
    P7OUT |= 0x30;
    P8OUT |= 0x06;
    __delay_cycles(60000);
    updateADC();
    // Turn off IR emitters
    P7OUT &= ~0x30;
    P8OUT &= ~0x06;
    //__delay_cycles(10);
    //printf("IR Distances -- 1: %d\n", ADCdistance1);
   //printf("IR Distances -- 1: %d, 2: %d, 3: %d, 4: %d\n", ADCdistance1, ADCdistance2, ADCdistance3, ADCdistance4);
   //printf("IR Distances -- 5: %d, 6: %d, 7: %d, 8: %d\n", ADCdistance5, ADCdistance6, ADCdistance7, ADCdistance8);
   //printf("Photodiode read: %d\n", topDiodeRead);
}


