#include "includes.hpp"

using namespace pros;

void on_center_button() {}

void initialize()
{
    imu.reset();
    // liftImu.reset(true);
    // autonSelect.start();
    pros::lcd::initialize();
    systemMotors.set_zero_position(0);
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
    bool liftToggle = false;
    bool scoring = false;
    bool reverse = false;
    bool liftDown = false;
    bool wingState = false;
    int time = 0;
    int engageTime = 0;
    while (true)
    {
        time = pros::millis();
        chassis.arcade(LEFT_Y, RIGHT_X, false, 0.54);
        if (L1_NEW_PRESS)
        {
            scoring = true;
            if (liftDown)
            {
                systemMotors.set_zero_position(0);
            }
            liftDown = false;
        }
        else if (L1_RELEASED)
        {
            scoring = false;
        }
        if (DOWN_NEW_PRESS)
        {
            wingState = true;
            wing.setState(true);
        }
        else if (DOWN_RELEASED)
        {
            wingState = false;
            wing.setState(false);
        }
        if (L2_NEW_PRESS)
        {
            liftToggle = !liftToggle;
        }
        if (R2_NEW_PRESS)
        {
            reverse = true;
        }
        else if (R2_RELEASED)
        {
            reverse = false;
        }
        if (R1_NEW_PRESS)
        {
            intakeToggle = !intakeToggle;
        }

        if (scoring)
        {
            pto.setState(false);
            if (systemMotors.get_position() < 200)
                systemMotors.move(127);
            else if (systemMotors.get_position() < 240)
                systemMotors.move(40);
            else
                systemMotors.move(-10);
        }
        else if (!liftDown)
        {
            pto.setState(false);
            systemMotors.move(-127);
            if (systemMotors.get_position() < 20)
            {
                liftDown = true;
            }
        }
        else if (reverse)
        {
            if (!pto.getState())
            {
                engageTime = time;
                pto.setState(true);
            }
            else if (time - engageTime < 500)
            {
                systemMotors.move(0);
            }
            else
            {
                systemMotors.move(127);
            }
            // systemMotors.move(-127);
        }
        else if (intakeToggle)
        {
            if (!pto.getState())
            {
                engageTime = time;
                pto.setState(true);
            }
            else if (time - engageTime < 500)
            {
                systemMotors.move(0);
            }
            else
            {
                systemMotors.move(127);
            } // systemMotors.move(127);
        }
        else
        {
            systemMotors.move(0);
        }

        if (liftToggle)
        {
            lift.setState(true);
        }
        else
        {
            lift.setState(false);
        }

        pros::delay(10);
    }
}