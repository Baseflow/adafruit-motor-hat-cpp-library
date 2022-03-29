//
// Created by Maurits van Beusekom on 3/22/22.
//

#ifndef ADAFRUITMOTOR_ADAFRUITSTEPPERMOTOR_H
#define ADAFRUITMOTOR_ADAFRUITSTEPPERMOTOR_H

#include "pwm.h"

class AdafruitStepperMotor
{
private:
    const int kMicrosteps = 8;
    const int kMicrostepCurve[9] = { 0, 50, 98, 142, 180, 212, 236, 250, 255};

    PWM& controller;
    int motorIndex, stepsPerRevolution;
    int steppingCounter = 0, currentStep = 0;
    int in1PinA = 0, in2PinA = 0, in1PinB = 0, in2PinB = 0, pwmPinA = 0, pwmPinB = 0;
    float secondsPerStep = 0.1;

public:
    enum Direction {
        kForward = 1,
        kBackward = 2,
    };

    enum Style {
        kSingle = 1,
        kDouble = 2,
        kInterleave = 3,
        kMicrostep = 4,
    };

private:
    void init();

    void singleStep(Direction direction);

    void doubleStep(Direction direction);

    void interleaveStep(Direction direction);

    void microstep(Direction direction, int& pwm_a, int& pwm_b);

    void nextStepAndWrap();

public:
    AdafruitStepperMotor(PWM& pwm, int motorIndex);
    AdafruitStepperMotor(PWM& pwm, int motorIndex, int steps);

    void setSpeed(int rpm);

    int oneStep(Direction direction, Style style);

    void release();
};

#endif //ADAFRUITMOTOR_ADAFRUITSTEPPERMOTOR_H
