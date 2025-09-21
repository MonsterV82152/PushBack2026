// using namespace pros;

#include "globals.hpp"

void on_center_button() {}

void initialize()
{
    mcl.calibrate();
    mcl.setPose(Pose2D(48, 48, 0));
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    // chassis.calibrate();
}

void opcontrol()
{
    while (true)
    {
        // Pose2D pose = mcl.getPose();
        // pros::lcd::print(0, "X: %.2f", pose.x);
        // pros::lcd::print(1, "Y: %.2f", pose.y);
        // pros::lcd::print(2, "Theta: %.2f", pose.theta);
        pros::delay(20);
        int throttle = master.get_analog(ANALOG_LEFT_Y);
        int turn = master.get_analog(ANALOG_RIGHT_X);
        leftDT.move(throttle + turn);
        rightDT.move(throttle - turn);
    }
}