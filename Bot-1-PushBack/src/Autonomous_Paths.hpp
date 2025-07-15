#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void redSAWP() {

    chassis.setPose(-60.3,-18.5,180);

    chassis.moveToPoint(-48,-47,700);
    correct_position(rightLoc, &chassis, true); 

    chassis.turnToPoint(-70,-47,400);
    matchLoader.setState(true);
    chassis.moveToPoint(-64,-46,1000,{.maxSpeed = 70});
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
    chassis.turnToPoint(-17,-17, 700);
    chassis.moveToPoint(-17,-17,700,{.maxSpeed = 40});
    pros::delay(200);
    rollers::setState("scoreBottom");

    chassis.waitUntilDone();
    pros::delay(1000);
    rollers::setState("none");
    chassis.moveToPoint(-24,-24,300,{false});
    chassis.turnToPoint(-24,24,400);
    chassis.moveToPoint(-24,16,400, {.minSpeed = 90, .earlyExitRange = 4});
    rollers::setState("intake");

    chassis.moveToPoint(-24,24,1000,{.maxSpeed = 50});
    chassis.turnToPoint(0,0,700);
    chassis.moveToPoint(-12,10,700);
    chassis.turnToPoint(0,-3,300);
    rollers::setState("scoreMiddleAuton");

    chassis.waitUntilDone();

    
    


    
    
}
void redAuton2() {
    chassis.setPose(-60.3,18.5,0);

    chassis.moveToPoint(-48,47,700);
    correct_position(leftLoc, &chassis, true); 

    chassis.turnToPoint(-70,47,400);
    matchLoader.setState(true);
    chassis.moveToPoint(-70,46,1000,{.maxSpeed = 70});
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48,46,700,{false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);

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

void skills() {
    
}
#endif