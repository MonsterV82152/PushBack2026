#include "includes.hpp"
#include "autonomous_paths.hpp"

/// Callback for center button press (currently unused)
void on_center_button() {}

/// Robot initialization
/// Sets up chassis calibration, LCD, motors, and starts the periodic task
void initialize()
{
    // autonSelect.start();
    pros::lcd::initialize();
    chassis.calibrate(); // Calibrate IMU and odometry

    // Start background task for periodic control loop
    autonSelect.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "right1", "", right},
        autonomousRoute{"red", "right2", "", right2},
        autonomousRoute{"red", "right3", "", right3},
        autonomousRoute("red", "Left1G", "", left),
        autonomousRoute{"red", "Left2G", "", left2},
        autonomousRoute{"red", "Left3G", "", left3},
        autonomousRoute("red", "Solo", "", solo),
        autonomousRoute{"red", "DriveOff", "", driveOff},

        autonomousRoute{"blue", "right1", "", right},
        autonomousRoute{"blue", "right2", "", right2},
        autonomousRoute{"blue", "right3", "", right3},
        autonomousRoute{"blue", "Left1G", "", left},
        autonomousRoute{"blue", "Left2G", "", left2},
        autonomousRoute{"blue", "Left3G", "", left3},
        autonomousRoute("blue", "Solo", "", solo),
        autonomousRoute{"blue", "DriveOff", "", driveOff},
    });
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "", skills});
    // autonSelect.start(); // Start autonomous selector task
    pros::Task screen_task([&]()
                           {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f.2", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f.2", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f.2", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(20);
        } });

    startScoreThread(); // Start background scoring thread (only starts once)

    wing.setState(false);
    lift.setState(true);
    intakeLift.setState(false);
    pto.setState(true);
    matchLoader.setState(false);
    lever.setState(false);
    hood.setState(false);

    while (pot.get_value() > LEVER_DOWN_ANGLE)
    {
        pto.setState(false);
        motors.move(-127);
        pros::delay(10);
    }
    motors.move(0);
    pto.setState(true);
}

/// Called when robot is disabled
void disabled() {}

/// Pre-competition initialization
void competition_initialize() {}

/// Autonomous routine - runs selected autonomous strategy
void autonomous()
{
    // autonSelect.runAuton(); // Use autonomous selector
    square();
}

/// Main driver control loop
void opcontrol()
{
    wing.setState(true);
    bool lowgoal = false;
    while (true)
    {
        moveDT(LEFT_Y, RIGHT_X);

        // pros::lcd::print(0, "X: %f", chassis.getPose().x);         // x
        // pros::lcd::print(1, "Y: %f", chassis.getPose().y);         // y
        // pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

        if (A_NEW_PRESS)
        {
            chassis.setPose(0, 0, 0);
            chassis.turnToHeading(90, 3000);
        }
        if (B_NEW_PRESS)
        {
            chassis.setPose(0, 0, 0);
            chassis.turnToHeading(180, 3000);
        }
        if (X_NEW_PRESS)
        {
            square();
        }

        // if (R1_NEW_PRESS)
        // {
        //     if (intakeState == 0 || intakeState == -1)
        //     {
        //         intakeOn = true;
        //         intakeState = 1;
        //         intake(127);
        //     }
        //     else if (intakeState == 1)
        //     {
        //         intakeOn = false;
        //         intakeState = 0;
        //         intake(0);
        //     }
        // }

        // if (R2_NEW_PRESS)
        // {
        //     if (lowgoal)
        //     {
        //         intakeLift.setState(true);
        //         lever.setState(false);
        //         intake(-40);
        //     }
        //     else
        //     {
        //         intake(-127);
        //     }
        // }
        // if (R2_RELEASED)
        // {
        //     if (intakeState == 1)
        //     {
        //         intakeOn = true;
        //         intakeState = 1;
        //         intake(127);
        //     }
        //     else
        //     {
        //         intake(0);
        //         intakeOn = false;
        //         intakeState = 0;
        //     }
        //     intakeLift.setState(false);
        //     lowgoal = false;
        // }

        // if (L1_NEW_PRESS)
        // {
        //     if (!lift.getState())
        //     {
        //         requestScore(70, 2000);
        //     }
        //     else
        //         requestScore(127, 1000);
        // }

        // if (L2_NEW_PRESS)
        // {
        //     lift.toggle();
        // }

        // if (DOWN_NEW_PRESS)
        // {
        //     wing.setState(false);
        // }
        // else if (DOWN_RELEASED)
        // {
        //     wing.setState(true);
        // }

        // if (B_NEW_PRESS)
        // {
        //     matchLoader.setState(true);
        // }
        // else if (B_RELEASED)
        // {
        //     matchLoader.setState(false);
        // }

        // if (X_NEW_PRESS)
        // {
        //     lever.setState(true);
        //     lowgoal = true;
        // }

        // if (UP_NEW_PRESS)
        // {
        //     correct_position(LF, &chassis, false, true);
        // }
        // if (LEFT_NEW_PRESS)
        // {
        //     correct_position(LL, &chassis, true, true);
        // }
        // if (RIGHT_NEW_PRESS)
        // {
        //     correct_position(LR, &chassis, true, true);
        // }
        // if (DOWN_NEW_PRESS)
        // {
        //     correct_position(LB, &chassis, false, true);
        // }
        // if (A_NEW_PRESS)
        // {
        //     correct_position(LR, &chassis, false, true);
        // }
        // if (Y_NEW_PRESS)
        // {
        //     correct_position(LL, &chassis, false, true);
        // }
        // if (B_NEW_PRESS)
        // {
        //     correct_position(LB, &chassis, true, true);
        // }
        // if (X_NEW_PRESS)
        // {
        //     correct_position(LF, &chassis, true, true);
        // }

        pros::delay(10);
    }
}