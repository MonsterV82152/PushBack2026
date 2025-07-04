#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void redSAWP() {
    chassis.setPose(-60.3,-17.5,180);
    chassis.moveToPoint(-48,-48,1000);
    chassis.turnToPoint(-70,-48,700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70,-48,1000,{.maxSpeed = 70});
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(500);
    chassis.moveToPoint(-48,-48,1000,{false});
    chassis.turnToPoint(0,-48,1000);
    chassis.moveToPoint(-28,-48,1000,{.maxSpeed = 60});
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
    
}
void redAuton2() {
    
}
void blueAuton1() {
    
}
void blueAuton2() {
    
}

void exampleAuton() {

}
#endif