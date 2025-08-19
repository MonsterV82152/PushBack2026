#include "globals.hpp"
#include "AutonSelector.hpp"
#include "Autonomous_Paths.hpp"
#include "movements.cpp"

using namespace pros;

void on_center_button() {}

void initialize()
{
    // pros::lcd::initialize();
    topColour.set_integration_time(40);
    bottomColor.set_integration_time(40);
    autonSelect.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "Solo AWP", "Position: Right \n Total: 3 goals, 7~10 blocks,  3-3-4", SAWP},
        autonomousRoute{"red", "Left", "Position: Left \n Total: 1 goal, 7 blocks", Left},
        autonomousRoute{"red", "Right", "Position: Right \n Total: 1 goal, 7 blocks", Right},
        autonomousRoute{"red", "Right 2 Goals", "Position: Right \n Total: 1 goal, 7 blocks", Right2Goals},
        autonomousRoute{"red", "Left 2 Goals", "Position: Left \n Total: 1 goal, 7 blocks", Left2Goals},
        autonomousRoute{"blue", "Solo AWP", "Position: Right \n Total: 3 goals, 7~10 blocks,  3-3-4", SAWP},
        autonomousRoute{"blue", "Left", "Position: Left \n Total: 1 goal, 7 blocks", Left},
        autonomousRoute{"blue", "Right", "Position: Right \n Total: 1 goal, 7 blocks", Right},
        autonomousRoute{"blue", "Right 2 Goals", "Position: Right \n Total: 1 goal, 7 blocks", Right2Goals},
        autonomousRoute{"blue", "Left 2 Goals", "Position: Left \n Total: 1 goal, 7 blocks", Left2Goals}});
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills});
    pros::delay(500);
    autonSelect.start();
    chassis.setBrakeMode(coast);

    // pros::lcd::initialize();
    // pros::Task screen_task([&]()
    //                        {
    //     while (true) {
    //         // print robot location to the brain screen
    //         pros::lcd::print(0, "X: %f    Theta: %f", chassis.getPose().x, chassis.getPose().theta); // x
    //         pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //         pros::lcd::print(2, "Front: %d    Right: %d", localization::frontDS.get_distance(), localization::rightDS.get_distance()); // heading
    //         pros::lcd::print(3, "Left: %d    Left: %d", localization::leftDS.get_distance(), localization::backDS.get_distance()); // heading
    //         // delay to save resources
    //         pros::delay(20);
    //     } });

    chassis.calibrate();
    master.clear();

    pros::Task colourSortThread(colourSort::start); chassis.setPose(0, 0, 0);
}
void disabled() {}
void competition_initialize() {}
void autonomous()
{
    // exampleAuton();
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, 24, 10000);
    colourSort::redTeam = autonSelect.isRedTeam();
    autonSelect.runAuton();
    // redAuton2();
    // SAWP();
    // SAWP();
    // Left2Goals();
    // skills();
    // Left();
    // Left2Goals();
    // example();
}
void opcontrol()
{
    int teamSequence[6] = {1, 0, 2, 0, 2, 1};

    // skills();
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

    // int tar = 90;
    // chassis.turnToHeading(tar, 3000);
    // delay(2500);
    // master.print(1, 0, "%.3f", tar - chassis.getPose().theta);
    // delay(3000);

    // lateral awr
    // double tot = 0;
    // for (double i = 48; i <= 48; i += 8) {
    // 	double target = chassis.getPose().y + i;
    // 	chassis.moveToPoint(0, target, 4000);
    // 	pros::delay(4050);
    // 	master.print(0, 0, "%f", target-chassis.getPose().y);
    // 	tot += target-chassis.getPose().y;
    // }
    // pros::delay(1000);
    // master.print(1, 0, "tot: %.2f", tot);
    // pros::delay(5000);

    // double tar = 48;
    // int time = 4000;
    // chassis.moveToPoint(0, tar, time);
    // pros::delay(time+100);
    // master.print(1, 0, "%.2f", tar-chassis.getPose().y);
    // pros::delay(100);
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
    int teamIndex = 0;
    if (autonSelect.isSkills())
    {
        colourSort::redTeam = false;
        colourSort::on = false;
    } else {
        colourSort::redTeam = autonSelect.isRedTeam();
        colourSort::on = true;
    }

    while (true)
    {
        // double leftY; if (master.get_digital(buttons::DOWN)) { if (leftY * 1.5 > 127) { leftY = 127;} else if (leftY == 0) {leftY = 5;}else{ leftY += abs(leftY * 0.5);}}else if (master.get_digital(buttons::B)) {if (leftY * 1.5 < -127){leftY = -127;} else if (leftY == 0) {leftY = -5;}else{leftY -= abs(leftY * 0.5);}}else {if (leftY * 0.5 < 10) {leftY = 0;} else {leftY *= 0.5;}}
        double rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        double leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        chassis.arcade(leftY, rightX, false, 0.54);
        if (!programmerMode)
        {
            if (master.get_digital_new_press(buttons::Y))
            {
                if (teamSequence[teamIndex] == 1)
                {
                    colourSort::on = true;
                    colourSort::redTeam = false;
                    master.print(0, 0, colourSort::on ? colourSort::redTeam ? "red " : "blue" : "off ");
                }
                else if (teamSequence[teamIndex] == 2)
                {
                    colourSort::on = true;
                    colourSort::redTeam = true;
                    master.print(0, 0, colourSort::on ? colourSort::redTeam ? "red " : "blue" : "off ");
                }
                else
                {
                    colourSort::on = false;
                    master.print(0, 0, colourSort::on ? colourSort::redTeam ? "red " : "blue" : "off ");
                }
                if (teamIndex < 6)
                {
                    teamIndex++;
                }
                else
                {
                    teamIndex = 0;
                }
            }
            if (master.get_digital(buttons::B))
            {
                matchLoader.setState(true);
            }
            else
            {
                if (matchLoader.getState())
                    matchLoader.setState(false);
            }

            if (master.get_digital_new_press(buttons::UP))
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
                    rollers::setState("intake");
                }
            }
            if (master.get_digital_new_press(buttons::DOWN)) { rollers::addTemporaryState("cycle", 7);}
            else if (rollers::currentTemporaryState.name == "cycle" && !master.get_digital(buttons::DOWN))
            {
                rollers::removeTemporaryState("cycle");
            }
            if (master.get_digital_new_press(buttons::X))
            {
                rollers::addTemporaryState("clearIntake", 7);
            }
            else if (rollers::currentTemporaryState.name == "clearIntake" && !master.get_digital(buttons::X))
            {
                rollers::removeTemporaryState("clearIntake");
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
                rollers::rollerStates[5].middleSpeed = -90; // Adjust middle speed for scoreMiddle
                rollers::rollerStates[5].bottomSpeed = 90;  // Adjust bottom speed for scoreMiddle
                rollers::addTemporaryState("scoreMiddle", 7);
            }
            else if (master.get_digital(buttons::L1))
            {
                if (rollers::rollerStates[5].middleSpeed < -40)
                {
                    rollers::rollerStates[5].middleSpeed += 1; // Increment middle speed
                    rollers::rollerStates[5].bottomSpeed -= 1; // Decrement bottom speed
                    rollers::addTemporaryState("scoreMiddle", 7);
                    rollers::removeTemporaryState("scoreMiddle");
                }
            }
            else if (rollers::currentTemporaryState.name == "scoreMiddle" && !master.get_digital(buttons::L1))
            {
                rollers::removeTemporaryState("scoreMiddle");
            }
        }
        else
        {
            if (master.get_digital_new_press(buttons::UP))
            {
                correct_position(frontLoc, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::RIGHT))
            {
                correct_position(rightLoc, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::LEFT))
            {
                correct_position(leftLoc, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::DOWN))
            {
                correct_position(backLoc, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::X))
            {
                correct_position(frontLoc, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::A))
            {
                correct_position(rightLoc, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::Y))
            {
                correct_position(leftLoc, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::B))
            {
                correct_position(backLoc, &chassis, true);
            }
        }

        // chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}