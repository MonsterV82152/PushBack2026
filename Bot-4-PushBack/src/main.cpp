#include "includes.hpp"
#include "movements.hpp"

using namespace pros;

void on_center_button() {}

void initialize()
{
    // imu.reset();
    // liftImu.reset(true);
    // autonSelect.start();
    chassis.calibrate();
    pros::lcd::initialize();
    systemMotors.set_zero_position(0);
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    // autonSelect.runAuton();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 24, 10000);
}

void opcontrol()
{
    while (true)
    {
        chassis.arcade(LEFT_Y, RIGHT_X, false, 0.54);
        if (L1_NEW_PRESS)
        {
            scoreAndHold();
        }
        else if (L1_RELEASED)
        {
            lowerScoring();
        }
        if (DOWN_NEW_PRESS)
        {
            wingToggle(true);
        }
        else if (DOWN_RELEASED)
        {
            wingToggle(false);
        }
        if (L2_NEW_PRESS)
        {
            liftToggle();
        }
        if (R2_NEW_PRESS)
        {
            reverse(true);
            intakeLiftToggle(true);
        }
        else if (R2_RELEASED)
        {
            reverse(false);
            intakeLiftToggle(false);
        }
        if (R1_NEW_PRESS)
        {
            intake();
        }
        if (Y_NEW_PRESS)
        {
            matchLoad(true);
        }
        else if (Y_RELEASED)
        {
            matchLoad(false);
        }

        pros::delay(10);
    }
}