//
// Created by maurits on 3/29/22.
//

#ifndef ADAFRUIT_MOTOR_HAT_CPP_LIBRARY_SIMPLESTEPPER_H
#define ADAFRUIT_MOTOR_HAT_CPP_LIBRARY_SIMPLESTEPPER_H

#include "pwm.h"

class SimpleStepper {
private:
    const int kPinCount = 4;
    const int kNumberOfSteps = 200;

    int direction; // Direction of rotation.
    int step_number; // The current step the motor is on.

    unsigned long step_delay; // Delay between steps, in ms, based on speed.
    unsigned long last_step_time; // Timestamp in us of when the last step was taken.

    PWM& controller;

    // Motor pin numbers
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
public:
    Stepper(PWM& pwm, int motor_index);

    void setSpeed(long whatSpeed);

    void step(int number_of_steps);

    void release();

private:
    void stepMotor(int this_step);
};


#endif //ADAFRUIT_MOTOR_HAT_CPP_LIBRARY_SIMPLESTEPPER_H
