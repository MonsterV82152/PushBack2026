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
    // autonSelect.runAuton();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 24, 10000);
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
        double currentAngle = leverImu.get_pitch();
        chassis.arcade(LEFT_Y, RIGHT_X, false, 0.54);
        if (L1_NEW_PRESS)
        {
            scoring = true;
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
            if (!liftToggle)
            {
                wingState = false;
                wing.setState(false);
            }
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
            if (pto.getState())
            {
                engageTime = time;
                pto.setState(false);
            }
            else if (time - engageTime < 100)
            {
                systemMotors.move(0);
            }
            else
            {
                if (systemMotors.get_position() < 60)
                    systemMotors.move(100);
                else if (systemMotors.get_position() < 79)
                    systemMotors.move(30);
                else
                    systemMotors.move(-10);
            }
        }
        else if (!liftDown)
        {
            if (pto.getState())
            {
                engageTime = time;
                pto.setState(false);
            }
            else if (time - engageTime < 100)
            {
                systemMotors.move(0);
            }
            else
            {
                pto.setState(false);
                systemMotors.move(-60);
                if (systemMotors.get_position() < -20)
                {
                    liftDown = true;
                }
            }
        }
        else if (reverse)
        {
            if (!pto.getState())
            {
                engageTime = time;
                pto.setState(true);
            }
            else if (time - engageTime < 100)
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
            else if (time - engageTime < 100)
            {
                systemMotors.move(0);
            }
            else
            {
                systemMotors.move(-127);
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

        if (B_HELD)
        {
            matchLoad.setState(true);
        }
        else
        {
            matchLoad.setState(false);
        }

        pros::delay(10);
    }
}