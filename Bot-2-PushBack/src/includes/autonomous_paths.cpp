#include "autonomous_paths.hpp"


void left() {
    chassis.setPose(-46.733, 15.607, 0);
    chassis.moveToPoint(-48, 48, 800);
    chassis.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70, 48, 1000);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, 48, 700, {false});
    robot.matchLoad(false);
    chassis.turnToPoint(-30, 48, 700);
    chassis.moveToPoint(-30, 48, 700);
    chassis.waitUntilDone();
    robot.scoreL3();
    pros::delay(2000);
    chassis.moveToPoint(-48, 48, 700, {false});
    chassis.turnToPoint(0,0,700);
    robot.intake();
    chassis.moveToPoint(-30,30,700, {.minSpeed = 70, .earlyExitRange = 5});
    chassis.moveToPoint(-18,18,1200, {.maxSpeed = 70});
    chassis.waitUntilDone();
    robot.scoreL1();

}

void left2() {
    chassis.setPose(-47.335, 16.611, 90);
    robot.intake();
    chassis.moveToPoint(-30,24,1000);
    chassis.moveToPoint(-24,24,700,{.maxSpeed=70});
    chassis.waitUntilDone();
    chassis.turnToPoint(-48,48,700);
    chassis.moveToPoint(-48,48,1200);
    chassis.turnToPoint(-70,48,700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70,48,1000);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48,48,700,{false});
    robot.matchLoad(false);
    chassis.turnToPoint(-30,48,700);
    chassis.moveToPoint(-30,48,1000);
    chassis.waitUntilDone();
    robot.scoreL3();

}

void soloAWP() {

}

void right() {
    chassis.setPose(-46.733, -15.607, 180);
    chassis.moveToPoint(-48, -48, 800);
    chassis.turnToPoint(-70, -48, 700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70, -48, 1000);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    chassis.turnToPoint(-30, -48, 700);
    chassis.moveToPoint(-30, -48, 700);
    chassis.waitUntilDone();
    robot.scoreL3();
    pros::delay(2000);
    chassis.moveToPoint(-48, -48, 700, {false});
    chassis.turnToPoint(0,0,700);
    robot.intake();
    chassis.moveToPoint(-30,-30,700, {.minSpeed = 70, .earlyExitRange = 5});
    chassis.moveToPoint(-18,-18,1200, {.maxSpeed = 70});
    chassis.waitUntilDone();
    robot.scoreL1();
}

void right2() {
    chassis.setPose(-47.335, -16.611, 90);
    robot.intake();
    chassis.moveToPoint(-30,-24,1000);
    chassis.moveToPoint(-24,-24,700,{.maxSpeed=70});
    chassis.waitUntilDone();
    chassis.turnToPoint(-48,-48,700);
    chassis.moveToPoint(-48,-48,1200);
    chassis.turnToPoint(-70,-48,700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70,-48,1000);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48,-48,700,{false});
    robot.matchLoad(false);
    chassis.turnToPoint(-30,-48,700);
    chassis.moveToPoint(-30,-48,1000);
    chassis.waitUntilDone();
    robot.scoreL3();
}

void skills() {
    
}

void test() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 48, 2000);
    chassis.turnToPoint(48, 48, 1000);
    chassis.moveToPoint(48, 48, 2000);
    chassis.turnToPoint(48, 0, 1000);
    chassis.moveToPoint(48, 0, 2000);
    chassis.turnToPoint(0, 0, 1000);
    chassis.moveToPoint(0, 0, 2000);
    chassis.turnToHeading(0, 1000);
}