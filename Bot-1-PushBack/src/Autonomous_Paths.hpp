#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void redSAWP() {

    chassis.setPose(-60.3,-18.5,180);

    chassis.moveToPoint(-48,-48,700);
    correct_position(rightLoc, &chassis, true);

    chassis.turnToPoint(-70,-48,400);
    matchLoader.setState(true);
    chassis.moveToPoint(-70,-46,1000,{.maxSpeed = 70});
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48,-46,700,{false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);

    chassis.turnToPoint(0,-48,500);
    chassis.moveToPoint(-36,-48,700);
    pros::delay(200);
    rollers::setState("scoreTop");  

    chassis.waitUntilDone();
    pros::delay(1000);
    rollers::setState("intake");
    pros::delay(300);
    chassis.moveToPoint(-48,-24,700,{false});
    chassis.turnToPoint(-24,-24,400);
    rollers::setState("directIntake");
    chassis.moveToPoint(-24,-24,1000);
    chassis.turnToPoint(-9,-9, 700);
    chassis.moveToPoint(-9,-9,1000,{.maxSpeed = 40});
    chassis.waitUntilDone();
    pros::delay(400);
    rollers::setState("scoreBottom");
    pros::delay(1200);
    rollers::setState("none");
    chassis.moveToPoint(-24,-24,300,{false});
    chassis.turnToPoint(-24,24,400);
    chassis.moveToPoint(-24,16,400, {.minSpeed = 90, .earlyExitRange = 4});
    rollers::setState("intake");

    chassis.moveToPoint(-24,24,1000,{.maxSpeed = 50});
    chassis.turnToPoint(0,0,700);
    chassis.moveToPoint(-12,10,700);
    chassis.turnToPoint(0,0,200);
    chassis.waitUntilDone();
    rollers::setState("scoreMiddleAuton");

    
    


    
    
}
void redAuton2() {
    
}
void blueSAWP() {
    chassis.setPose(60.3,-17.5,180);
    chassis.moveToPoint(48,-48,1000);
    chassis.turnToPoint(70,-48,700);
    matchLoader.setState(true);
    chassis.moveToPoint(70,-48,1000,{.maxSpeed = 70});
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(500);
    chassis.moveToPoint(48,-48,1000,{false});
    chassis.turnToPoint(0,-48,1000);
    chassis.moveToPoint(28,-48,1000,{.maxSpeed = 60});
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}
void blueAuton2() {
    
}

void exampleAuton() {

}
#endif