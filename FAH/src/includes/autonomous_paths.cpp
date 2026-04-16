#include "autonomous_paths.hpp"

void moveTo(double x, double y, int t1, int t2, bool b1, bool b2)
{
    chassis.turnToPoint(x, y, t1, {.forwards = b1});
    chassis.moveToPoint(x, y, t2, {.forwards = b2});
}

void left()
{
    chassis.setPose(-47, 16.4, 90);
    intake(127);
    moveTo(-22, 22, 300, 1000, true, true);
    pros::delay(700);
    matchLoader.setState(true);
    // finished intaking middle blocks

    moveTo(-47, 47, 500, 1500, true, true);
    chassis.turnToPoint(-60, 47, 400);
    correct_position(LR, &chassis, false);
    // infront of right matchloader

    chassis.moveToPoint(-55, 47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, 47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, 47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    moveTo(-45, 58, 500, 1500, true, true);
    // finished aligning for wing

    moveTo(-8, 58, 500, 5000, true, true);
    wing.setState(false);
    // finished wing
}

void left2()
{
    chassis.setPose(-47, 15.5, 0);
    intake(127);
    moveTo(-47, 47, 0, 1500, false, false);
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-60, 47, 400);
    // infront of left matchloader

    matchLoader.setState(true);
    chassis.moveToPoint(-55, 47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, 47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, 47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(500);
    chassis.moveToPoint(-22, 22, 700);
    // finished intaking for midgoal

    moveTo(-11, 11, 500, 500, false, false);
    chassis.waitUntilDone();
    requestScore(80, 3000);
    pros::delay(500);
    // finished scoring midgoal

    moveTo(-45, 58, 500, 1500, true, true);
    // finished aligning for wing

    moveTo(-8, 58, 500, 5000, true, true);
    wing.setState(false);
    // finished wing
}

void left3()
{
    chassis.setPose(-47, 15.5, 0);
    intake(127);
    moveTo(-47, 47, 0, 1500, false, false);
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-60, 47, 400);
    // infront of left matchloader

    matchLoader.setState(true);
    chassis.moveToPoint(-55, 47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, 47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, 47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(500);
    chassis.moveToPoint(-22, 22, 700);
    // finished intaking for midgoal

    moveTo(-11, 11, 500, 500, false, false);
    chassis.waitUntilDone();
    requestScore(80, 3000);
    pros::delay(500);
    // finished scoring midgoal

    chassis.moveToPoint(-22, 22, 700);
    intake(127);
    moveTo(-22, -22, 300, 1000, true, true);
    pros::delay(900);
    matchLoader.setState(true);
    lift.setState(true);
    // finished intaking for lowgoal

    moveTo(-8, -8, 300, 600, true, true);
    matchLoader.setState(false);
    chassis.waitUntilDone();
    intake(-80);
    pros::delay(1000);
    // finished scoring lowgoal
}

void right()
{
    chassis.setPose(-47, -16.4, 90);
    intake(127);
    moveTo(-22, -22, 300, 1000, true, true);
    pros::delay(700);
    matchLoader.setState(true);
    // finished intaking middle blocks

    moveTo(-47, -47, 500, 1500, true, true);
    chassis.turnToPoint(-60, -47, 400);
    correct_position(LL, &chassis, false);
    // infront of right matchloader

    chassis.moveToPoint(-55, -47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, -47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, -47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(300);
    moveTo(-36.75, -36.75, 300, 1000, true, true);
    wing.setState(false);
    chassis.turnToPoint(-36.75, -9.5, 700);
    // finished aligning for wing

    chassis.moveToPoint(-9.5, -36.75, 5000);
    // finished wing
}

void right2()
{
    chassis.setPose(-47, -15.5, 0);
    intake(127);
    moveTo(-47, -47, 0, 1500, false, false);
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-60, -47, 400);
    // infront of right matchloader

    matchLoader.setState(true);
    chassis.moveToPoint(-55, -47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, -47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, -47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(500);
    chassis.moveToPoint(-22, -22, 700);
    // finished intaking for lowgoal

    moveTo(-8, -8, 500, 500, true, true);
    chassis.waitUntilDone();
    intake(-70);
    pros::delay(500);
    // finished scoring low goal

    moveTo(-36.75, -36.75, 300, 1000, false, false);
    wing.setState(false);
    // finished aligning for wing

    moveTo(-9.5, -36.75, 300, 5000, true, true);
    // finished wing
}

void right3()
{
    chassis.setPose(-47, -15.5, 0);
    intake(127);
    moveTo(-47, -47, 0, 1500, false, false);
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-60, -47, 400);
    // infront of right matchloader

    matchLoader.setState(true);
    chassis.moveToPoint(-55, -47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, -47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, -47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(500);
    chassis.moveToPoint(-22, -22, 700);
    // finished intaking for lowgoal

    moveTo(-8, -8, 500, 500, true, true);
    chassis.waitUntilDone();
    intake(-70);
    pros::delay(500);
    // finished scoring low goal

    chassis.moveToPoint(-22, -22, 700, {.forwards = false});
    intake(127);
    moveTo(-22, 22, 300, 1000, true, true);
    pros::delay(900);
    matchLoader.setState(true);
    lift.setState(true);
    // finished intaking for midgoal

    moveTo(-11, 11, 300, 600, false, false);
    chassis.waitUntilDone();
    requestScore(80, 3000);
    pros::delay(1000);
    // finished scoring midgoal
}

void solo()
{
    chassis.setPose(-47, -15.5, 0);
    intake(127);
    moveTo(-47, -47, 0, 1500, false, false);
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-60, -47, 400);
    // infront of right matchloader

    matchLoader.setState(true);
    chassis.moveToPoint(-55, -47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, -47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, -47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(126, 2000);
    matchLoader.setState(false);
    pros::delay(400);
    // finished scoring right

    chassis.arcade(0, 127);
    pros::delay(500);
    chassis.moveToPoint(-22, -22, 700);
    // finished intaking for lowgoal

    moveTo(-8, -8, 500, 500, true, true);
    chassis.waitUntilDone();
    intake(-70);
    pros::delay(500);
    // finished scoring low goal

    chassis.moveToPoint(-22, -22, 700, {.forwards = false});
    intake(127);
    moveTo(-22, 22, 300, 1000, true, true);
    pros::delay(900);
    matchLoader.setState(true);
    lift.setState(true);
    // finished intaking for midgoal

    moveTo(-11, 11, 300, 600, false, false);
    chassis.waitUntilDone();
    requestScore(80, 3000);
    pros::delay(1000);
    // finished scoring midgoal

    chassis.moveToPoint(-47, 47, 1500);
    chassis.turnToPoint(-60, 47, 400);
    correct_position(LR, &chassis, false);
    // infront of left matchloader

    chassis.moveToPoint(-55, 47, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-60, 47, 300, {.maxSpeed = 80});
    pros::delay(500);
    // finished matchloading

    chassis.moveToPoint(-28, 47, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(127, 2000);
    // finished scoring left
}

void driveOff()
{
    chassis.arcade(127, 0);
    pros::delay(200);
    pros::delay(67);
    chassis.cancelAllMotions();
}

void parkWiggle()
{
    chassis.waitUntilDone();
    for (int i = 0; i < 2; i++)
    {
        chassis.arcade(10, 30);
        pros::delay(300);
        chassis.arcade(10, -30);
        pros::delay(300);
    }
}

void loadWiggle()
{
    chassis.waitUntilDone();
    for (int i = 0; i < 2; i++)
    {
        chassis.arcade(10, -30);
        pros::delay(300);
        chassis.arcade(10, 30);
        pros::delay(300);
    }
}

void skills()
{
    lemlib::Pose currentPose = chassis.getPose();
    chassis.setPose(-46, 0, 270);
    // intake(127);
    // intakeOn = true;
    // lift.setState(true);

    // // Clear Park
    // chassis.moveToPoint(-62, 0, 1000, {.minSpeed = 80});
    // parkWiggle();
    // chassis.turnToHeading(270, 500);
    // chassis.moveToPoint(-30, 0, 700, {.forwards = false});
    // chassis.waitUntilDone();

    // // Zero Drive Position
    // correct_position(LF, &chassis, true, true);

    // Mid High Goal
    chassis.turnToPoint(-15, 15, 800, {.forwards = false});
    chassis.moveToPoint(-15, 15, 1500, {.forwards = false, .maxSpeed = 80});
    chassis.turnToPoint(-20, 20, 800);
    chassis.moveToPoint(-20, 20, 700);
    chassis.waitUntilDone();
    lift.setState(false);
    pros::delay(500);
    chassis.moveToPoint(-8, 8, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    requestScore(90, 3000);
    pros::delay(3000);

    // High Goal #1
    chassis.moveToPoint(-48, 48, 1500);
    chassis.turnToPoint(-24, 48, 800, {.forwards = false});
    lift.setState(true);
    chassis.moveToPoint(-24, 48, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    // Match Load #1
    chassis.moveToPoint(-48, 48, 700, {.minSpeed = 30, .earlyExitRange = 3});
    matchLoader.setState(true);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-70, 48, 700, {.maxSpeed = 80});
    loadWiggle();
    chassis.moveToPoint(-50, 48, 700, {.forwards = false});
    pros::delay(300);
    correct_position(LR, &chassis, false);

    chassis.turnToPoint(-24, 65, 500, {.forwards = false});
    chassis.moveToPoint(-24, 65, 1000, {.forwards = false});
    chassis.turnToPoint(36, 65, 500, {.forwards = false});
    matchLoader.setState(false);
    chassis.moveToPoint(36, 65, 1500, {.forwards = false});
    correct_position(LR, &chassis, false);

    // Zero Drive Position
    chassis.turnToHeading(180, 700);
    correct_position(LB, &chassis, false);
    correct_position(LL, &chassis, true);

    // High Goal #2
    chassis.moveToPoint(48, 48, 800);
    chassis.turnToPoint(24, 48, 500, {.forwards = false});
    lift.setState(true);
    chassis.moveToPoint(24, 48, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    // Match Load #2
    chassis.moveToPoint(48, 48, 700, {.minSpeed = 30, .earlyExitRange = 3});
    matchLoader.setState(true);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(70, 48, 700, {.maxSpeed = 80});
    loadWiggle();

    // High Goal #3
    chassis.moveToPoint(24, 48, 1000, {.forwards = false});
    pros::delay(300);
    correct_position(LL, &chassis, false);
    matchLoader.setState(false);
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    chassis.moveToPoint(48, 48, 700);
    chassis.turnToPoint(45, 0, 600);
    chassis.moveToPoint(45, 0, 1000);
    lever.setState(true);
    chassis.turnToPoint(62, 0, 500);

    // Clear Park
    chassis.moveToPoint(62, 0, 1000, {.minSpeed = 80});
    parkWiggle();
    chassis.turnToHeading(90, 500);
    chassis.moveToPoint(30, 0, 700, {.forwards = false});
    chassis.waitUntilDone();

    // Zero Drive Position
    correct_position(LF, &chassis, true);

    // Low Goal
    chassis.moveToPoint(42, 0, 500);
    chassis.turnToPoint(42, 42, 700, {.forwards = false});
    chassis.moveToPoint(42, 42, 1000, {.forwards = false});
    chassis.turnToPoint(0, 0, 500);
    chassis.moveToPoint(34, 34, 700);
    chassis.turnToHeading(45, 300);
    chassis.turnToPoint(0, 0, 500);
    chassis.moveToPoint(8, 8, 700);
    lever.setState(false);
    intakeLift.setState(true);
    intake(-40);
    pros::delay(2000);
    chassis.moveToPoint(24, 24, 700, {.forwards = false});
    chassis.waitUntilDone();
    intake(127);
    intakeLift.setState(false);
    chassis.turnToPoint(24, -24, 500);
    chassis.moveToPoint(24, -24, 1000);

    // High Goal #4
    chassis.moveToPoint(48, -48, 1500);
    chassis.turnToPoint(24, -48, 500, {.forwards = false});
    lift.setState(true);
    chassis.moveToPoint(24, -48, 1000, {.forwards = false});
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    // Match Load #3
    chassis.moveToPoint(48, -48, 700, {.minSpeed = 30, .earlyExitRange = 3});
    matchLoader.setState(true);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(70, -48, 300, {.maxSpeed = 80});
    loadWiggle();
    chassis.moveToPoint(50, -48, 700, {.forwards = false});
    pros::delay(300);
    correct_position(LR, &chassis, false);

    chassis.turnToPoint(24, -65, 500, {.forwards = false});
    chassis.moveToPoint(24, -65, 1000, {.forwards = false});
    chassis.turnToPoint(-36, -65, 500, {.forwards = false});
    matchLoader.setState(false);
    chassis.moveToPoint(-36, -65, 1500, {.forwards = false});
    correct_position(LR, &chassis, false);

    // Zero Drive Position
    chassis.turnToHeading(0, 700);
    correct_position(LB, &chassis, false);
    correct_position(LL, &chassis, true);

    // High Goal #5
    chassis.moveToPoint(-48, -48, 800);
    chassis.turnToPoint(-24, -48, 500, {.forwards = false});
    lift.setState(true);
    chassis.moveToPoint(-24, -48, 1000);
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    // Match Load #4
    chassis.moveToPoint(-48, -48, 700, {.minSpeed = 30, .earlyExitRange = 3});
    matchLoader.setState(true);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(70, -48, 300, {.maxSpeed = 80});
    loadWiggle();

    // High Goal #5
    chassis.moveToPoint(24, -48, 1000, {.forwards = false});
    pros::delay(300);
    correct_position(LL, &chassis, false);
    matchLoader.setState(false);
    chassis.waitUntilDone();
    requestScore(100, 2000);
    pros::delay(2000);

    chassis.moveToPoint(-48, -48, 700);
    chassis.turnToPoint(-45, 0, 600);
    chassis.moveToPoint(-45, 0, 1000);
    lever.setState(true);
    chassis.turnToPoint(-62, 0, 500);

    // Clear Park
    chassis.moveToPoint(-62, 0, 1000, {.minSpeed = 80});
}

void square()
{
    chassis.setPose(-24, -24, 0);
    chassis.moveToPoint(-24, 24, 600, {.minSpeed = 50, .earlyExitRange = 5});
    // chassis.turnToPoint(24, 24, 1000);
    chassis.moveToPoint(24, 24, 1000, {.minSpeed = 50, .earlyExitRange = 5});
    // chassis.turnToPoint(24, -24, 1000);
    chassis.moveToPoint(24, -24, 1200, {.minSpeed = 50, .earlyExitRange = 5});

    chassis.moveToPoint(-24, -24, 1100, {.minSpeed = 50, .earlyExitRange = 5});

    chassis.moveToPoint(-24, 24, 1100, {.minSpeed = 50, .earlyExitRange = 5});
    // chassis.turnToPoint(24, 24, 1000);
    chassis.moveToPoint(24, 24, 1100, {.minSpeed = 50, .earlyExitRange = 5});
    // chassis.turnToPoint(24, -24, 1000);
    chassis.moveToPoint(24, -24, 1100, {.minSpeed = 50, .earlyExitRange = 5});

    chassis.turnToPoint(-24, -24, 1000);
    chassis.moveToPoint(-24, -24, 3000);
    pros::delay(3000);
}