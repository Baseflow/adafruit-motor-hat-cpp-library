//
// Created by maurits on 3/29/22.
//

#include <chrono>
#include "SimpleStepper.h"
#include "util.h"

SimpleStepper::SimpleStepper(PWM &pwm, int motor_index)
    : controller(pwm)
{
    if (motor_index == 0) {
        motor_pin_1 = 10;
        motor_pin_2 = 9;
        motor_pin_3 = 11;
        motor_pin_4 = 12;
    } else if (motor_index == 1) {
        motor_pin_1 = 4; // 4
        motor_pin_2 = 3; // 6
        motor_pin_3 = 5; // 3
        motor_pin_4 = 6; // 5
    }
}

void SimpleStepper::setSpeed(long whatSpeed)
{
    step_delay = 60L * 1000L * 1000L / kNumberOfSteps / whatSpeed;
}

void SimpleStepper::step(int steps_to_move)
{
    int steps_left = abs(steps_to_move);  // how many steps to take

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0) { direction = 1; }
    if (steps_to_move < 0) { direction = 0; }

    controller.setChannel(2, 0, 255);
    controller.setChannel(7, 0, 255);

    // decrement the number of steps, moving one step each time:
    while (steps_left > 0)
    {
        unsigned long now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        // move only if the appropriate delay has passed:
        //if (now - last_step_time >= step_delay)
        {
            // get the timeStamp of when you stepped:
            last_step_time = now;
            // increment or decrement the step number,
            // depending on direction:
            if (direction == 1)
            {
                step_number++;
                if (step_number == kNumberOfSteps) {
                    step_number = 0;
                }
            }
            else
            {
                if (step_number == 0) {
                    step_number = kNumberOfSteps;
                }
                step_number--;
            }
            // decrement the steps left:
            steps_left--;
            // step the motor to step number 0, 1, ..., {3 or 10}
            stepMotor(step_number % 4);
        }
    }
}

void SimpleStepper::stepMotor(int thisStep)
{
    if (kPinCount == 4) {
        switch (thisStep) {
            case 0:  // 1010
                util::setPin(controller, motor_pin_1, true);
                util::setPin(controller, motor_pin_2, false);
                util::setPin(controller, motor_pin_3, true);
                util::setPin(controller, motor_pin_4, false);
                break;
            case 1:  // 0110
                util::setPin(controller, motor_pin_1, false);
                util::setPin(controller, motor_pin_2, true);
                util::setPin(controller, motor_pin_3, true);
                util::setPin(controller, motor_pin_4, false);
                break;
            case 2:  //0101
                util::setPin(controller, motor_pin_1, false);
                util::setPin(controller, motor_pin_2, true);
                util::setPin(controller, motor_pin_3, false);
                util::setPin(controller, motor_pin_4, true);
                break;
            case 3:  //1001
                util::setPin(controller, motor_pin_1, true);
                util::setPin(controller, motor_pin_2, false);
                util::setPin(controller, motor_pin_3, false);
                util::setPin(controller, motor_pin_4, true);
                break;
        }
    }
}

void SimpleStepper::release()
{
    util::setPin(controller, motor_pin_1, false);
    util::setPin(controller, motor_pin_2, false);
    util::setPin(controller, motor_pin_3, false);
    util::setPin(controller, motor_pin_4, false);
}
