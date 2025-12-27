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
		autonomousRoute{"red", "Left2G", "Position: Left, facing up", left},
		autonomousRoute{"red", "Right2G", "Position: Right, facing up", right},
		autonomousRoute{"red", "Left1G", "Position: Left, facing up", left2},
		autonomousRoute{"red", "Right1G", "Position: Right, facing up", right2},
		autonomousRoute{"blue", "Left2G", "Position: Left, facing up", left},
		autonomousRoute{"blue", "Right2G", "Position: Right, facing up", right},
		autonomousRoute{"blue", "Left1G", "Position: Left, facing up", left2},
		autonomousRoute{"blue", "Right1G", "Position: Right, facing up", right2},
		autonomousRoute{"blue", "SoloAWP", "Position: Right, facing right", soloAWP},
		autonomousRoute{"red", "SoloAWP", "Position: Right, facing right", soloAWP},
		autonomousRoute{"blue", "HalfSolo", "Position: Right, facing right", halfSAWPRight},
		autonomousRoute{"red", "HalfSolo", "Position: Right, facing right", halfSAWPRight},
		autonomousRoute{"blue", "HalfSolo", "Position: Left, facing right", halfSAWPLeft},
		autonomousRoute{"red", "HalfSolo", "Position: Left, facing right", halfSAWPLeft},
		autonomousRoute{"blue", "DriveOff", "Position: literally anywhere 😭", test},
		autonomousRoute{"red", "DriveOff", "Position: literally anywhere 😭", test},
	});
	autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", riskySkillsV2});
	robot.init();
	// autonSelect.start();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

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
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	// right();
	soloAWP();
	// autonSelect.runAuton();
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
	robot.lift(false);
	while (true)
	{
		robot.teleopControl();
		pros::delay(20); // Run for 20 ms then update
	}
}