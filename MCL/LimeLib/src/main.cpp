// using namespace pros;

#include "globals.hpp"
#include <iostream>

void on_center_button() {}

void initialize()
{

    pros::lcd::initialize();
    mcl.calibrate();
    master.rumble("..");
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    chassis.setPose(-48, 24, 0);
    chassis.moveToPoint(-48, 48, 10000);
}

void opcontrol()
{
    chassis.setPose(-48, 24, 0);
    while (true)
    {
        if (master.get_digital_new_press(buttons::A))
        {
            Pose2D pose = mcl.getPose();
            std::cout << "Current Pose: (" << pose.x << ", " << pose.y << ", " << pose.theta << ")\n";
        }
        pros::delay(20);
        int throttle = master.get_analog(ANALOG_LEFT_Y);
        int turn = master.get_analog(ANALOG_RIGHT_X);
        leftDT.move(throttle + turn);
        rightDT.move(throttle - turn);
    }
}