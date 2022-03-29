//
// Created by Maurits van Beusekom on 3/22/22.
//

#include <array>

#include "adafruitsteppermotor.h"
#include "util.h"

AdafruitStepperMotor::AdafruitStepperMotor(PWM& pwm, int motorIndex)
    : controller(pwm), motorIndex(motorIndex), stepsPerRevolution(200) {
    init();
}

AdafruitStepperMotor::AdafruitStepperMotor(PWM& pwm, int motorIndex, int stepsPerRevolution)
    : controller(pwm), motorIndex(motorIndex), stepsPerRevolution(stepsPerRevolution) {
    init();
}

void AdafruitStepperMotor::init() {
    if (motorIndex == 0) {
        in1PinA = 10;
        in2PinA = 9;
        in1PinB = 11;
        in2PinB = 12;
        pwmPinA = 8;
        pwmPinB = 13;
    } else if (motorIndex == 1) {
        in1PinA = 4;
        in2PinB = 3;
        in1PinB = 5;
        in2PinB = 6;
        pwmPinA = 2;
        pwmPinB = 7;
    }
}

void AdafruitStepperMotor::setSpeed(int rpm) {
    secondsPerStep = 60.0f / (stepsPerRevolution * rpm);
    steppingCounter = 0;
}

int AdafruitStepperMotor::oneStep(Direction direction, Style style) {
    int pwm_a = 255, pwm_b = 255;

    if (style == Style::kSingle) {
        singleStep(direction);
    } else if (style == Style::kDouble) {
        doubleStep(direction);
    } else if (style == Style::kInterleave) {
        interleaveStep(direction);
    } else if (style == Style::kMicrostep) {
        microstep(direction, pwm_a, pwm_b);
    }

    nextStepAndWrap();

    controller.setChannel(pwmPinA, 0, pwm_a * 16);
    controller.setChannel(pwmPinB, 0, pwm_b * 16);

    std::array<bool, 4> coils[4] = { false, false, false, false };
    std::array<std::array<bool, 4>, 8> step2coils[8][4] = {
            { true, false, false, false },
            { true, true, false, false },
            { false, true, false, false },
            { false, true, true, false },
            { false, false, true, false },
            { false, false, true , true },
            { false, false, false, true },
            { true, false, false, true },
    };

    if (style == Style::kMicrostep) {
        if (currentStep >= 0 && currentStep < kMicrosteps) {
            std::copy(step2coils[1]->begin(), step2coils[1]->end(), coils);
        } else if (currentStep >= kMicrosteps && currentStep < kMicrosteps * 2) {
            std::copy(step2coils[3]->begin(), step2coils[3]->end(), coils);
        } else if (currentStep >= kMicrosteps * 2 && currentStep < kMicrosteps * 3) {
            std::copy(step2coils[5]->begin(), step2coils[5]->end(), coils);
        } else if (currentStep >= kMicrosteps * 3 && currentStep < kMicrosteps * 4) {
            std::copy(step2coils[7]->begin(), step2coils[7]->end(), coils);
        }
    } else {
        std::size_t index = currentStep / (kMicrosteps / 2);
        std::copy(step2coils[index]->begin(),step2coils[index]->end(), coils);
    }

    util::setPin(controller, in2PinA, coils->at(0));
    util::setPin(controller, in1PinB, coils->at(1));
    util::setPin(controller, in1PinA, coils->at(2));
    util::setPin(controller, in2PinB, coils->at(3));

    return currentStep;
}

void AdafruitStepperMotor::singleStep(Direction direction) {
    int stepSize = ((currentStep / (kMicrosteps / 2)) % 2) == 0
            ? kMicrosteps / 2
            : kMicrosteps;

    if (direction == Direction::kForward) {
        currentStep += stepSize;
    } else {
        currentStep -= stepSize;
    }
}

void AdafruitStepperMotor::doubleStep(Direction direction) {
    int stepSize = ((currentStep / (kMicrosteps / 2)) % 2) != 0
                   ? kMicrosteps / 2
                   : kMicrosteps;

    if (direction == Direction::kForward) {
        currentStep += stepSize;
    } else {
        currentStep -= stepSize;
    }
}

void AdafruitStepperMotor::interleaveStep(Direction direction) {
    if (direction == Direction::kForward) {
        currentStep += kMicrosteps / 2;
    } else {
        currentStep -= kMicrosteps / 2;
    }
}

void AdafruitStepperMotor::microstep(Direction direction, int& pwm_a, int& pwm_b) {
    if (direction == Direction::kForward) {
        currentStep += 1;
    } else {
        currentStep -= 1;

        nextStepAndWrap();
    }

    if (currentStep >= 0 && currentStep < kMicrosteps) {
        pwm_a = kMicrostepCurve[kMicrosteps - currentStep];
        pwm_b = kMicrostepCurve[currentStep];
    } else if (currentStep >= kMicrosteps && currentStep < kMicrosteps * 2) {
        pwm_a = kMicrostepCurve[currentStep - kMicrosteps];
        pwm_b = kMicrostepCurve[kMicrosteps * 2 - currentStep];
    } else if (currentStep >= kMicrosteps * 2 && currentStep < kMicrosteps * 3) {
        pwm_a = kMicrostepCurve[kMicrosteps * 3 - currentStep];
        pwm_b = kMicrostepCurve[currentStep - kMicrosteps * 2];
    } else if (currentStep >= kMicrosteps * 3 && currentStep < kMicrosteps * 4) {
        pwm_a = kMicrostepCurve[currentStep - kMicrosteps * 3];
        pwm_b = kMicrostepCurve[kMicrosteps * 4 - currentStep];
    }
}

void AdafruitStepperMotor::nextStepAndWrap() {
    currentStep += kMicrosteps * 4;
    currentStep %= kMicrosteps * 4;
}

void AdafruitStepperMotor::release() {
    util::setPin(controller, in2PinA, false);
    util::setPin(controller, in1PinB, false);
    util::setPin(controller, in1PinA, false);
    util::setPin(controller, in2PinB, false);
}