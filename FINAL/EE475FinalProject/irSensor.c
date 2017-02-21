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

    //P2SEL &= 0x00;
    P1DIR |= BIT4;
    P2DIR |= BIT0;
}

// conversion results stored in ADC12MEMx
void setupADC(void){
    // configure ADC control bits
    P6SEL = 0x0F;
    ADC12CTL0 = ADC12ON + ADC12MSC +ADC12SHT0_8;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1; // sets sampling from input clock instead of signal

    // configure ADC inputs
    ADC12MCTL0 = ADC12INCH_0;              // A0
    ADC12MCTL1 = ADC12INCH_1;              // A1
    ADC12MCTL2 = ADC12INCH_2;              // A2
    ADC12MCTL3 = ADC12INCH_3 + ADC12EOS;   // A3 and end seq.
    //ADC12IE = 0x08;
    ADC12CTL0 |= ADC12ENC;
    // ADC12CTL0 |= ADC12SC;

    //__bis_SR_register(LPM0_bits + GIE);
    //__no_operation();
}

void updateADC(void){
    ADC12CTL0 |= 0x01; //ADC12SC

    ADCdistance1 = ADC12MEM0;
    ADCdistance2 = ADC12MEM1;
    ADCdistance3 = ADC12MEM2;
    ADCdistance4 = ADC12MEM3;

    ADC12CTL0 &= 0xFE; // ADC12SC

}

/* Update and test IR sensors
 */
void irSensorLoop(void) {
    P1OUT = P1OUT ^ 0x10;
    P2OUT = P2OUT ^ 0x01;
    if(P2OUT & 0x01 == 1) {
        updateADC();
    }
    printf("IR Distances -- 1: %d, 2: %d, 3: %d, 4: %d\n", ADCdistance1, ADCdistance2, ADCdistance3, ADCdistance4);
    /*if(ADCdata4 >= 250) {
        P2OUT |= 0x20;
    } else {
        P2OUT &= 0xDF;
    }
    */
}

/* Setup IR sensors and ADC read registers
 */
void irSensorInit(void) {
    setupGPIO();
    setupADC();

}



