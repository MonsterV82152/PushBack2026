#include "main.h"
#include "robot_commands.hpp"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button()
{
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	CommandScheduler::start(); // Start the command scheduler
	pros::lcd::initialize();
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
void autonomous() {}

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
	CommandScheduler::setDefaultCommand(new JoystickDrive(), DRIVETRAIN_ID);
	while (true)
	{
		pros::lcd::clear_line(1);
		pros::lcd::print(1, "%d", potentiometer.get_value());

		if (R1_NEW_PRESS)
		{
			RobotStates::shouldBeIntaking = !RobotStates::shouldBeIntaking;
			if (RobotStates::currentState != RobotState::SCORING)
			{
				if (RobotStates::shouldBeIntaking)
					CommandScheduler::scheduleCommand(intake());
				else
					CommandScheduler::scheduleCommand(new StopIntake());
			}
		}
		if (R2_NEW_PRESS)
		{
			if (RobotStates::stickState == RobotState::INTAKING)
			{
				CommandScheduler::scheduleCommand(reverseIntake());
			}
		}
		if (R2_RELEASED)
		{
			if (RobotStates::stickState == RobotState::INTAKING && RobotStates::shouldBeIntaking)
			{
				CommandScheduler::scheduleCommand(intake());
			}
			else if (RobotStates::stickState == RobotState::INTAKING)
			{
				CommandScheduler::scheduleCommand(new StopIntake());
			}
		}
		if (L1_NEW_PRESS)
		{
			CommandScheduler::scheduleCommand(score(matchScoreFunction));
		}
		if (L1_RELEASED)
		{
			if (R2_HELD)
			{
				CommandScheduler::scheduleCommand(reverseIntake());
			}
			else if (RobotStates::shouldBeIntaking)
			{
				CommandScheduler::scheduleCommand(intake());
			}
			else
			{
				CommandScheduler::scheduleCommand(lowerScoring());
			}
		}

		pros::delay(20); // Run for 20 ms then update
	}
}