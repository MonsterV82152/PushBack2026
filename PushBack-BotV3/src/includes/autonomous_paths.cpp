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

void left3()
{
}

void printPose()
{
}

void soloAWP()
{
    robot.setPose(-48, -9, 0);
    helper.descore.setState(true);
    correct_position(leftDistanceSensor, &locator, true);
    robot.intake();
    robot.moveToPoint(-48, 15, 500);
    pros::delay(200);
    helper.descore.setState(false);
    robot.moveToPoint(-48, -48, 1200, {false});
    robot.waitUntilDone();
    // correct_position(backDistanceSensor, &locator, false);
    robot.turnToPoint(-70, -48, 500);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 900);
    robot.waitUntilDone();
    helper.leftDT.move(90);
    helper.rightDT.move(90);
    pros::delay(300);
    robot.moveToPoint(-48, -47.5, 600, {false});
    robot.matchLoad(false);
    robot.turnToHeading(90, 800);
    pros::delay(500);
    robot.lift();
    robot.moveToPoint(-28, -47.5, 1000);
    pros::delay(300);
    robot.score(1750);
    robot.waitUntilDone();
    pros::delay(800);
    robot.lift(false);
    robot.setLow();
    robot.moveToPoint(-48, -47.5, 500, {false});
    pros::delay(300);
    robot.intake();
    robot.turnToPoint(-24, -24, 500);
    robot.moveToPoint(-24, -24, 900);
    robot.moveToPoint(-10, -8, 1000);
    pros::delay(700);
    robot.scoreLow(127);
    robot.moveState({ON, LEAVE, -100, LEAVE});
    pros::delay(1000);
    robot.moveToPoint(-24, -24, 700, {false});
    robot.turnToPoint(-24, 24, 700);
    robot.reset();
    robot.lowerIntake();
    robot.intake();
    robot.moveToPoint(-24, 24, 1100, {.minSpeed = 30});
    pros::delay(1000);
    robot.matchLoad(true);
    robot.turnToPoint(-10, 11, 700);
    robot.moveToPoint(-10, 11, 1000);
    // robot.matchLoad(false);
    pros::delay(200);
    robot.score(1770);
}

void right()
{
    robot.setPose(-48, -17.25, 90);
    helper.descore.setState(true);
    // robot.moveToPoint(-36, -16, 700, {.minSpeed = 40, .earlyExitRange = 5.0});
    pros::delay(200);
    helper.descore.setState(false);
    // robot.turnToPoint(-24, -26, 700);
    robot.intake();
    robot.moveToPoint(-24, -26, 700, {.minSpeed = 30, .earlyExitRange = 5.0});
    // robot.turnToPoint(-7, -46, 700);
    robot.moveToPoint(-8.5, -46, 1000, {.curve = 1});
    robot.waitUntilDone();
    correct_position(frontDistanceSensor, &locator, false);
    robot.turnToPoint(-24, -30, 500, {false});
    robot.moveToPoint(-24, -30, 700, {false});
    robot.turnToPoint(-48, -48.5, 500, {false});
    robot.moveToPoint(-48, -48.5, 800, {false});
    robot.turnToPoint(-70, -48.5, 700);
    robot.waitUntilDone();
    correct_position(frontDistanceSensor, &locator, true);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48.5, 1200);
    robot.moveToPoint(-48, -48.5, 900, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48.5, 900);
    robot.moveToPoint(-28, -48.5, 700);
    robot.lift();
    robot.waitUntilDone();
    robot.setScoringAction(ScoringAction::SCOREANDHOLD);
}

void right2()
{
    robot.setPose(-48, -17.25, 90);
    helper.descore.setState(true);
    // robot.moveToPoint(-36, -16, 700, {.minSpeed = 40, .earlyExitRange = 5.0});
    pros::delay(200);
    helper.descore.setState(false);
    // robot.turnToPoint(-24, -26, 700);
    robot.intake();
    robot.moveToPoint(-24, -26, 700, {.minSpeed = 30, .earlyExitRange = 5.0});
    // robot.turnToPoint(-7, -46, 700);
    robot.moveToPoint(-8.5, -46, 1000, {.curve = 1});
    robot.waitUntilDone();
    correct_position(frontDistanceSensor, &locator, false);
    robot.turnToPoint(-24, -24, 500, {false});
    robot.moveToPoint(-24, -24, 900, {false});
    robot.turnToPoint(-9, -9, 700);
    robot.moveToPoint(-9, -9, 700);
    robot.waitUntilDone();
    robot.matchLoad(true);
    robot.raiseIntake();
    robot.moveState({ON, LEAVE, -80, LEAVE});
    pros::delay(200);
    robot.matchLoad(false);
    pros::delay(500);
    robot.turnToPoint(-48, -48, 1000, {false});
    pros::delay(200);
    robot.lowerIntake();
    robot.intake();
    robot.moveToPoint(-48, -48, 800, {false});
    robot.turnToPoint(-70, -47.5, 800);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -47.5, 1200);
    correct_position(frontDistanceSensor, &locator, true);
    correct_position(leftDistanceSensor, &locator, false);
    robot.moveToPoint(-48, -48, 900, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48, 900);
    robot.moveToPoint(-28, -48, 700);
    robot.lift();
    robot.waitUntilDone();
    robot.setScoringAction(ScoringAction::SCOREANDHOLD);
}

