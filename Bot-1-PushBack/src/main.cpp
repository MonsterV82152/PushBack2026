#include "globals.hpp"
#include "AutonSelector.hpp"
#include "Autonomous_Paths.hpp"
#include "movements.cpp"

void on_center_button() {}

void initialize()
{
    pros::delay(1000);
    autonSelect.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "Red SAWP", "Solo AWP", redSAWP},
        autonomousRoute{"red", "Red Auton 2", "2nd Red Auton", redAuton2},
        autonomousRoute{"blue", "Blue Auton 1", "1st Blue Auton", blueAuton1},
        autonomousRoute{"blue", "Blue Auton 2", "2nd Blue Auton", blueAuton2}});
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", exampleAuton});
    autonSelect.start();
    pros::Task colourSort(colourSort::start);
    // pros::Task screen_task([&]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
    //         pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //         pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
    //         // delay to save resources
    //         pros::delay(20);
    //     }
    // });

    // chassis.calibrate();
    // chassis.setPose(0,0,0);
}

void disabled() {}

void competition_initialize() {}

void autonomous()
{
    exampleAuton();
}

void opcontrol()
{
    colourSort::redTeam = autonSelect.isRedTeam();
    while (true)
    {
        double rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        double leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        chassis.arcade(leftY, rightX);
        if (master.get_digital_new_press(buttons::Y)) {
            colourSort::redTeam = !colourSort::redTeam;
        }
        if (master.get_digital_new_press(buttons::R1))
        {
            if (rollers::state.name == "intake")
            {
                rollers::stop();
            }
            else
            {
                rollers::intake();
            }
        }
        if (master.get_digital_new_press(buttons::A))
        {
            rollers::addTemporaryState("cycle", 7);
        }
        else if (rollers::currentTemporaryState.name == "cycle" && !master.get_digital(buttons::A))
        {
            rollers::removeTemporaryState("cycle");
        }
        if (master.get_digital_new_press(buttons::X))
        {
            rollers::addTemporaryState(rollers::rollerState{"middleIntake", 0, 127, 127, 0}, 7);
        }
        else if (rollers::currentTemporaryState.name == "middleIntake" && !master.get_digital(buttons::X))
        {
            rollers::removeTemporaryState("middleIntake");
        }
        if (master.get_digital_new_press(buttons::R2))
        {
            rollers::addTemporaryState("scoreBottom", 7);
        }
        else if (rollers::currentTemporaryState.name == "scoreBottom" && !master.get_digital(buttons::R2))
        {
            rollers::removeTemporaryState("scoreBottom");
        }
        if (master.get_digital_new_press(buttons::L2))
        {
            rollers::addTemporaryState("scoreTop", 7);
        }
        else if (rollers::currentTemporaryState.name == "scoreTop" && !master.get_digital(buttons::L2))
        {
            rollers::removeTemporaryState("scoreTop");
        }
        if (master.get_digital_new_press(buttons::L1))
        {
            rollers::addTemporaryState("scoreMiddle", 7);
        }
        else if (rollers::currentTemporaryState.name == "scoreMiddle" && !master.get_digital(buttons::L1))
        {
            rollers::removeTemporaryState("scoreMiddle");
        }

        // chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}