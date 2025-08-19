#include "globals.hpp"
#include "AutonSelector.hpp"
#include "Autonomous_Paths.hpp"
#include "utils.cpp"

using namespace pros;

void on_center_button() {}

void initialize()
{
    bool debug = false;
    if (debug)
    {
        debugOdom();
    }
    else
    {
        autonSelect.setAutons(std::vector<autonomousRoute>{
            autonomousRoute{"red", "Left", "Position: Left", left},
            autonomousRoute{"red", "Right", "Position: Right", right},
            autonomousRoute{"blue", "Left", "Position: Left", left},
            autonomousRoute{"blue", "Right", "Position: Right", right}});
        autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills});

        pros::delay(500);
        autonSelect.start();
    }

    chassis.calibrate();
    master.clear();
    chassis.setPose(0, 0, 0);
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    colourSort::redTeam = autonSelect.isRedTeam();
    autonSelect.runAuton();
}

void opcontrol()
{

    while (true)
    {

        pros::delay(20);
    }
}