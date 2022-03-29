/**
 *  adafruitmotorhat.h
 *
 *  MIT License
 *
 *  Copyright (c) 2018, Tom Clarke
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef ADAFRUITMOTOR_ADAFRUITMOTORHAT_H
#define ADAFRUITMOTOR_ADAFRUITMOTORHAT_H

#include <memory>
#include <vector>

#include "pwm.h"
#include "adafruitdcmotor.h"
#include "adafruitsteppermotor.h"
#include "SimpleStepper.h"

class AdafruitMotorHAT
{
public:
    AdafruitMotorHAT (int address = 0x60, int freq = 1600);

    /** Get one of the DC motors controlled by the HAT.
     *  Expects a value between 1 and 4 inclusive.
     *  If the number is out-of-range, the shared pointer
     *  returned from the method will be empty.
     */
    std::shared_ptr<AdafruitDCMotor> getDCMotor (unsigned int number);

    /** Get one of the Stepper motors controlled by the HAT.
     * Expects a value between 1 and 2 inclusive.
     * If the number is out-of-range, the shared pointer
     * returned from the method will be empty.
     */
     std::shared_ptr<AdafruitStepperMotor> getStepperMotor(unsigned int number);

     std::shared_ptr<SimpleStepper> getSimpleStepper(unsigned int number);

    void setPin(int pin, bool enabled);
private:
    PWM controller;
    const int frequency;
    std::vector<std::shared_ptr<AdafruitDCMotor>> dcMotors;
    std::vector<std::shared_ptr<AdafruitStepperMotor>> stepperMotors;
    std::vector<std::shared_ptr<SimpleStepper>> simpleSteppers;
};

#endif // ADAFRUITMOTOR_ADAFRUITMOTORHAT_H