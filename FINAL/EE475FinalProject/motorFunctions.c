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
    //justForwards(20);
    //while(taskComplete != 1) {
        //taskComplete = traverseVertices(0, 20);
    //}

        for(i = 0; i < 8; i++) {
            // printf("loop iteration: %d\n", i);
            while(taskComplete != 1) {
                taskComplete = traverseVertices(i, 100);
            }
            taskComplete = 0;
            while(taskComplete != 1) {
                if(i < 4) {
                    taskComplete = traverseVertices(i + 4, 100);
                } else { // if i >=4
                    taskComplete = traverseVertices(i - 4, 100);
                }
            }
            taskComplete = 0;
        }


    // printf("leftDist: %d\n", leftDist);
    // printf("rightDist: %d\n", rightDist);
}

void motorsInit(void){
    state = 0;
    // PWMA: P2.4, PWMB: 2.5, timer TA2.1 TA2.2
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

    // AIN1: P3.2, AIN2: P3.3, BIN1: P3.4, BIN2: P3.5
    // Initialize pins as outputs and in stop mode. all low.
    P3DIR |= 0x3F;
    P3OUT &= 0xC3;

    // STBY: P2.7, H-bridge not active until initialized to high state
    // Will not alter standby pin after this line
    P3DIR |= BIT7;
    P3OUT |= BIT7;

    encodersInit();
}

int traverseVertices(int targetVertex, int duty) {
    int orient = state - targetVertex;
    int orientComplete = 0;
    int moveComplete = 0;
    resetLeftEncoder();
    resetRightEncoder();
    if(orient == 7) {
        orient = 1;
    } else if (orient == -7) {
        orient = -1;
    }
    while(!orientComplete || !moveComplete) {
        if (orient < 0) {
            orient *= -1;
            orientComplete = turnRight(orient, duty);
        } else if (orient > 0) {
            orientComplete = turnLeft(orient, duty);
        } else { // orient == 0
            orientComplete = 1;
        }
        moveComplete = moveForwards(targetVertex, duty);
    }
    // target vertex is to the right and bot must turn right

    state = targetVertex;
    return orientComplete * moveComplete;
}

int moveForwards(int pos, int duty) {
    int complete = 0;
    if(pos % 2 == 0) {
        // Lateral distance is valid
        complete = motorFeedbackLoop(LATERAL_VERTEX_DISTANCE, 1, LATERAL_VERTEX_DISTANCE, 1, duty);
    } else { // if position is odd, diagonal traversal necessary
        complete = motorFeedbackLoop(DIAGONAL_VERTEX_DISTANCE, 1, DIAGONAL_VERTEX_DISTANCE, 1, duty);
    }
    return complete;
}

// will turn robot "pos" number to left
int turnLeft(int pos, int duty) {
    int radialDistance = pos * EIGTH_TURN_CONST;
    int complete = motorFeedbackLoop(radialDistance, -1, radialDistance, 1, duty / 2);
    return complete;
}

// will turn robot "pos" number to right
int turnRight(int pos, int duty) {
    int radialDistance = pos * EIGTH_TURN_CONST;
    int complete = motorFeedbackLoop(radialDistance, 1, radialDistance, -1, duty / 2);
    return complete;
}

/*
 * targetLeft/targetRight = intended distances for left and right motors
 * leftDir/rightDir = direction CW(1)/CCW(-1) for left and right motors
 * duty = duty cycle speed for all motors
 * iteration = recursive cutoff for corrective feedback loop
 */
