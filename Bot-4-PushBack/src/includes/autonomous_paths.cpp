#include "autonomous_paths.hpp"

void moveTo(double x, double y, int t1, int t2, bool b1, bool b2)
{
    chassis.turnToPoint(x, y, t1, {.forwards = b1});
    chassis.moveToPoint(x, y, t2, {.forwards = b2});
}

void left()
{
    chassis.setPose(-47.5, 18, 90);
    intake();
    chassis.moveToPoint(-24, 24, 1000, {.maxSpeed = 70});
    pros::delay(600);
    matchLoad(true);
    chassis.turnToPoint(0, 0, 500);
    chassis.moveToPoint(-8, 8, 800, {.maxSpeed = 70});
    matchLoad(false);
    pros::delay(600);
    score();
    chassis.waitUntilDone();
    pros::delay(1200);
    intake();
    chassis.moveToPoint(-42, 46, 1200, {false});
    chassis.turnToPoint(-70, 46, 500);
    matchLoad(true);
    chassis.waitUntilDone();
    chassis.moveToPoint(-70, 47, 1000, {.maxSpeed = 60});
    chassis.waitUntilDone();
    chassis.arcade(10, 0);
    pros::delay(200);

    chassis.moveToPoint(-42, 47, 500, {false});
    chassis.waitUntilDone();
    matchLoad(false);
    chassis.turnToPoint(-24, 48, 700);
    chassis.moveToPoint(-24, 48, 500);
    liftToggle(true);
    chassis.waitUntilDone();
    score();
    chassis.arcade(30, 0);
    pros::delay(1300);
    chassis.moveToPoint(-48, 48, 500, {false});
    chassis.turnToPoint(-34, 37, 500);
    chassis.moveToPoint(-34, 37, 500);
    chassis.turnToPoint(-10, 40, 300);
    chassis.moveToPoint(-10, 40, 1400, {.minSpeed = 127});
    chassis.turnToHeading(90, 700);
    pros::delay(1200);
}
void left2() {}
void soloAWP() {}
void right()
{
    chassis.setPose(-48, -17, 90);
    intake(true);
    pros::delay(300);
    chassis.moveToPoint(-24, -24, 700, {.minSpeed = 40, .earlyExitRange = 3});
    chassis.moveToPoint(-14.5, -40, 700, {.minSpeed = 60, .earlyExitRange = 5});
    chassis.moveToPoint(-9.5, -55, 1000, {.maxSpeed = 70, .minSpeed = 50, .earlyExitRange = 5});
    chassis.moveToPoint(-29, -62, 1000, {.maxSpeed = 70, .minSpeed = 50, .earlyExitRange = 5});
    chassis.moveToPoint(-48, -48, 700);
    chassis.turnToPoint(-24, -48, 700, {false});
    chassis.moveToPoint(-24, -48, 1000, {false});
    liftToggle(true);
    chassis.waitUntilDone();
    score();
    chassis.arcade(-30, 0);
    pros::delay(1300);
    chassis.moveToPoint(-70, -48, 1200, {.maxSpeed = 70});
    intake(true);
    matchLoad(true);
    chassis.waitUntilDone();
    chassis.arcade(30, 0);
    pros::delay(1000);
    matchLoad(false);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -48, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    pros::delay(300);
    score();
    chassis.arcade(-30, 0);
}
void right2() {}
void skills() {}

void test()
{
    chassis.setPose(0, 0, 0);
    moveTo(0, 48, 3000, 3000, true, true);
    moveTo(-48, 48, 3000, 3000, true, true);
    moveTo(-48, 0, 3000, 3000, true, true);
    moveTo(0, 0, 3000, 3000, true, true);
}