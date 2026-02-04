#include "includes.hpp"

using namespace pros;

void on_center_button() {}

void initialize()
{

    autonSelect.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "Left", "Position: Left", left},
        autonomousRoute{"red", "Right", "Position: Right", right},
        autonomousRoute{"blue", "Left", "Position: Left", left},
        autonomousRoute{"blue", "Right", "Position: Right", right}});
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills});

    pros::delay(500);
    autonSelect.start();

    chassis.calibrate();
    master.clear();
    chassis.setPose(0, 0, 0);
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    autonSelect.runAuton();
}

void opcontrol()
{
    while (true)
    {
        pros::delay(20);
    }
}