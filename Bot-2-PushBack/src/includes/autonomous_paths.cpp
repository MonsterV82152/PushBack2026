#include "autonomous_paths.hpp"

void left()
{
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
    chassis.turnToPoint(0, 0, 700);
    robot.intake();
    chassis.moveToPoint(-30, 30, 700, {.minSpeed = 70, .earlyExitRange = 5});
    chassis.moveToPoint(-18, 18, 1200, {.maxSpeed = 70});
    chassis.waitUntilDone();
    robot.scoreL1();
}

void left2()
{
    chassis.setPose(-47.5, 16.611, 90);
    correct_position(LB, &chassis, true);
    robot.intake();
    chassis.moveToPoint(-24, 24, 1000, {.maxSpeed = 70});
    pros::delay(600);
    robot.matchLoad(true);
    chassis.turnToPoint(-48, 48, 700);
    chassis.moveToPoint(-48, 48, 1200);
    chassis.turnToPoint(-70, 48, 700);
    chassis.moveToPoint(-70, 48, 1000);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, 48, 700, {false});
    robot.matchLoad(false);
    chassis.turnToPoint(-28, 48, 700);
    chassis.moveToPoint(-28, 48, 1000);
    chassis.waitUntilDone();
    robot.scoreL3();
}

void soloAWP()
{
}

void right()
{
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
    chassis.turnToPoint(0, 0, 700);
    robot.intake();
    chassis.moveToPoint(-30, -30, 700, {.minSpeed = 70, .earlyExitRange = 5});
    chassis.moveToPoint(-18, -18, 1200, {.maxSpeed = 70});
    chassis.waitUntilDone();
    robot.scoreL1();
}

void right2()
{
    chassis.setPose(-47.5, -16.611, 90);
    correct_position(LB, &chassis, true);
    robot.intake();
    chassis.moveToPoint(-24, -24, 1000, {.maxSpeed = 70});
    pros::delay(600);
    robot.matchLoad(true);
    chassis.turnToPoint(-48, -48, 700);
    chassis.moveToPoint(-48, -48, 1200);
    chassis.turnToPoint(-70, -48, 700);
    chassis.moveToPoint(-70, -48, 1000);
    correct_position(LL, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    chassis.turnToPoint(-28, -48, 700);
    chassis.moveToPoint(-28, -48, 1000);
    chassis.waitUntilDone();
    robot.scoreL3();
}

void skills()
{
    chassis.setPose(-48, 17, 0);
    correct_position(LL, &chassis, true);
    chassis.moveToPoint(-48, 48, 1000);
    chassis.waitUntilDone();
    correct_position(LF, &chassis, false);
    chassis.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70, 48, 1000);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(1200);
    chassis.moveToPoint(-48, 48, 1000, {false});
    robot.matchLoad(false);
    chassis.moveToPoint(-30, 59, 900, {.forwards = false});
    pros::delay(400);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(30, 65, 1000, {.forwards = false});
    chassis.moveToPoint(48, 48, 1000, {.forwards = false});
    chassis.turnToPoint(25, 48, 900);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(25, 48, 900);
    chassis.waitUntilDone();
    robot.scoreL3();
    correct_position(LR, &chassis, false);
    pros::delay(2000);
    chassis.moveToPoint(48, 48, 900, {false});
    chassis.turnToPoint(70, 48, 900);
    robot.matchLoad(true);
    chassis.moveToPoint(70, 48, 1000);
    correct_position(LL, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(1200);
    chassis.moveToPoint(48, 48, 900, {false});
    robot.matchLoad(false);
    chassis.turnToPoint(25, 48, 900);
    chassis.moveToPoint(25, 48, 1000);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    robot.scoreL3();
    pros::delay(2000);
    chassis.moveToPoint(40, 48, 700, {false});
    robot.intake();
    chassis.turnToPoint(40, -48, 900);
    chassis.moveToPoint(40, -48, 2000);
    pros::delay(400);
    correct_position(LL, &chassis, true);
    // chassis.turnToPoint(60, 24, 700);
    // chassis.moveToPoint(62, 24, 700);
    // chassis.moveToPoint(60, -24, 1000);
    // chassis.moveToPoint(48, -48, 1000);
    pros::delay(400);
    correct_position(LF, &chassis, false);
    chassis.waitUntilDone();
    correct_position(LF, &chassis, false);
    chassis.turnToPoint(70, -48, 700);
    robot.matchLoad(true);
    chassis.moveToPoint(70, -48, 1000);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(3000);
    chassis.moveToPoint(48, -48, 700, {false});
    robot.matchLoad(false);
    chassis.moveToPoint(30, -60, 900, {.forwards = false});
    pros::delay(400);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-30, -65, 1000, {.forwards = false});
    pros::delay(400);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-48, -48, 900, {.forwards = false});
    chassis.turnToPoint(-25, -48, 700);
    chassis.moveToPoint(-25, -48, 700);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    robot.scoreL3();
    pros::delay(2000);
    chassis.moveToPoint(-48, -48, 700, {false});
    chassis.turnToPoint(-70, -48, 700);
    robot.matchLoad(true);
    chassis.moveToPoint(-70, -48, 1000);
    correct_position(LL, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(3000);
    chassis.moveToPoint(-48, -48, 700, {false});
    correct_position(LL, &chassis, false);
    robot.matchLoad(false);
    chassis.turnToPoint(-30, -48, 700);
    chassis.moveToPoint(-30, -48, 700);
    correct_position(LR, &chassis, false);
    chassis.waitUntilDone();
    robot.scoreL3();
    pros::delay(2000);
    chassis.moveToPoint(-48, -48, 700, {false});
    chassis.turnToPoint(-60, -25, 700, {false});
    chassis.moveToPoint(-62, -25, 700, {false});
    chassis.moveToPoint(-60, 0, 1000, {false});
}

void test()
{
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