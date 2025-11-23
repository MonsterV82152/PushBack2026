/**
 * @file main.cpp
 * @brief Main control file for the PushBack2026 robot
 *
 * This file contains the core control logic for the VEX robotics competition bot,
 * including initialization, autonomous mode, driver control, and color sorting
 * functionality for ring management.
 *
 * @details The robot features:
 * - Arcade drive control with variable brake modes
 * - Color sorting system to reject enemy rings
 * - Multi-level scoring mechanisms (L1, L2, L3 lifts)
 * - Back lift for additional scoring options
 * - Autonomous route selection and execution
 * - Programmer mode for development/testing
 */

#include "includes.hpp"

using namespace pros;

/// Flag to track if an intake task is currently scheduled
bool intakeTask = false;

/// Flag to indicate if color sorting is currently active
bool colourSorting = false;

/**
 * @struct timeouts
 * @brief Structure for scheduling delayed function execution
 *
 * Used to queue functions that should execute at a specific timestamp,
 * enabling precise timing for complex multi-step mechanical sequences.
 */
struct timeouts
{
    int timestamp;      ///< The timestamp (ms since program start) when the function should execute
    void (*function)(); ///< Function pointer to the callback to execute
};

/**
 * @brief Background task handling color sorting and intake automation
 *
 * This task runs continuously during driver control and manages:
 * - Color sorting logic to detect and eject enemy-colored rings
 * - Intake state machine transitions (INTAKE -> INTAKE2 -> INTAKE3)
 * - Button press handling for lift controls (L1/L2/L3 and back lift)
 * - Parking mechanism activation
 * - Delayed function execution via timeout queue
 *
 * @param params Unused parameter (required by PROS task signature)
 *
 * @details Color sorting works by:
 * 1. Reading hue and proximity from the middle color sensor
 * 2. Checking if ring color matches the opposing team
 * 3. Activating COLOURSORT state to eject the ring
 * 4. Operating on both teams based on isRedTeam flag
 */