void right3()
{
    // robot.setPose(-48, -17.25, 90);
    // robot.lift();
    // // robot.moveToPoint(-36, -16, 700, {.minSpeed = 40, .earlyExitRange = 5.0});
    // pros::delay(200);
    // robot.lift(false);
    // // robot.turnToPoint(-24, -26, 700);
    // robot.intake();
    // robot.moveToPoint(-24, -26, 700, {.minSpeed = 30, .earlyExitRange = 5.0});
    // // robot.turnToPoint(-7, -46, 700);
    // robot.moveToPoint(-7, -46, 1000, {.curve = 1});
    // robot.waitUntilDone();
    // correct_position(rightDistanceSensor, &locator, false);
    // robot.moveToPoint(-24, -60, 700, {.minSpeed = 30, .earlyExitRange = 5.0});
    // robot.turnToPoint(-48, -48, 500);
    // robot.moveToPoint(-48, -48, 1000, {false});
    // robot.turnToPoint(-70, -48, 700);
    // robot.waitUntilDone();
    // correct_position(frontDistanceSensor, &locator, true);
    // robot.matchLoad(true);
    // robot.moveToPoint(-70, -48, 1200);
    // robot.moveToPoint(-48, -48, 900, {false});
    // robot.matchLoad(false);
    // robot.turnToPoint(-28, -48, 900);
    // robot.moveToPoint(-28, -48, 700);
    // robot.lift();
    // robot.waitUntilDone();
    // robot.score();
}

void move(int speed)
{
    helper.leftDT.move(speed);
    helper.rightDT.move(speed);
}

void skills()
{
    robot.setPose(-50, 0, 270);
    robot.lift();
    pros::delay(500);
    robot.lift(false);
    robot.setLow();
    pros::delay(200);
    robot.intake();
    // getting out of park
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

    // incase if balls are missed
    pros::delay(600);

    // move to high tube
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 900);
    pros::delay(500);
    robot.intake(false);
    robot.turnToPoint(-9, -9, 700);
    robot.moveToPoint(-9, -9, 1100);
    robot.intake();
    robot.waitUntilDone();
    pros::delay(100);

    // score to high tube
    robot.moveState({ON, LEAVE, -30, LEAVE});
    robot.scoreLow(50);
    pros::delay(3000);
    robot.matchLoad(false);
    robot.reset();
    robot.moveToPoint(-46, -48, 1500, {false});

    // bottom left match load
    robot.turnToPoint(-70, -48, 900);
    robot.lowerIntake();
    robot.intake();
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);

    // move to bottom tube
    robot.moveToPoint(-52, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-24, -31, 700, {false});
    robot.moveToPoint(-24, -31, 800, {false});
    robot.turnToPoint(24, -31, 400, {false});
    robot.moveToPoint(24, -31, 1100, {false});
    robot.turnToPoint(48, -48, 400, {false});
    robot.moveToPoint(48, -48, 900, {false});
    robot.turnToPoint(28, -48, 600);
    robot.moveToPoint(28, -48, 1000);

    // score to bottom tube
    robot.lift();
    robot.descore(false);
    pros::delay(700);
    robot.score(1750);
    pros::delay(1500);
    robot.lift(false);

    // bottom right match load
    robot.intake();
    robot.moveToPoint(44, -48, 1000, {false});
    robot.turnToPoint(70, -48, 1000);
    robot.matchLoad(true);
    robot.moveToPoint(70, -48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);

    // move to bottom tube again
    robot.moveToPoint(48, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(28, -48, 1000);
    robot.moveToPoint(28, -48, 1000);

    // score to bottom tube
    robot.lift();
    robot.descore(false);
    pros::delay(700);
    robot.score(1750);
    pros::delay(1500);
    robot.lift(false);

    // right side balls
    robot.intake();
    robot.moveToPoint(44, -48, 800, {false});

    // top right match load
    robot.turnToPoint(44, 48, 800);
    robot.moveToPoint(48, 48, 2500);
    robot.turnToPoint(70, 48, 700);
    robot.matchLoad(true);
    robot.intake();
    robot.moveToPoint(70, 48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);

    // move to top tube
    robot.moveToPoint(52, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(24, 32, 700, {false});
    robot.moveToPoint(24, 32, 800, {false});
    robot.turnToPoint(-24, 32, 700, {false});
    robot.moveToPoint(-24, 32, 1200, {false});
    robot.turnToPoint(-48, 48, 700, {false});
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 1000);

    // score to top tube
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);

    // top left match load
    robot.intake();
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-70, 48, 900);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    robot.moveToPoint(-48, 48, 700, {false});

    // move to top tube again
    robot.matchLoad(false);
    robot.turnToPoint(-28, 48, 700);
    robot.moveToPoint(-28, 48, 700);

    // score to top tube again
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);

    // park
    robot.intake();
    robot.moveToPoint(-48, 48, 1000, {false});
    robot.turnToPoint(-56, 24, 700);
    robot.moveToPoint(-56, 24, 1000);
    robot.turnToPoint(-60, 0, 700);
    robot.waitUntilDone();
    move(80);
    pros::delay(1500);
    move(0);
}

