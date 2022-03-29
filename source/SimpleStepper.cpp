//
// Created by maurits on 3/29/22.
//

#include "SimpleStepper.h"
#include "util.h"

SimpleStepper::Stepper(PWM &pwm, int motor_index)
    : controller(pwm)
{
    if (motorIndex == 0) {
        motor_pin_1 = 10;
        motor_pin_2 = 9;
        motor_pin_3 = 11;
        motor_pin_4 = 12;
    } else if (motorIndex == 1) {
        motor_pin_1 = 4;
        motor_pin_2 = 3;
        motor_pin_3 = 5;
        motor_pin_4 = 6;
    }
}

void Stepper::setSpeed(long whatSpeed)
{
    this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

void Stepper::step(int steps_to_move)
{
    int steps_left = abs(steps_to_move);  // how many steps to take

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0) { this->direction = 1; }
    if (steps_to_move < 0) { this->direction = 0; }


    // decrement the number of steps, moving one step each time:
    while (steps_left > 0)
    {
        unsigned long now = micros();
        // move only if the appropriate delay has passed:
        if (now - this->last_step_time >= this->step_delay)
        {
            // get the timeStamp of when you stepped:
            this->last_step_time = now;
            // increment or decrement the step number,
            // depending on direction:
            if (this->direction == 1)
            {
                this->step_number++;
                if (this->step_number == this->number_of_steps) {
                    this->step_number = 0;
                }
            }
            else
            {
                if (this->step_number == 0) {
                    this->step_number = this->number_of_steps;
                }
                this->step_number--;
            }
            // decrement the steps left:
            steps_left--;
            // step the motor to step number 0, 1, ..., {3 or 10}
            if (this->pin_count == 5)
                stepMotor(this->step_number % 10);
            else
                stepMotor(this->step_number % 4);
        }
    }
}

void Stepper::stepMotor(int thisStep)
{
    if (this->pin_count == 4) {
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

void release()
{
    util::setPin(controller, motor_pin_1, false);
    util::setPin(controller, motor_pin_2, false);
    util::setPin(controller, motor_pin_3, false);
    util::setPin(controller, motor_pin_4, false);
}
