/*
 * irSensor.c
 *
 *  Created on: Feb 18, 2017
 *      Author: ee475
 */


#include "irSensor.h"

/* sets up pins 2.0 and 2.2 as output GPIOs
 */
void setupGPIO(void){

    P2SEL &= 0x00;
    P2DIR |= 0x31;
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
    // static unsigned int index = 0;
    /*while(index < 8) {
        ADCdata1[index] = ADC12MEM0;           // Move A0 results, IFG is cleared
        ADCdata2[index] = ADC12MEM1;           // Move A1 results, IFG is cleared
        ADCdata3[index] = ADC12MEM2;           // Move A2 results, IFG is cleared
        ADCdata4[index] = ADC12MEM3;           // Move A3 results, IFG is cleared
        index++;
    }
    */

    ADCdata1 = ADC12MEM0;
    ADCdata2 = ADC12MEM1;
    ADCdata3 = ADC12MEM2;
    ADCdata4 = ADC12MEM3;

    ADC12CTL0 &= 0xFE; // ADC12SC

}

/* Update and test IR sensors
 */
void irSensorLoop(void) {
    P2OUT = P2OUT ^ 0x11;
    if(P2OUT & 0x01 == 1) {
        updateADC();
    }
    if(ADCdata4 >= 250) {
        P2OUT |= 0x20;
    } else {
        P2OUT &= 0xDF;
    }
}

/* Setup IR sensors and ADC read registers
 */
void irSensorInit(void) {
    setupGPIO();
    setupADC();

}



