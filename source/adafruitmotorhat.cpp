/**
 *  adafruitmotorhat.cpp
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

#include "adafruitmotorhat.h"
#include "util.h"

AdafruitMotorHAT::AdafruitMotorHAT (int address, int freq)
    : controller (address)
    , frequency (freq)
{
    // set the frequency
    controller.setFrequency (frequency);

    // add the dc motors
    //for (int i = 0; i < 4; ++i)
    //{
    //    dcMotors.push_back (std::make_shared<AdafruitDCMotor> (controller, i));
    //}

    // add the stepper motors
    for (int i =0; i < 2; i++)
    {
        stepperMotors.push_back (std::make_shared<AdafruitStepperMotor>(controller, i));
    }
}

//std::shared_ptr<AdafruitDCMotor> AdafruitMotorHAT::getDCMotor (unsigned int number) {
//    if (number <= 0 && number > dcMotors.size()) {
//        return {};
//    }
//
//    return { dcMotors[number - 1] };
//}

std::shared_ptr<AdafruitStepperMotor> AdafruitMotorHAT::getStepperMotor(unsigned int number) {
    if (number <= 0 && number > stepperMotors.size()) {
        return {};
    }

    return stepperMotors[number - 1];
}

void AdafruitMotorHAT::setPin(int pin, bool enabled) {
    if (pin < 0 || pin > 15)
    {
        log::error ("Failed to set PWM pin " + std::to_string (pin) + ". Must be between 0 and 15 inclusive.");
        return;
    }

    controller.setChannel (pin, enabled ? 4096 : 0, enabled ? 0 : 4096);
}
