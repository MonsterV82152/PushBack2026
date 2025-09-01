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
    robot.mapButtons({L1, buttons::R2, true});
    robot.mapButtons({L2, buttons::L1, true});
    robot.mapButtons({L3, buttons::L2, true});
    robot.mapButtons({DESCORE, buttons::DOWN, true, false, buttons::A, 2});
    // robot.mapButtons({BACKL2, buttons::L1, true});
    // robot.mapButtons({BACKL3, buttons::L2, true});
    while (true)
    {
        chassis.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), false, 0.54);
        robot.driverControl();
        if (robot.getRollerState() == INTAKE && front.get_actual_velocity() < 3) {
            robot.setState(INTAKE2);
        } else if (robot.getRollerState() == INTAKE2 && back.get_actual_velocity() < 195) {
            robot.setState(INTAKE3);
        }
        if (master.get_digital(buttons::B)) {
            robot.matchLoad(true);
        } else {
            robot.matchLoad(false);
        }
        if (master.get_digital_new_press(buttons::X)) {
            park.toggle();
        }
        if (master.get_digital_new_press(buttons::R1)) {
            if (robot.getRollerState() == INTAKE || robot.getRollerState() == INTAKE2 || robot.getRollerState() == INTAKE3) 
                robot.setState(STOP);
            else robot.setState(INTAKE);
        }
        
        // if (master.get_digital_new_press(buttons::R1)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(L1);
        //     else roller.setState(INTAKE);
        // }
        // if (master.get_digital_new_press(buttons::L2)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(BACKL3);
        //     else roller.setState(L3);
        // }
        // if (master.get_digital_new_press(buttons::L1)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(BACKL2);
        //     else roller.setState(L2);
        // }
        // if (master.get_digital_new_press(buttons::A)) {
        //     roller.setState(STOP);
        // }

        

        pros::delay(20);
    }
}