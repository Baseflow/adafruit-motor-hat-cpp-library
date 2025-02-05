/**
 *  adafruitdcmotor.cpp
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

#include "adafruitdcmotor.h"
#include "util.h"

#include <iostream>
#include <algorithm>

AdafruitDCMotor::AdafruitDCMotor (PWM& pwm, int index)
    : controller (pwm)
{
    switch (index)
    {
        case 0:
            pwmPin = 8;
            in2Pin = 9;
            in1Pin = 10;
            break;
        case 1:
            pwmPin = 13;
            in2Pin = 12;
            in1Pin = 11;
            break;
        case 2:
            pwmPin = 2;
            in2Pin = 3;
            in1Pin = 4;
            break;
        case 3:
            pwmPin = 7;
            in2Pin = 6;
            in1Pin = 5;
            break;
        default:
            log::error ("Motor index out-of-range. Must be between 0 and 3 inclusive.");
            break;
    }
}

void AdafruitDCMotor::run (Command command)
{
    switch (command)
    {
        case kForward:
            util::setPin (controller, in1Pin, true);
            util::setPin (controller, in2Pin, false);
            break;
        case kBackward:
            util::setPin (controller, in1Pin, false);
            util::setPin (controller, in2Pin, true);
            break;
        case kBrake:
            util::setPin (controller, in1Pin, true);
            util::setPin (controller, in2Pin, true);
            break;
        case kRelease:
            util::setPin (controller, in1Pin, false);
            util::setPin (controller, in2Pin, false);
            break;
        default:
            break;
    }
}

void AdafruitDCMotor::setSpeed (int speed)
{
    speed = std::max (0, std::min (speed, 255));
    controller.setChannel (pwmPin, 0, speed * 16);
}