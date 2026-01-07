#include "includes.hpp"
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	autonSelect.setAutons(std::vector<autonomousRoute>{
		autonomousRoute{"red", "Left1G", "Position: Left, facing up", left},
		autonomousRoute{"red", "Right1G", "Position: Right, facing up", right},
		autonomousRoute{"red", "Left2G", "Position: Left, facing up", left2},
		autonomousRoute{"red", "Right2G", "Position: Right, facing up", right2},
		autonomousRoute{"blue", "Left1G", "Position: Left, facing up", left},
		autonomousRoute{"blue", "Right1G", "Position: Right, facing up", right},
		autonomousRoute{"blue", "Left2G", "Position: Left, facing up", left2},
		autonomousRoute{"blue", "Right2G", "Position: Right, facing up", right2},
		autonomousRoute{"blue", "SoloAWP", "Position: Right, facing right", soloAWP},
		autonomousRoute{"red", "SoloAWP", "Position: Right, facing right", soloAWP},
		autonomousRoute{"blue", "DriveOff", "Position: literally anywhere 😭", test},
		autonomousRoute{"red", "DriveOff", "Position: literally anywhere 😭", test},
	});
	autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills2});
	robot.init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
	robot.disabled();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be st  opped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	// soloAWP();
	// left();
	// left();
	// soloAWP();
	left2();
	// skills2();
	// autonSelect.runAuton();
	// skills2();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
		skills2();
	robot.setPose(50, 50, 0);
	robot.lift(false);
	robot.reset();
	if (helper.autonSelector.isSkills() && master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
	{
		robot.intake();
		move(40);
		pros::delay(500);
		move(-30);
		pros::delay(200);
		move(30);
		pros::delay(500);
		move(-50);
		pros::delay(400);
		move(70);
		pros::delay(500);
		move(-30);
		pros::delay(200);
		move(30);
		pros::delay(500);
		move(-30);
		pros::delay(200);
		move(30);
		pros::delay(500);
		move(-80);
		robot.matchLoad(true);
		pros::delay(700);
		robot.matchLoad(false);
	}
	while (true)
	{
		robot.teleopControl();
		// robot.debug();
		// Pose2D pose = locator.getPose();
		// pros::lcd::print(3, "X: %.2f Y: %.2f T: %.2f", pose.x, pose.y, pose.theta);

		// if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
		// {
		// 	correct_position(frontDistanceSensor, &locator, false, true);
		// }
		// if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
		// {
		// 	correct_position(backDistanceSensor, &locator, false, true);
		// }
		// if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
		// {
		// 	correct_position(leftDistanceSensor, &locator, true, true);
		// }
		// if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
		// {
		// 	correct_position(rightDistanceSensor, &locator, true, true);
		// }
		pros::delay(20); // Run for 20 ms then update
	}
}