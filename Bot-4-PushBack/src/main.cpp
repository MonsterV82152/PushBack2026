#include "includes.hpp"
#include "movements.hpp"
#include "autonomous_selector.hpp"



using namespace pros;

void on_center_button() {}

void initialize()
{
    //autonSelect.start();
    chassis.calibrate();
    pros::lcd::initialize();
    systemMotors.set_zero_position(0);
    pros::Task periodicTask([&](){
        while (true)
        {
            periodic();
            pros::delay(10);
        }
    });
    wingToggle(false);
    liftToggle(false);
    

}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    // autonSelect.runAuton();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 24, 10000);
    
}

void opcontrol()
{
    //right();
    test();
    while (true)
    {
        lcd::print(0, "X: %.3f, %.3f, %.3f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta); // x, y, theta
        // lcd::print(1, "Y: %.3f", ); // y
        chassis.arcade(LEFT_Y, RIGHT_X, false, 0.6);
        if (L1_NEW_PRESS)
        {
            if (lift.getState())
            {
                scoreAndHold(60);
            }
            else
            {
                scoreAndHold(50);
            }
        }
        else if (L1_RELEASED)
        {
            lowerScoring();
        }
        if(lift.getState())
        {
            if (DOWN_NEW_PRESS)
            {
                wingToggle(false);
            }
            else if (DOWN_RELEASED)
            {
                wingToggle(true);
            }
        }
        if (L2_NEW_PRESS)
        {
            liftToggle();
            if(!lift.getState())
            {
                wingToggle(false);
            }
            else
            {
                wingToggle(true);
            }
        }
        if (R2_NEW_PRESS)
        {
            intakeLiftToggle(true);
            reverse(true, 35);
        }
        else if (R2_RELEASED)
        {
            intakeLiftToggle(false);
            reverse(false);
        }
        if (R1_NEW_PRESS)
        {
            intake();
        }
        if (B_NEW_PRESS)
        {
            matchLoad(true);
        }
        else if (B_RELEASED)
        {
            matchLoad(false);
        }

        
        //PID tuning
        if (LEFT_NEW_PRESS)
        {
            chassis.setPose(0, 0, 0);
            int tar = 179.9999;
            chassis.turnToHeading(tar, 3000);
            delay(2500);
            master.print(1, 0, "%.3f", tar - chassis.getPose().theta);
            delay(3000);
        }
        if (UP_NEW_PRESS)
        {
            chassis.setPose(0, 0, 0);
            chassis.moveToPose(0, 48, 0, 5000);
            delay(5500);
            master.print(1, 0, "X: %.2f, Y: %.4f", chassis.getPose().x, chassis.getPose().y);
        }
        
        if (X_NEW_PRESS) {
            double tot = 0;
            for (double i = 48; i <= 48; i += 8) {
                double target = chassis.getPose().y + i;
                chassis.moveToPoint(0, target, 4000);
                delay(4050);
                master.print(0, 0, "%f", target-chassis.getPose().y);
                tot += target-chassis.getPose().y;
            }

            master.print(1, 0, "%.4f", tot);
            delay(5000);
        }
        if (A_NEW_PRESS) {
            double tot = 0;
            for (double i = 9.99; i <= 180; i += 10) {
                double target = chassis.getPose().theta + i;
                chassis.turnToHeading(target, 1500);
                delay(2000);
                tot += target - chassis.getPose().theta;
            }
            delay(500);
	        master.print(1, 0, "%.5f", tot);
        }

        pros::delay(10);
    }
}