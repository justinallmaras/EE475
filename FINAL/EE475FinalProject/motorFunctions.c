/*
 * motorFunctions.c
 *
 *  Created on: Feb 19, 2017
 *      Author: ee475
*/

#include "motorFunctions.h"
#include "encoders.h"

void motorFunctionsLoop(void){

    int taskComplete = 0;
    int i;

    /*
    while(1) {
        //rightControl(1, 20);
        //leftControl(1, 20);
        //printf("hello\n");
        taskComplete = traverseVertices(0, 20);
        __delay_cycles(10000000);
        taskComplete = turnRight(4, 20);
        __delay_cycles(10000000);
        taskComplete = traverseVertices(0, 20);
        __delay_cycles(10000000);
        taskComplete = turnLeft(4, 20);
       __delay_cycles(10000000);
        // rightControl(-1, 20);
    }
    */

        for(i = 0; i < 8; i++) {
            // printf("loop iteration: %d\n", i);
            while(taskComplete != 1) {
                taskComplete = traverseVertices(i, 20);
            }
            __delay_cycles(3000000);
            taskComplete = 0;
            while(taskComplete != 1) {
                if(i < 4) {
                    taskComplete = traverseVertices(i + 4, 20);
                } else { // if i >=4
                    taskComplete = traverseVertices(i - 4, 20);
                }
            }
            taskComplete = 0;
            __delay_cycles(3000000);
        }

    // printf("leftDist: %d\n", leftDist);
    // printf("rightDist: %d\n", rightDist);
}

void motorsInit(int botNumber){
    state = 0;
    updatePWM = 0;

    PIDleftCountCurr = 0;
    PIDrightCountCurr = 0;
    PIDleftCountPrev = 0;
    PIDrightCountPrev = 0;

    // PWMA: P2.5 (TA2.2), PWMB: 2.4 (TA2.1)
    P2DIR |= 0x30;
    P2SEL |= 0x30; // set P4 pins to peripheral mode function
    // set base functionality for PWM pins

    TA2CCR0 = 1000 - 1;         // PWM Period
    TA2CCTL1 = OUTMOD_7;        // CCR1 reset/set
    TA2CCR1 = 0;                // CCR1 PWM duty cycle
    TA2CCTL2 = OUTMOD_7;
    TA2CCR2 = 0;                // initialize duty cycle to 0%
    // can try MC_1 for SMCLK
    TA2CTL = TASSEL_2 + MC_1;   // SMCLK, up mode

    //_BIS_SR(LPM0_bits);       // Enter LPM0

    // AIN1: P3.7, AIN2: P3.6, BIN1: P4.1, BIN2: P4.2
    // Initialize pins as outputs and in stop mode. all low.
    P3DIR |= 0xC0;
    P3OUT &= 0x3F;
    P4DIR |= 0x06;
    P4OUT &= 0xF9;

    // STBY: P4.0, H-bridge not active until initialized to high state
    // Will not alter standby pin after this line
    P4DIR |= BIT0;
    P4OUT |= BIT0;

    // setup motor function loop interrupt
    TBCCTL0 = CCIE;
    TBCCR0 = 50000;
    TBCTL = TBSSEL_2 + MC_1 + TBCLR;

    // scale by 1/10 for duty cycle equivalent
    if(botNumber == 1) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    } else if (botNumber == 2) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    } else if (botNumber == 3) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    } else if (botNumber == 4) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    } else if (botNumber == 5) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    } else if (botNumber == 6) {
        leftPWMadj = 0;
        rightPWMadj = 0;
    }

    encodersInit();
}

int traverseVertices(int targetVertex, int duty) {
    int diff = targetVertex - state;
    int orientComplete = 0;
    int moveComplete = 0;

    if(diff > 0) {
        if((diff) < 5) {
            orientComplete = turnRight(diff, duty);
        } else { //diff < 5
            orientComplete = turnLeft((8 - diff), duty);
        }
    } else if (diff < 0) {
        diff *= -1;
        if((diff) < 5) {
            orientComplete = turnLeft(diff, duty);
        } else { //diff < 5
            orientComplete = turnRight((8 - diff), duty);
        }
    } else { // diff == 0
        orientComplete = 1;
    }
    __delay_cycles(3000000);
    moveComplete = moveForwards(targetVertex, duty);
    state = targetVertex;
    return orientComplete * moveComplete;

}

int moveForwards(int pos, int duty) {
    int complete = 0;
    if(pos % 2 == 0) {
        // Lateral distance is valid
        complete = motorFeedbackLoop(LATERAL_VERTEX_DISTANCE, 1, 1, duty);
    } else { // if position is odd, diagonal traversal necessary
        complete = motorFeedbackLoop(DIAGONAL_VERTEX_DISTANCE, 1, 1, duty);
    }
    return complete;
}

// will turn robot "pos" number to left
int turnLeft(int pos, int duty) {
    int radialDistance = pos * EIGTH_TURN_CONST;
    int complete = motorFeedbackLoop(radialDistance, -1, 1, duty);
    return complete;
}

// will turn robot "pos" number to right
int turnRight(int pos, int duty) {
    int radialDistance = pos * EIGTH_TURN_CONST;
    int complete = motorFeedbackLoop(radialDistance, 1, -1, duty);
    return complete;
}

/*
 * targetLeft/targetRight = intended distances for left and right motors
 * leftDir/rightDir = direction CW(1)/CCW(-1) for left and right motors
 * duty = duty cycle speed for all motors
 * iteration = recursive cutoff for corrective feedback loop
 */

