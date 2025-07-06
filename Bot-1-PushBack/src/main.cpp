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
        autonomousRoute{"blue", "Blue Auton 1", "1st Blue Auton", blueSAWP},
        autonomousRoute{"blue", "Blue Auton 2", "2nd Blue Auton", blueAuton2}});
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", exampleAuton});
    autonSelect.start();
    // pros::Task colourSort(colourSort::start);
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
        // double leftY; if (master.get_digital(buttons::DOWN)) { if (leftY * 1.5 > 127) { leftY = 127;} else if (leftY == 0) {leftY = 5;}else{ leftY += abs(leftY * 0.5);}}else if (master.get_digital(buttons::B)) {if (leftY * 1.5 < -127){leftY = -127;} else if (leftY == 0) {leftY = -5;}else{leftY -= abs(leftY * 0.5);}}else {if (leftY * 0.5 < 10) {leftY = 0;} else {leftY *= 0.5;}}



        // angular awr
        // double tot = 0;
        // for (double i = 9.99; i <= 180; i += 10) {
        // 	double target = chassis.getPose().theta + i;
        // 	chassis.turnToHeading(target, 1500);
        // 	delay(2000);
        // 	tot += target - chassis.getPose().theta;
        // }

        // delay(500);
        // master.print(1, 0, "%.5f", tot);
        // lcd::print(6, 0, "%.5f", tot);
        // delay(3000);

        // int tar = 180;
        // chassis.turnToHeading(tar, 3000);
        // delay(2500);
        // master.print(1, 0, "%.3f", tar - chassis.getPose().theta);
        // delay(3000);

        // lateral awr
        // double tot = 0;
        // for (double i = 8; i <= 40; i += 8) {
        // 	double target = chassis.getPose().y + i;
        // 	chassis.moveToPoint(0, target, 4000);
        // 	delay(4050);
        // 	master.print(0, 0, "%f", target-chassis.getPose().y);
        // 	tot += target-chassis.getPose().y;
        // }
        // master.print(1, 0, "%.2f", tot);
        // delay(5000);

        // double tar = 24;
        // int time = 4000;
        // chassis.moveToPoint(0, tar, time);
        // delay(time+100);
        // master.print(1, 0, "%.2f", tar-chassis.getPose().y);
        // delay(100);
        // master.print(2, 0, "%.2f", chassis.getPose().y);

        // chassis.turnToHeading(90, 1500);
        // delay(2000);
        // master.print(1, 0, "%.2f", chassis.getPose().theta);
        // delay(1000);

        // int tar = 48;
        // chassis.moveToPoint(0, tar, 2000);
        // delay(2000);
        // master.print(1, 0, "%.2f", tar - chassis.getPose().y);
        // delay(1000);
        double rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        double leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        chassis.arcade(leftY, rightX);
        if (master.get_digital_new_press(buttons::Y))
        {
            colourSort::redTeam = !colourSort::redTeam;
        }
        if (master.get_digital_new_press(buttons::UP))
        {
            colourSort::on = !colourSort::on;
        }
        if (master.get_digital(buttons::B))
        {
            matchLoader.setState(true);
        } else {
            if (matchLoader.getState()) matchLoader.setState(false);
        }
        if (master.get_digital_new_press(buttons::DOWN))
        {
            park.toggle();
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