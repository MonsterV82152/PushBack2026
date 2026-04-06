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

    pto.setState(false);
    matchLoader.setState(false);
    intakeLift.setState(false);
    lift.setState(false);
    wing.setState(false);
    hood.setState(true);
    lever.setState(false);
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
        moveDT(LEFT_Y, RIGHT_X);

        if (R1_NEW_PRESS) {
            if (intakeState != 1) {
                intake(127);
                intakeState = 1;
            }
            else {
                intake(0);
                intakeState = 0;
            }
        }
        if (R2_NEW_PRESS) {
            prevIntakeSpeed = intakeState == 1 ? 127 : 0; // Store current speed before toggling
            if (intakeState != -1) {
                intake(-127);
                intakeState = -1;
            }
            else {
                intake(0);
                intakeState = 0;
            }
        }
        if (R2_RELEASED) {
            intake(prevIntakeSpeed); // Restore previous speed on release
            intakeState = prevIntakeSpeed == 127 ? 1 : 0; // Update state based on previous speed
        }

        pros::delay(20);
    }
}