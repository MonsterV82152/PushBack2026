#include "autonomous_paths.hpp"

void left()
{
    robot.setPose(-46, 16, 90);
    robot.intake();
    robot.moveToPoint(-24, 24, 1000);
    robot.moveToPoint(-9, 48, 1000);
    robot.moveToPoint(-24, 30, 1000, {false});
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000);
    robot.moveToPoint(-48, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score();
}

void left2()
{
    robot.setPose(-46, 16, 90);
    robot.intake();
    robot.moveToPoint(-24, 24, 1000);
    robot.moveToPoint(-9, 48, 1000);
    robot.moveToPoint(-24, 24, 1000, {false});
    robot.turnToPoint(-12, 12, 700);
    robot.moveToPoint(-12, 12, 700);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(500);
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000);
    robot.moveToPoint(-48, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score();
}

void printPose()
{
}
void soloAWP()
{
    robot.setPose(-48, -24, 180);
    robot.moveToPoint(-48, -40, 400, {.minSpeed = 127, .earlyExitRange = 5});
    robot.turnToHeading(270, 400, {.minSpeed = 127, .earlyExitRange = 3});
    robot.matchLoad(true);
    robot.waitUntilDone();
    robot.intake();
    robot.moveToPoint(-70, -48, 1000);
    robot.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48, 900);
    robot.moveToPoint(-28, -48, 1000);
    pros::delay(500);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1700);
    pros::delay(1000);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 800);
    pros::delay(600);
    robot.matchLoad(true);
    robot.turnToPoint(-24, 24, 500);
    robot.moveToPoint(-24, 24, 1300);
    pros::delay(200);
    robot.matchLoad(false);
    pros::delay(1000);
    robot.matchLoad(true);
    robot.turnToPoint(-12, 12, 700);
    robot.moveToPoint(-12, 12, 1000);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(1000);
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000);
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 1000);
    pros::delay(500);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1700);
}

void halfSAWPRight()
{
}

void halfSAWPLeft()
{
}

void right()
{
    robot.setPose(-46, -16, 90);
    robot.intake();
    robot.moveToPoint(-36, -16, 700);
    // robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 700);
    // robot.turnToPoint(-9, -48, 700);
    robot.moveToPoint(-9, -48, 1000);
    robot.moveToPoint(-24, -30, 1000, {false});
    robot.turnToPoint(-48, -48, 700, {false});
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-70, -48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000);
    robot.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48, 700);
    robot.moveToPoint(-28, -48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score();
}

void right2()
{
    robot.setPose(-46, -16, 90);
    robot.intake();
    robot.moveToPoint(-24, -24, 1000);
    robot.moveToPoint(-9, -48, 1000);
    robot.moveToPoint(-24, -24, 1000, {false});
    robot.turnToPoint(-12, -12, 700);
    robot.moveToPoint(-12, -12, 700);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(1000);
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-70, -48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000);
    robot.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48, 700);
    robot.moveToPoint(-28, -48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score();
}

void skills()
{
    robot.setPose(-36, 0, 270);
    robot.intake();
    robot.moveToPoint(-70, 0, 2000);
    robot.moveToPoint(-36, 0, 2000, {false});
    robot.turnToPoint(-24, 24, 700);
    robot.moveToPoint(-24, 24, 1000);
    robot.turnToPoint(-12, 12, 700);
    robot.moveToPoint(-12, 12, 1000);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(1000);
    robot.moveToPoint(-24, 24, 1000, {false});
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 1000);
    robot.turnToPoint(-48, -48, 700);
    robot.moveToPoint(-48, -48, 1000);
    robot.turnToPoint(-70, -48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-24, -60, 700, {false});
    robot.moveToPoint(-24, -60, 800, {false});
    robot.moveToPoint(24, -60, 1000, {false});
    robot.moveToPoint(48, -48, 1000, {false});
    robot.turnToPoint(28, -48, 700);
    robot.moveToPoint(28, -48, 1000);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1700);
    pros::delay(1000);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(48, -48, 1000, {false});
    robot.turnToPoint(70, -48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(70, -48, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(28, -48, 700);
    robot.moveToPoint(28, -48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1700);
    pros::delay(1000);
    robot.lift(false);
    robot.intake();
}

void riskySkills()
{
}
void riskySkillsV2()
{
}

void test()
{
}