int motorFeedbackLoop(int targetDistance, int8_t leftDir, int8_t rightDir, int startDuty) {
    // left and right encoder and duty cycle initial variables
    int leftCount = 0;
    int rightCount = 0;
    resetLeftEncoder();
    resetRightEncoder();
    int dutyLeft = startDuty;
    int dutyRight = startDuty;

    uint8_t finished = 0;

    // PID control variables
    int p_error = 0;
    int p_error_prev = 0;
    int i_error = 0;
    int d_error = 0;
    double output;


    while (finished == 0) {
       // __delay_cycles(1000);
       // run distance checks on both
       // if one has completed, the stop it
       leftCount = checkLeftCount();
       rightCount = checkRightCount();
       // call timer interrupt and update PWM values
       if(updatePWM) {
           //printf("PID loop\n");
           p_error_prev = p_error;
           p_error = PIDleftCountCurr - PIDrightCountCurr;
           i_error = i_error + p_error;
           d_error = p_error - p_error_prev;
           output = (K_P * p_error) + (K_I * i_error * PID_TIME) + (K_D * d_error / PID_TIME);
           //output = (K_P * p_error);

           if(output > 0) {
               dutyRight += output;
           } else if (output < 0) {
               dutyRight -= output;
           }
           updatePWM = 0;
       }

       if (leftCount >= targetDistance || rightCount >= targetDistance) {
           rightControl(0,dutyRight);
           leftControl(0, dutyLeft);
           finished = 1;
       } else {
           leftControl(leftDir, dutyLeft);
           rightControl(rightDir, dutyRight);
       }
       /*
       if(completeRight == 0 || completeLeft == 0) {
           //printf("MC loop\n");

           if (leftCount >= targetDistance)  {
               leftControl(0, dutyLeft);
               completeLeft = 1;

           } else {
               leftControl(leftDir, dutyLeft);
           }
           if(rightCount >= targetDistance) {
               rightControl(0, dutyRight);
               completeRight = 1;
           } else {
               rightControl(rightDir, dutyRight);
           }
       } else {
           finished = 1;
       }
       */
    }
    return 1;
}

// interrupt to set updatePWM flag high
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERB0_VECTOR))) TIMERB0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    //printf("here 2\n");
    updatePWM = 1;
    PIDrightCountPrev = PIDrightCountCurr;
    PIDleftCountPrev = PIDleftCountCurr;
    PIDrightCountCurr = checkRightCount();
    PIDleftCountCurr = checkLeftCount();
}


void leftControl(int dir, int duty) {
    setPWMB(duty);
    // left forwards (dir = 1)
    if (dir == 1) {
        setB(1, 0);
    // left backwards (dir = -1)
    } else if (dir == -1) {
        setB(0, 1);
    // left stop (dir = 0)
    } else if (dir == 0) {
        setB(0, 0);
    }
}

void rightControl(int dir, int duty) {
    setPWMA(duty);
    // right forwards (dir = 1)
    if (dir == 1) {
        setA(0, 1);
    // right backwards (dir = 0)
    } else if (dir == -1) {
        setA(1, 0);
    // right stop (dir = 0)
    } else if (dir == 0) {
        setA(0, 0);
    }
}

/*
void justForwards(double pwm) {
    setA(1, 0);
    setB(1, 0);
}

void turnRight(int pwm) {
    setA(0, 1, pwm);
    setB(1, 0, pwm);
}

void turnLeft(int pwm) {
    setA(1, 0, pwm);
    setB(0, 1, pwm);
}



void moveBackwards(int pwm) {
    setA(0, 1, pwm);
    setB(0, 1, pwm);
}

void stop(void) {
    setA(0,0,0);
    setB(0,0,0);
}
*/

// right
// AIN1: P3.7, AIN2: P3.6
void setA(int a1, int a2) {
    // stop/halt
    if(a1 == 1 && a2 == 1) {
        P3OUT &= ~(BIT7);
        P3OUT &= ~(BIT6);
    // rotate CCW
    } else if (a1 == 1 && a2 == 0) {
        P3OUT &= ~BIT6;
        P3OUT |= BIT7;
     // rotate CW
    } else if (a1 == 0 && a2 == 1) {
        P3OUT &= ~BIT7;
        P3OUT |= BIT6;
    // stop
    } else if (a1 == 0 && a2 == 0) {
        P3OUT &= ~(BIT7);
        P3OUT &= ~(BIT6);
    }
}

// left
// old::: BIN1: P3.4, BIN2: P3.5
//        BIN1: P4.1, BIN2: P4.2
void setB(int b1, int b2) {
    //stop/halt
    if(b1 == 1 && b2 == 1) {
        P4OUT &= ~(BIT1);
        P4OUT &= ~(BIT2);
    // rotate CCW
    } else if (b1 == 1 && b2 == 0) {
        P4OUT &= ~BIT2;
        P4OUT |= BIT1;
    // rotate CW
    } else if (b1 == 0 && b2 == 1) {
        P4OUT &= ~BIT1;
        P4OUT |= BIT2;
    // stop
    } else if (b1 == 0 && b2 == 0) {
        P4OUT &= ~(BIT1);
        P4OUT &= ~(BIT2);
    }
}

// right
// setPWM sets duty cycle for both motors input 0-100
void setPWMA(double duty) {
    if(duty > 100.0) {
        duty = 100.0;
    } else if (duty < 0.0) {
        duty = 0.0;
    }
    int pwm = (int) (duty * 10 + rightPWMadj);
    TA2CCR2 = pwm;              // CCR1 PWM duty cycle
}

// left
// setPWM sets duty cycle for both motors input 0-100
void setPWMB(double duty) {
    if(duty > 100.0) {
        duty = 100.0;
    } else if (duty < 0.0) {
        duty = 0.0;
    }
    int pwm = (int) (duty * 10 + leftPWMadj);
    TA2CCR1 = pwm;
}
