#include <thread>
#include <chrono>
#include "adafruitmotorhat.h"

int main()
{
    using namespace std::chrono_literals;

    // connect using the default device address 0x60
    AdafruitMotorHAT hat;

    // get the motor connected to port 1
    if (auto motor { hat.getSimpleStepper (2) })
    {
        // speed must be set before running commands
        motor->setSpeed (100000);

	motor->step(200);

        motor->release();
    }

    return 0;
}
