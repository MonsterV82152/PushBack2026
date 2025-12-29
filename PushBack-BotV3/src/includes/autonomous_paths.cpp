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
    robot.setPose(-48, -9, 0);
    robot.intake();
    robot.moveToPoint(-48, 15, 500);
    robot.moveToPoint(-48, -48, 1300, {false});
    robot.turnToPoint(-70, -48, 500);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 800);
    robot.waitUntilDone();
    pros::delay(200);
    helper.leftDT.move(70);
    helper.rightDT.move(70);
    pros::delay(200);
    helper.leftDT.move(0);
    helper.rightDT.move(0);
    robot.moveToPoint(-48, -48, 600, {false});
    robot.matchLoad(false);
    robot.turnToHeading(90, 800);
    robot.moveToPoint(-28, -48, 1000);
    robot.lift();
    pros::delay(300);
    robot.score(1750);
    robot.waitUntilDone();
    pros::delay(800);
    robot.lift(false);
    robot.intake();
    robot.setLow();
    robot.moveToPoint(-48, -48, 500, {false});
    robot.turnToPoint(-24, -24, 500);
    robot.moveToPoint(-24, -24, 700);
    robot.moveToPoint(-10, -8, 1000);
    pros::delay(700);
    robot.scoreLow(127);
    robot.moveState({ON, LEAVE, -100, LEAVE});
    pros::delay(1000);
    robot.moveToPoint(-24, -24, 700, {false});
    robot.turnToPoint(-24, 24, 600);
    robot.lowerIntake();
    robot.intake();
    robot.moveToPoint(-23.5, 24, 1200);
    pros::delay(900);
    robot.matchLoad(true);
    robot.turnToPoint(-10, 11.5, 700);
    robot.moveToPoint(-10, 11.5, 1000);
    robot.matchLoad(false);
    pros::delay(200);
    robot.score(1770);
}

void right()
{
    robot.setPose(-46, -16, 90);
    robot.intake();
    robot.moveToPoint(-36, -16, 700);
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 700);
    robot.turnToPoint(-9, -48, 700);
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
    robot.moveToPoint(-36, -16, 700);
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 700);
    robot.turnToPoint(-9, -48, 700);
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

void move(int speed)
{
    helper.leftDT.move(speed);
    helper.rightDT.move(speed);
}

void skills()
{
    robot.setPose(-50, 0, 270);
    robot.intake();
    move(40);
    pros::delay(500);
    move(-30);
    pros::delay(200);
    move(30);
    pros::delay(500);
    move(-50);
    pros::delay(400);
    move(70);
    pros::delay(500);
    move(-30);
    pros::delay(300);
    move(30);
    pros::delay(400);
    move(-30);
    pros::delay(300);
    move(30);
    pros::delay(400);
    move(-80);
    robot.matchLoad(true);
    pros::delay(700);
    robot.matchLoad(false);
    robot.turnToPoint(-24, 24, 800);
    robot.moveToPoint(-24, 24, 1200);
    robot.turnToPoint(-12, 12, 700);
    robot.moveToPoint(-12, 12, 1000);
    robot.waitUntilDone();
    robot.score(1800, 55, true);
    pros::delay(3000);
    robot.moveToPoint(-24, 24, 1000, {false});
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 1200);
    robot.turnToPoint(-48, -48, 700);
    robot.moveToPoint(-48, -48, 1200);
    robot.turnToPoint(-70, -48, 900);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(-48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-24, -58, 700, {false});
    robot.moveToPoint(-24, -58, 800, {false});
    robot.moveToPoint(24, -58, 1200, {false});
    robot.turnToPoint(48, -44, 700, {false});
    robot.moveToPoint(48, -44, 1000, {false});
    robot.turnToPoint(28, -46, 700);
    robot.moveToPoint(28, -46, 1000);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1700);
    pros::delay(1000);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(44, -46, 1000, {false});
    robot.turnToPoint(70, -46, 1000);
    robot.matchLoad(true);
    robot.moveToPoint(70, -46, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(48, -46, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(28, -48, 1000);
    robot.moveToPoint(28, -48, 1000);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.setLow();
    pros::delay(1000);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(48, -48, 1000, {false});
    robot.turnToPoint(36, 0, 700);
    robot.moveToPoint(36, 0, 1300);
    robot.turnToPoint(70, 0, 1000);
    robot.waitUntilDone();
    pros::delay(100);
    move(100);
    pros::delay(1000);
    move(-30);
    pros::delay(200);
    move(30);
    pros::delay(500);
    move(-30);
    pros::delay(200);
    move(30);
    pros::delay(500);
    move(-50);
    pros::delay(400);
    move(70);
    pros::delay(500);
    move(-30);
    pros::delay(300);
    move(30);
    pros::delay(400);
    move(-30);
    pros::delay(300);
    move(30);
    pros::delay(400);
    move(-80);
    pros::delay(700);
    robot.turnToPoint(24, 24, 1000);
    robot.moveToPoint(24, 24, 1000);
    robot.turnToPoint(9, 9, 700);
    robot.moveToPoint(9, 9, 1000);
    robot.waitUntilDone();
    robot.scoreLow(60);
    pros::delay(1600);
    robot.moveToPoint(48, 48, 1000, {false});
    robot.turnToPoint(70, 48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(70, 48, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(48, 48, 700, {false});
    robot.matchLoad(false);
    robot.moveToPoint(24, 60, 800, {false});
    robot.moveToPoint(-24, 60, 1000, {false});
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 1000);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-70, 48, 700);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000);
    robot.waitUntilDone();
    pros::delay(500);
    robot.moveToPoint(-48, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 700);
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);
    robot.intake();
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-56, 24, 700);
    robot.moveToPoint(-56, 24, 1000);
    robot.turnToPoint(-60, 0, 700);
    move(80);
    pros::delay(800);
    move(0);
}

void test()
{
    move(50);
    pros::delay(500);
    move(0);
}