void skills2()
{
    robot.setPose(-50, 0, 270);
    robot.lift();
    pros::delay(500);
    robot.lift(false);
    robot.setLow();
    pros::delay(200);
    robot.intake();
    // getting out of park
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

    // incase if balls are missed
    pros::delay(600);

    // move to high tube
    robot.turnToPoint(-24, -24, 700);
    robot.moveToPoint(-24, -24, 900);
    pros::delay(500);
    robot.intake(false);
    robot.turnToPoint(-9, -9, 700);
    robot.moveToPoint(-9, -9, 1100);
    robot.intake();
    robot.waitUntilDone();
    pros::delay(100);

    // score to high tube
    robot.moveState({ON, LEAVE, -30, LEAVE});
    robot.scoreLow(50);
    pros::delay(3000);
    robot.matchLoad(false);
    robot.reset();
    robot.moveToPoint(-24, -24, 1000, {false});
    robot.turnToPoint(-24, 24, 1000);
    robot.lowerIntake();
    robot.intake();
    robot.moveToPoint(-24, 24, 1200);

    // top left match load
    robot.turnToPoint(-48, 48, 900);
    robot.moveToPoint(-48, 48, 1000);
    robot.turnToPoint(-70, 48, 900);
    robot.matchLoad(true);
    robot.moveToPoint(-70, 48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(60);
    pros::delay(200);

    // move to bottom tube
    robot.moveToPoint(-52, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(-24, 31, 700, {false});
    robot.moveToPoint(-24, 31, 800, {false});
    robot.turnToPoint(24, 31, 400, {false});
    robot.moveToPoint(24, 31, 1100, {false});
    robot.turnToPoint(48, 48, 400, {false});
    robot.moveToPoint(48, 48, 900, {false});
    robot.turnToPoint(28, 48, 600);
    robot.moveToPoint(28, 48, 1000);
    // score to bottom tube
    robot.lift();
    robot.descore(false);
    pros::delay(700);
    robot.score(1750);
    pros::delay(1500);
    robot.lift(false);

    // bottom right match load
    robot.intake();
    robot.moveToPoint(44, 48, 1000, {false});
    robot.turnToPoint(70, 48, 1000);
    robot.matchLoad(true);
    robot.moveToPoint(70, 48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);

    // move to bottom tube again
    robot.moveToPoint(48, 48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(28, 48, 1000);
    robot.moveToPoint(28, 48, 1000);

    // score to bottom tube
    robot.lift();
    robot.descore(false);
    pros::delay(700);
    robot.score(1750);
    pros::delay(1500);
    robot.lift(false);

    // right side balls
    robot.intake();
    robot.moveToPoint(44, 48, 800, {false});
    robot.turnToPoint(60, 24, 700);
    robot.moveToPoint(60, 24, 1000);

    // top right match load
    robot.turnToPoint(60, -24, 800);
    robot.waitUntilDone();
    robot.matchLoad();
    move(100);
    pros::delay(2000);

    robot.turnToPoint(48, -48, 800);
    robot.moveToPoint(48, -48, 2500);
    robot.turnToPoint(70, -48, 700);
    robot.matchLoad(true);
    robot.intake();
    robot.moveToPoint(70, -48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);

    // move to top tube
    robot.moveToPoint(52, -48, 700, {false});
    robot.matchLoad(false);
    robot.turnToPoint(24, -32, 700, {false});
    robot.moveToPoint(24, -32, 800, {false});
    robot.turnToPoint(-24, -32, 700, {false});
    robot.moveToPoint(-24, -32, 1200, {false});
    robot.turnToPoint(-48, -48, 700, {false});
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-28, -48, 700);
    robot.moveToPoint(-28, -48, 1000);

    // score to top tube
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);

    // top left match load
    robot.intake();
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-70, -48, 900);
    robot.matchLoad(true);
    robot.moveToPoint(-70, -48, 1000, {.maxSpeed = 100});
    robot.waitUntilDone();
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    move(-20);
    pros::delay(100);
    move(80);
    pros::delay(200);
    robot.moveToPoint(-48, -48, 700, {false});

    // move to top tube again
    robot.matchLoad(false);
    robot.turnToPoint(-28, -48, 700);
    robot.moveToPoint(-28, -48, 700);

    // score to top tube again
    pros::delay(400);
    robot.lift();
    robot.waitUntilDone();
    robot.score(1740);
    pros::delay(1500);
    robot.lift(false);

    // park
    robot.intake();
    robot.moveToPoint(-48, -48, 1000, {false});
    robot.turnToPoint(-56, -24, 700);
    robot.moveToPoint(-56, -24, 1000);
    robot.turnToPoint(-60, 0, 700);
    robot.waitUntilDone();
    move(80);
    pros::delay(1500);
    move(0);
}

void test()
{
    move(50);
    pros::delay(500);
    move(0);
}