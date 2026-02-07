#include "includes.hpp"

using namespace pros;

void on_center_button() {}

void initialize()
{
    imu.reset();
    liftImu.reset(true);
    // autonSelect.start();
    pros::lcd::initialize();
    
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    autonSelect.runAuton();
}

void opcontrol()
{
    bool intakeToggle = false;
    bool hoodToggle = false;
    bool scoring = false;
    while (true)
    {
        if (L1_NEW_PRESS) {
            scoring = true;
        } else if (L1_RELEASED) {
            scoring = false;
        }

        if (R1_NEW_PRESS) {
            intakeToggle = !intakeToggle;
        }

        pros::delay(10);
    }
}