/*
int motorFeedbackLoop(int targetLeft, int leftDir, int targetRight, int rightDir, int duty, int iteration) {
    int leftDist = 0;
    int rightDist = 0;
    int completeLeft = 0;
    int completeRight = 0;
    int leftDifference;
    int rightDifference;
    int complete = 0;
    while(completeLeft != 1 && completeRight != 1) {
        if(completeLeft != 1) {
            leftControl(leftDir, duty);
        }
        if(completeRight != 1) {
            rightControl(rightDir, duty);
        }
        __delay_cycles(100000);
        leftDist += checkDistLeft();
        //printf("leftDist: %d\n", leftDist);
        rightDist += checkDistRight();
        if(leftDist >= targetLeft) {
            completeLeft = 1;
            leftControl(0, 0);
        }
        if(rightDist >= targetRight) {
            completeRight = 1;
            rightControl(0,0);
        }
    }
    // overshot distance by leftDifference and rightDifference at some pwm value
    complete = 1;
    leftDifference = leftDist - targetLeft;
    rightDifference = rightDist - targetRight;
    iteration++;
    if(iteration < 2) {
        complete = motorFeedbackLoop(leftDifference, (leftDir * -1), rightDifference, (rightDir * -1), (duty / 2), iteration);
    }
    return complete;
}
*/

int motorFeedbackLoop(int targetLeft, int leftDir, int targetRight, int rightDir, int duty) {
    int completeLeft = 0;
    int completeRight = 0;
    resetLeftEncoder();
    resetRightEncoder();
    int leftDist = checkDistLeft();
    int rightDist = checkDistRight();
    while(completeRight == 0 || completeLeft == 0) {
        rightDist = checkDistRight();
        leftDist = checkDistLeft();
        if(rightDist >= targetRight) {
            rightControl(0, duty);
            completeRight = 1;
        } else {
            rightControl(rightDir, duty);
        }
        if (leftDist >= targetLeft)  {
            leftControl(0, duty);
            completeLeft = 1;
        } else {
            leftControl(leftDir, duty);
        }
        //__delay_cycles(10);
    }
    __delay_cycles(1000000);
    //leftDist = checkDistLeft();
    //rightDist = checkDistRight();
    //printf("leftDist: %d\n", leftDist);
    //printf("rightDist: %d\n", rightDist);
    return 1;
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
void turnRight(int pwm) {
    setA(0, 1, pwm);
    setB(1, 0, pwm);
}

void turnLeft(int pwm) {
    setA(1, 0, pwm);
    setB(0, 1, pwm);
}

void justForwards(int pwm) {
    setA(1, 0, pwm);
    setB(1, 0, pwm);
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

// AIN1: P3.2, AIN2: P3.3
void setA(int a1, int a2) {
    // stop/halt
    if(a1 == 1 && a2 == 1) {
        P3OUT &= ~(BIT2);
        P3OUT &= ~(BIT3);
    // rotate CCW
    } else if (a1 == 1 && a2 == 0) {
        P3OUT &= ~BIT3;
        P3OUT |= BIT2;
     // rotate CW
    } else if (a1 == 0 && a2 == 1) {
        P3OUT &= ~BIT2;
        P3OUT |= BIT3;
    // stop
    } else if (a1 == 0 && a2 == 0) {
        P3OUT &= ~(BIT2);
        P3OUT &= ~(BIT3);
    }
}

// BIN1: P3.4, BIN2: P3.5
void setB(int b1, int b2) {
    //stop/halt
    if(b1 == 1 && b2 == 1) {
        P3OUT &= ~(BIT4);
        P3OUT &= ~(BIT5);
    // rotate CCW
    } else if (b1 == 1 && b2 == 0) {
        P3OUT &= ~BIT5;
        P3OUT |= BIT4;
    // rotate CW
    } else if (b1 == 0 && b2 == 1) {
        P3OUT &= ~BIT4;
        P3OUT |= BIT5;
    // stop
    } else if (b1 == 0 && b2 == 0) {
        P3OUT &= ~(BIT4);
        P3OUT &= ~(BIT5);
    }
}

// setPWM sets duty cycle for both motors input 0-100
void setPWMA(int duty) {
    duty = duty - RIGHT_ADJUST;
    if(duty > 100) {
        duty = 100;
    } else if (duty < 0) {
        duty = 0;
    }
    int pwm = (duty * 10);
    TA2CCR1 = pwm;              // CCR1 PWM duty cycle
}

// setPWM sets duty cycle for both motors input 0-100
void setPWMB(int duty) {
    duty = duty - LEFT_ADJUST;
    if(duty > 100) {
        duty = 100;
    } else if (duty < 0) {
        duty = 0;
    }
    int pwm = (duty * 10);
    TA2CCR2 = pwm;
}

