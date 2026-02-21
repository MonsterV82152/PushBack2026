#include "includes.hpp"
#include "movements.hpp"
#include "autonomous_selector.hpp"

using namespace pros;

/// Callback for center button press (currently unused)
void on_center_button() {}

/// Robot initialization
/// Sets up chassis calibration, LCD, motors, and starts the periodic task
void initialize()
{
    //autonSelect.start();
    chassis.calibrate();  // Calibrate IMU and odometry
    pros::lcd::initialize();  // Initialize brain LCD
    systemMotors.set_zero_position(0);  // Reset system motor positions
    
    // Start background task for periodic control loop
    pros::Task periodicTask([&]()
                            {
        while (true)
        {
            periodic();  // Run scoring/intake control
            pros::delay(10);
        } });
    
    wingToggle(false);  // Initialize wings retracted
    liftToggle(false);  // Initialize lift down
}

/// Called when robot is disabled
void disabled() {}

/// Pre-competition initialization
void competition_initialize() {}

/// Autonomous routine - runs selected autonomous strategy
void autonomous()
{
    // autonSelect.runAuton();  // Use autonomous selector
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, 24, 10000);
    //right();  // Alternative right side routine
    soloAWP();  // Run solo AWP (Autonomous Win Point) strategy
    //right2();  // Alternative strategy
}

/// Main driver control loop
void opcontrol()
{
    //skills();  // Skills routine disabled
    while (true)
    {
        // Display chassis position on LCD
        lcd::print(0, "X: %.3f, %.3f, %.3f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta); // x, y, theta
        // lcd::print(1, "Y: %.3f", ); // y
        
        // Arcade drive control with 0.6 deadzone
        chassis.arcade(LEFT_Y, RIGHT_X, false, 0.67);
        
        // L1: Scoring control
        if (L1_NEW_PRESS)
        {
            if (lift.getState())
            {
                scoreAndHold(127); //change for skills 80/match 127
            }
            else
            {
                intakeLiftToggle(true);
                scoreAndHold(90); //change for skills 50/match 90
            }
        }
        else if (L1_RELEASED)
        {
            lowerScoring();  // Return to idle position
            intakeLiftToggle(false);
        }
        // DOWN: Wing control (only when lift is up)
        if (lift.getState())
        {
            if (DOWN_NEW_PRESS)
            {
                wingToggle(false);  // Retract wings
            }
            else if (DOWN_RELEASED)
            {
                wingToggle(true);  // Extend wings
            }
        }
        // L2: Lift toggle with automatic wing control
        if (L2_NEW_PRESS)
        {
            liftToggle();  // Toggle lift up/down
            if (!lift.getState())
            {
                wingToggle(false);  // Retract wings when lifting down
            }
            else
            {
                wingToggle(true);  // Extend wings when lifting up
            }
        }
        // R2: Reverse (outtake) control
        if (R2_NEW_PRESS)
        {
            intakeLiftToggle(true);  // Raise intake lift
            reverse(true, 127);      // Start reverse at full speed (change for skills 40) / match 127
        }
        else if (R2_RELEASED)
        {
            intakeLiftToggle(false); // Lower intake lift
            reverse(false);          // Stop reverse
        }
        // R1: Intake toggle
        if (R1_NEW_PRESS)
        {
            intake();  // Toggle intake on/off
        }
        // B: Match loader control
        if (B_NEW_PRESS)
        {
            matchLoad(true);  // Engage match loader
        }
        else if (B_RELEASED)
        {
            matchLoad(false); // Disengage match loader
        }

        // PID tuning and testing routines
        if (LEFT_NEW_PRESS)
        {
            // Test turning PID: turn 180 degrees and measure error
            chassis.setPose(0, 0, 0);
            int tar = 179.9999;
            chassis.turnToHeading(tar, 3000);
            delay(2500);
            master.print(1, 0, "%.3f", tar - chassis.getPose().theta);
            delay(3000);
        }
        if (UP_NEW_PRESS)
        {
            // Test linear movement PID: move 48 inches forward
            chassis.setPose(0, 0, 0);
            chassis.moveToPose(0, 48, 0, 5000);
            delay(5500);
            master.print(1, 0, "X: %.2f, Y: %.4f", chassis.getPose().x, chassis.getPose().y);
        }
        if (Y_NEW_PRESS)
        {
            // Quick rotation test
            chassis.arcade(0, -127);
            pros::delay(200);
            chassis.arcade(0, 127);
            pros::delay(200);
            chassis.arcade(0, 0);
        }
        if (X_NEW_PRESS)
        {
            // Forward movement distance test (disabled)
            // double tot = 0;
            // for (double i = 48; i <= 48; i += 8)
            // {
            //     double target = chassis.getPose().y + i;
            //     chassis.moveToPoint(0, target, 4000);
            //     delay(4050);
            //     master.print(0, 0, "%f", target - chassis.getPose().y);
            //     tot += target - chassis.getPose().y;
            // }

            // master.print(1, 0, "%.4f", tot);
            // delay(5000);
            //right();  // Run right side autonomous routine
            skills();
        }
        if (A_NEW_PRESS)
        {
            // Incremental turn test: measure cumulative error
            double tot = 0;
            for (double i = 9.99; i <= 180; i += 10)
            {
                double target = chassis.getPose().theta + i;
                chassis.turnToHeading(target, 1500);
                delay(2000);
                tot += target - chassis.getPose().theta;  // Accumulate error
            }
            delay(500);
            master.print(1, 0, "%.5f", tot);  // Display total error
        }

        pros::delay(10);
    }
}