void colourSort(void *params)
{

    bool red = false;
    bool blue = false;
    int currentTime = 0;
    std::vector<timeouts> timeouts = {};
    while (true)
    {
        currentTime = pros::millis();
        // Process and execute any scheduled timeouts
        for (int i = 0; i < timeouts.size(); i++)
        {
            if (timeouts[i].timestamp <= currentTime)
            {
                timeouts[i].function();
                timeouts.erase(timeouts.begin() + i);
                i--;
            }
        }

        // Read color sensor values
        hue = middleCS.get_hue();
        int distance = middleCS.get_proximity();

        // Determine if sensed ring is red or blue
        red = (hue > redMin || hue < redMax) && distance < 100;
        blue = (hue > blueMin && hue < blueMax) && distance < 100;

        // Automated intake state progression
        // Transition from INTAKE to INTAKE2 when ring is secured
        if (!intakeTask && robot.getRollerState() == INTAKE && frontDS.get_distance() < 30 && front.get_actual_velocity() < 30)
        {
            intakeTask = true;
            timeouts.push_back({currentTime + 300, []()
                                { robot.setState(INTAKE2); intakeTask = false; }});
        }
        // Transition from INTAKE2 to INTAKE3 when ring velocity stabilizes
        else if (robot.getRollerState() == INTAKE2 && middle.get_actual_velocity() < 20)
        {
            robot.setState(INTAKE3);
        }

        // Color sorting - eject enemy-colored rings
        if (colourSortOn)
        {
            if ((!isRedTeam && red) || (isRedTeam && blue))
            {
                colourSorting = true;
                robot.addTempState(COLOURSORT, 10);
                pros::delay(300);
                robot.removeTempState(COLOURSORT);
                colourSorting = false;
            }
        }

        // LEFT BUTTON: Activate parking mechanism
        if (master.get_digital_new_press(buttons::LEFT))
        {
            chassis.setBrakeMode(brake);
            brakeChassis = true;
            robot.setState(PARK);
            park.setState(false);
            // Wait for intake to raise
            while (intakeDS.get_distance() > 50)
            {
                pros::delay(10);
            }
            robot.setState(PARK2);
            // Wait for intake to lower
            while (intakeDS.get_distance() < 50)
            {
                pros::delay(10);
            }
            pros::delay(400);
            robot.stop();
            park.setState(true);
        }

        // L1 BUTTON: Activate L2 lift (with helper state for smooth motion)
        if (master.get_digital_new_press(buttons::L1))
        {
            master.rumble(".");
            robot.addTempState(L2HELPER, 1);
            if (autonSelect.isSkills())
            {
                timeouts.push_back({currentTime + 200, []()
                                    { if (robot.getRollerState() == L2HELPER) robot.removeTempState(L2HELPER); robot.addTempState(L2SKILLS, 1); }});
            }
            else
            {
                timeouts.push_back({currentTime + 200, []()
                                    { if (robot.getRollerState() == L2HELPER) robot.removeTempState(L2HELPER); robot.addTempState(L2, 1); }});
            }
        }

        // L2 BUTTON: Activate L3 lift
        if (master.get_digital_new_press(buttons::L2))
        {
            if (autonSelect.isSkills())
            {
                robot.addTempState(L3SKILLS, 1);
            }
            else
            {
                robot.addTempState(L3, 1);
            }
        }

        // RIGHT BUTTON: Activate back L3 lift (with helper state)
        if (master.get_digital_new_press(buttons::RIGHT))
        {
            robot.addTempState(BACKL3HELPER, 1);
            timeouts.push_back({currentTime + 200, []()
                                { if (robot.getRollerState() == BACKL3HELPER) robot.removeTempState(BACKL3HELPER); robot.addTempState(BACKL3, 1); }});
        }

        // RIGHT BUTTON RELEASE: Deactivate back L3 and return to intake if applicable
        if (!master.get_digital(buttons::RIGHT) && !master.get_digital_new_press(buttons::RIGHT))
        {
            if (robot.getRollerState() == BACKL3)
            {
                robot.removeTempState(BACKL3);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == BACKL3HELPER)
            {
                robot.removeTempState(BACKL3HELPER);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }

        // L2 BUTTON RELEASE: Deactivate L3 and return to intake if applicable
        if (!master.get_digital(buttons::L2) && !master.get_digital_new_press(buttons::L2))
        {
            if (robot.getRollerState() == L3)
            {
                robot.removeTempState(L3);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L3SKILLS)
            {
                robot.removeTempState(L3SKILLS);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }

        // L1 BUTTON RELEASE: Deactivate L2/L2SKILLS and return to intake if applicable
        if (!master.get_digital(buttons::L1) && !master.get_digital_new_press(buttons::L1))
        {
            if (robot.getRollerState() == L2)
            {
                robot.removeTempState(L2);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L2HELPER)
            {
                robot.removeTempState(L2HELPER);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L2SKILLS)
            {
                robot.removeTempState(L2SKILLS);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }

        pros::delay(5);
    }
}

/// @brief Placeholder callback for center brain button (currently unused)
void on_center_button() {}

/**
 * @brief Initialize robot systems and configure autonomous mode
 *
 * Sets up the robot for competition by:
 * - Registering all autonomous route options (red/blue, left/right positions)
 * - Configuring the skills-only autonomous routine
 * - Setting intake and color sensor parameters
 * - Starting the color sorting background task
 * - Initializing the chassis odometry and pose
 * - Setting initial robot state to DESCORE
 *
 * @details In programmer mode, displays real-time chassis pose (X, Y, Theta)
 * on the brain screen for debugging and calibration purposes.
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

    pros::delay(500);
    intake.set_brake_mode(brake);
    middleCS.set_integration_time(5);
    if (!programmerMode)
    {
        autonSelect.start();
        pros::Task colourSortTask(colourSort);
    }
    else
    {
        pros::lcd::initialize();
        // Display live odometry data for debugging
        pros::Task screen_task([&]()
                               {
            while (true) {
                pros::lcd::print(0, "X: %f", chassis.getPose().x);
                pros::lcd::print(1, "Y: %f", chassis.getPose().y);
                pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);
                pros::delay(20);
            } });
    }

    chassis.calibrate();
    master.clear();
    chassis.setPose(0, 0, 0);
    robot.setState(DESCORE);
}

/// @brief Called when robot is disabled (currently unused)
void disabled() {}

/// @brief Competition initialization callback (currently unused)
void competition_initialize() {}

/**
 * @brief Autonomous mode execution
 *
 * Determines the team color from the selected autonomous routine and
 * executes the pre-programmed autonomous path. The selected routine was
 * chosen during the initialize() phase via the autonomous selector menu.
 *
 * @details Team color (red/blue) determines:
 * - Color sorting preferences during autonomous
 * - Which rings are considered "ally" vs "enemy"
 * - Display on brain screen
 *
 * The remainder of the function contains commented-out calibration code
 * for tuning drivetrain movement and odometry accuracy (angular/lateral).
 */
void autonomous()
{
    middleCS.set_led_pwm(100);
    isRedTeam = autonSelect.isRedTeam();
    if (isRedTeam)
    {
        master.print(1, 1, "Red");
    }
    else
    {
        master.print(1, 1, "Blue");
    }
    autonSelect.runAuton();
    // riskySkillsV2();
    // soloAWP2();

    // [CALIBRATION CODE - COMMENTED OUT]
    // The following sections were used to tune:
    // 1. Angular wheel radius (awr) - turn accuracy
    // 2. Lateral wheel radius (awr) - forward/backward accuracy
    // 3. Odom pose accuracy at various distances
    // Uncomment and run individual tests as needed for recalibration
}

/**
 * @brief Operator control mode (driver control period)
 *
 * Handles all driver input and robot control during the operator control phase.
 * Supports two modes:
 * - **Normal Mode**: Full competition control with color sorting and autonomous state selection
 * - **Programmer Mode**: Development mode with odometry position correction tools
 *
 * @details **Normal Mode Controls**:
 * - Left Stick Y: Forward/backward movement (arcade drive)
 * - Right Stick X: Rotational movement (arcade drive)
 * - Button A: Toggle chassis brake mode (coast/brake)
 * - Button Y: Cycle color sorting (Off → Red Team → Blue Team)
 * - Button B (held): Activate match load mechanism
 * - Button R1 (pressed): Toggle intake on/off
 * - Button UP (pressed): Toggle parking mechanism
 * - Buttons L1/L2/R2: Automatic lift control (mapped to robot states)
 *
 * **Programmer Mode Controls** (for wheel calibration):
 * - Arcade drive with same stick controls
 * - Direction buttons activate position correction for individual wheels:
 *   - UP: Left Front (LF) wheel, forward
 *   - DOWN: Left Back (LB) wheel, reverse
 *   - LEFT: Left Back (LL) wheel, reverse
 *   - RIGHT: Left Rear (LR) wheel, reverse
 *   - X: Left Front (LF) wheel, reverse
 *   - A: Left Rear (LR) wheel, forward
 *   - Y: Left Back (LL) wheel, forward
 *   - B: Left Back (LB) wheel, forward
 */
void opcontrol()
{
    double batteryLevel = pros::battery::get_capacity();
    colourSortOn = false;
    robot.setState(STOP);
    if (!programmerMode)
    {
        middleCS.set_led_pwm(100);
        std::vector<int> teamColorQueue = {0, 1, 2};
        short teamColorIndex = 0;
        isRedTeam = autonSelect.isRedTeam();
        if (autonSelect.isSkills())
        {
            robot.mapButtons({L1SKILLS, buttons::R2, true});
        }
        else
        {
            robot.mapButtons({L1, buttons::R2, true});
        }
        robot.mapButtons({L3, buttons::L2, true});
        robot.mapButtons({DESCORE, buttons::DOWN, true});
        robot.mapButtons({BACKINTAKE, buttons::X, false});
        while (true)
        {
            // Arcade drive control with 54% power scaling
            chassis.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), false, 0.54);
            robot.driverControl();

            // Toggle brake mode
            if (master.get_digital_new_press(buttons::A))
            {
                brakeChassis = !brakeChassis;
                if (brakeChassis)
                {
                    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
                }
                else
                {
                    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
                }
            }

            // Cycle team color and color sorting (Off → Red → Blue)
            if (master.get_digital_new_press(buttons::Y))
            {
                teamColorIndex++;
                teamColorIndex %= 3;
                if (teamColorIndex == 0)
                {
                    colourSortOn = false;
                    master.print(1, 1, "Off %f", batteryLevel);
                }
                else if (teamColorIndex == 1)
                {
                    isRedTeam = true;
                    colourSortOn = true;
                    master.print(1, 1, "Red %f", batteryLevel);
                }
                else if (teamColorIndex == 2)
                {
                    isRedTeam = false;
                    colourSortOn = true;
                    master.print(1, 1, "Blue%f", batteryLevel);
                }
            }

            // Match load mechanism (held)
            if (master.get_digital(buttons::B))
            {
                robot.matchLoad(true, false);
            }
            else
            {
                robot.matchLoad(false);
            }

            // Toggle intake on/off
            if (master.get_digital_new_press(buttons::R1))
            {
                robot.toggleIntake();
            }

            // Toggle parking mechanism
            if (master.get_digital_new_press(buttons::UP))
            {
                park.toggle();
            }

            pros::delay(20);
        }
    }
    else
    {
        // Programmer mode: wheel calibration and position correction
        chassis.setPose(0, 0, 0);

        while (true)
        {
            chassis.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), false, 0.54);

            // Wheel position correction buttons
            if (master.get_digital_new_press(buttons::UP))
            {
                correct_position(LF, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::RIGHT))
            {
                correct_position(LR, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::LEFT))
            {
                correct_position(LL, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::DOWN))
            {
                correct_position(LB, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::X))
            {
                correct_position(LF, &chassis, true);
            }
            if (master.get_digital_new_press(buttons::A))
            {
                correct_position(LR, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::Y))
            {
                correct_position(LL, &chassis, false);
            }
            if (master.get_digital_new_press(buttons::B))
            {
                correct_position(LB, &chassis, true);
            }
            pros::delay(20);
        }
    }
}