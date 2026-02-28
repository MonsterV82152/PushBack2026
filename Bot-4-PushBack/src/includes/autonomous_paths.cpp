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
    chassis.turnToPoint(0, 0, 500, {false});
    chassis.moveToPoint(-13, 13, 800, {.forwards = false, .maxSpeed = 70});
    matchLoad(false);
    chassis.waitUntilDone();
    scoreAndHold(50);
    pros::delay(1200);
    lowerScoring();
    chassis.moveToPoint(-48, 50, 1800);
    chassis.turnToPoint(-70, 50, 600);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    intake(true);
    matchLoad(true);
    liftToggle(true);
    chassis.waitUntilDone();
    chassis.moveToPoint(-65, 50, 1300, {.maxSpeed = 60});
    chassis.waitUntilDone();
    chassis.arcade(-50, 0);
    pros::delay(100);
    chassis.arcade(50, 0);
    pros::delay(100);
    chassis.arcade(-50, 0);
    pros::delay(100);

    chassis.moveToPoint(-48, 50, 500, {false});
    chassis.waitUntilDone();
    matchLoad(false);
    chassis.turnToPoint(-24, 48, 700, {false});
    chassis.moveToPoint(-24, 48, 500, {.forwards = false});
    chassis.waitUntilDone();
    scoreAndHold();
    chassis.arcade(-20, 0);
    pros::delay(800);
    lowerScoring();
    chassis.moveToPoint(-40, 48, 700);
    chassis.turnToPoint(-34, 39, 700, {false});
    chassis.moveToPoint(-34, 39, 700, {.forwards = false});
    chassis.turnToPoint(-12, 39, 700, {false});
    chassis.moveToPoint(-12, 39, 1400, {.forwards = false, .minSpeed = 127});
    pros::delay(1200);
}
void left2()
{
    chassis.setPose(-46, 0, 0);
    intake(true);
    matchLoad(true);
    chassis.moveToPoint(-46, 47, 2000);
    chassis.turnToPoint(-70, 47, 700);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-65, 47, 200, {.earlyExitRange = 8});
    chassis.moveToPoint(-70, 47, 300, {.maxSpeed = 90});

    chassis.moveToPoint(-24, 47, 2000, {.forwards = false});
    scoreAndHold();
    pros::delay(500);
    lowerScoring();
    matchLoad(false);

    chassis.arcade(-30, -127);
    pros::delay(1000);
    chassis.moveToPoint(-20, 20, 1000);
    pros::delay(500);
    matchLoad(true);
    chassis.turnToPoint(0, 0, 500, {false});
    liftToggle(false);
    chassis.moveToPoint(-11, 11, 1000, {.forwards = false});
    scoreAndHold();
    pros::delay(500);
    lowerScoring();
    liftToggle(true);
    chassis.moveToPoint(-25, 37.5, 1000);
    chassis.turnToPoint(-70, 37.5, 500);
    wingToggle(true);
    chassis.moveToPose(-9, 37.5, 270, 5000, {.forwards = false});
}
void driveOff()
{
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 5, 1000);
}
void soloAWP()
{
    chassis.setPose(-46, -8, 0); // initial position
    liftToggle(true);
    intake(true);

    chassis.moveToPoint(-46, 8, 500);

    // drive to matchloader
    // chassis.moveToPoint(-46, -32, 700, {.forwards = false, .earlyExitRange = 8});
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-46, -48, 1200, {false});
    chassis.waitUntil(24);
    correct_position(LB, &chassis, true);

    // matchload
    chassis.turnToPoint(-70, -48, 700);
    matchLoad(true);
    chassis.moveToPoint(-65, -48, 200, {.earlyExitRange = 8});
    chassis.moveToPoint(-70, -48, 300, {.maxSpeed = 70});
    correct_position(LL, &chassis, false);

    // score
    chassis.arcade(10, 0);
    pros::delay(1000);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -49, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    score();
    pros::delay(1000);
    matchLoad(false);
    intake(true);

    // low goal
    chassis.arcade(-60, 127);
    pros::delay(700);
    move(0, 0);
    pros::delay(100);
    int counter = 0;
    if (correct_position(LB, &chassis, false, false, 24))
    {
        counter += 1;
    }
    if (correct_position(LL, &chassis, true))
    {
        counter += 2;
    }
    master.print(0, 0, "Counter: %d", counter);
    chassis.moveToPoint(-22, -24, 1000);
    chassis.turnToPoint(1, -1, 400);
    chassis.moveToPoint(-11, -13, 500);
    chassis.waitUntilDone();
    reverse(true, 50); // change the number to make scoring lowgoal fast/slow. 0-127
    pros::delay(1000);
    reverse(false);
    intake(true);

    // middle goal
    chassis.moveToPoint(-24, -24, 700, {.forwards = false});
    chassis.turnToPoint(-24, 24, 400);
    chassis.waitUntilDone();
    correct_position(LL, &chassis, true);
    chassis.moveToPoint(-24, 12, 700, {.earlyExitRange = 5});
    chassis.moveToPoint(-24, 24, 500, {.maxSpeed = 80});
    pros::delay(100);
    matchLoad(true);
    liftToggle(false);
    chassis.turnToPoint(0, 0, 700, {false});
    chassis.moveToPoint(-12, 12, 700, {.forwards = false});
    chassis.waitUntilDone();
    matchLoad(false);
    scoreAndHold(60);
    pros::delay(2000);
    lowerScoring();
}
void right() // tuned
{
    chassis.setPose(-48, -17, 90);
    intake(true);
    pros::delay(300);
    chassis.moveToPoint(-24, -24, 700, {.minSpeed = 40, .earlyExitRange = 3});
    chassis.waitUntilDone();
    matchLoad(true);
    chassis.moveToPoint(-14.5, -40, 700, {.minSpeed = 60, .earlyExitRange = 5});
    pros::delay(200);
    matchLoad(false);
    chassis.moveToPoint(-9.5, -55, 1000, {.maxSpeed = 70, .minSpeed = 50, .earlyExitRange = 5});
    chassis.waitUntilDone();
    matchLoad(true);
    chassis.moveToPoint(-29, -62, 1000, {.maxSpeed = 70, .minSpeed = 50, .earlyExitRange = 5});
    chassis.moveToPoint(-48, -48, 700);
    chassis.turnToPoint(-24, -48, 700, {false});
    chassis.moveToPoint(-24, -48, 1000, {false});
    liftToggle(true);
    chassis.waitUntilDone();
    score(100);
    chassis.arcade(-30, 0);
    correct_position(LL, &chassis, false);
    pros::delay(1200);
    chassis.moveToPoint(-70, -48, 1700, {.maxSpeed = 50}); // matchload
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
void right2()
{
    chassis.setPose(-46, -2, 0);
    liftToggle(true);
    intake(true);
    chassis.moveToPoint(-46, -32, 700, {.forwards = false, .earlyExitRange = 8});
    chassis.moveToPoint(-46, -47, 550, {.forwards = false, .maxSpeed = 90});
    correct_position(LL, &chassis, false);
    chassis.turnToPoint(-70, -47, 700);
    matchLoad(true);
    chassis.moveToPoint(-70, -47, 200, {.earlyExitRange = 8});
    chassis.moveToPoint(-70, -47, 1000, {.maxSpeed = 90});
    pros::delay(1000);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -47, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    score(100);
    pros::delay(1000);
    matchLoad(false);
    intake(true);
    chassis.arcade(-70, 127);
    pros::delay(550);
    chassis.moveToPoint(-24, -24, 1000);
    chassis.turnToPoint(0, 0, 400);
    chassis.moveToPoint(-11, -11, 500);
    chassis.waitUntilDone();
    reverse(true, 60);
    pros::delay(1500);
}

// void skills()
// {
//     // chassis.setPose(-48, 15, 0);
//     // intake(true);
//     // liftToggle(true);
//     // matchLoad(true);

//     // chassis.moveToPoint(-48, 48, 1000);
//     // chassis.turnToPoint(-70, 48, 700);
//     // correct_position(LR, &chassis, false);
//     // chassis.moveToPoint(-65, 48, 2000, {.maxSpeed = 70}, true);
//     // pros::delay(500);
//     // // chassis.turnToPoint(-70, 50, 200);
//     // // chassis.turnToPoint(-70, 46, 200);
//     // // chassis.turnToPoint(-70, 50, 200);
//     // // chassis.turnToPoint(-70, 46, 200);
//     // // chassis.turnToPoint(-70, 50, 200);
//     // // chassis.turnToPoint(-70, 48, 200);

//     // chassis.moveToPoint(-48, 48, 1000, {.forwards = false});
//     // chassis.turnToPoint(-26.5, 60, 700, {false});
//     // chassis.moveToPoint(-26.5, 60, 1000, {.forwards = false});
//     // moveTo(37, 60, 700, 3000, false, false);
//     // correct_position(LR, &chassis, false);
//     // matchLoad(false);
//     // moveTo(37, 46, 700, 1000, false, false);
//     // // moveTo(24, 46, 1000, 1500, false, false);
//     // chassis.turnToPoint(24, 48, 700, {false});
//     // chassis.waitUntilDone();
//     // correct_position(LL, &chassis, false);
//     // chassis.moveToPoint(24, 48, 1000, {.forwards = false});
//     // chassis.waitUntilDone();
//     // scoreAndHold(80);
//     // pros::delay(1000);
//     // lowerScoring();
//     // pros::delay(250);
//     // intake(true);
//     // matchLoad(true);
//     // correct_position(LL, &chassis, false);
//     // chassis.moveToPoint(70, 48, 2500, {.maxSpeed = 70}, true);
//     // pros::delay(500);
//     // // chassis.turnToPoint(70, 50, 200);
//     // // chassis.turnToPoint(70, 46, 200);
//     // // chassis.turnToPoint(70, 50, 200);
//     // // chassis.turnToPoint(70, 46, 200);
//     // // chassis.turnToPoint(70, 50, 200);
//     // // chassis.turnToPoint(70, 48, 200);

//     // chassis.moveToPoint(24, 48, 2000, {.forwards = false, .maxSpeed = 70});
//     // chassis.waitUntilDone();
//     // scoreAndHold(80);
//     // pros::delay(1000);
//     // lowerScoring();
//     // matchLoad(false);
//     // moveTo(48, 48, 500, 1000, true, true);
//     // moveTo(36, 0, 700, 3000, true, true);
//     // intake(true);
//     // // moveTo(65, 0, 700, 3000, true, true);
//     chassis.setPose(36, 0, 90);
//     intake(true);
//     liftToggle(true);
//     chassis.turnToPoint(65, 0, 700);
//     chassis.moveToPoint(65, 0, 2000, {.maxSpeed = 80, .minSpeed = 80});
//     chassis.waitUntilDone();
//     pros::delay(500);
//     chassis.turnToPoint(40, 0, 700, {false});
//     chassis.moveToPoint(40, 0, 2300, {.forwards = false, .minSpeed = 80});
//     chassis.turnToPoint(65, 0, 700);
//     chassis.moveToPoint(65, 0, 2000, {.maxSpeed = 80, .minSpeed = 80});
//     chassis.waitUntilDone();
//     pros::delay(500);
//     // moveTo(40, 0, 700, 3000, false, false);
//     // moveTo(65, 0, 700, 3000, true, true);

//     chassis.moveToPoint(36, 0, 1000, {.forwards = false});
//     // chassis.turnToPoint(36, 36, 700);
//     chassis.turnToHeading(0, 1000);
//     chassis.waitUntilDone();
//     correct_position(LR, &chassis, true, true);
//     chassis.moveToPoint(36, 36, 1200);//1000
//     // chassis.turnToPoint(0, 36, 700);
//     chassis.turnToHeading(270, 1000);
//     chassis.waitUntilDone();
//     correct_position(LR, &chassis, false, true);
//     chassis.turnToPoint(10, 6.5, 700);
//     chassis.moveToPoint(10, 6.5, 2000);
//     intakeLiftToggle(true);
//     reverse(true, 40);
//     pros::delay(2000);
//     chassis.moveToPoint(24, 24, 2000, {.forwards = false});
//     moveTo(40, -48, 1000, 5000, true, true);
//     reverse(false);
//     intake(true);
//     intakeLiftToggle(false);
//     matchLoad(true);

//     chassis.turnToPoint(70, -48, 1000);
//     chassis.waitUntilDone();
//     correct_position(LR, &chassis, false);
//     chassis.moveToPoint(65, -48, 2000, {.maxSpeed = 70}, true);
//     pros::delay(1000);
//     // chassis.turnToPoint(70, -50, 200);
//     // chassis.turnToPoint(70, -46, 200);
//     // chassis.turnToPoint(70, -50, 200);
//     // chassis.turnToPoint(70, -46, 200);
//     // chassis.turnToPoint(70, -50, 200);
//     // chassis.turnToPoint(70, -48, 200);

//     chassis.moveToPoint(40, -48, 1500, {.forwards = false});
//     moveTo(36, -58.5, 700, 1000, false, false);
//     // moveTo(-37, -0, 700, 2000, false, false);
//     chassis.turnToPoint(36, -60, 700, {false});
//     chassis.waitUntilDone();
//     correct_position(LR, &chassis, false);
//     chassis.moveToPoint(36, -60, 1000, {.forwards = false});
//     //chassis.turnToPoint(-48, -60, 700); //add to make it smoother
//     //chassis.moveToPoint(-48, -60, 700);
//     matchLoad(false);
//     moveTo(-48, -48, 700, 1000, false, false);
//     correct_position(LR, &chassis, false);
//     chassis.turnToPoint(-70, -48, 700);
//     chassis.waitUntilDone();
//     correct_position(LL, &chassis, false);
//     chassis.moveToPoint(-24, -48, 1000, {.forwards = false});
//     chassis.waitUntilDone();
//     scoreAndHold(80);
//     pros::delay(1000);
//     lowerScoring();
//     matchLoad(true);

//     correct_position(LL, &chassis, false);
//     chassis.moveToPoint(-70, -48, 2000, {.maxSpeed = 70}, true);
//     pros::delay(500);
//     // chassis.turnToPoint(-70, -50, 200);
//     // chassis.turnToPoint(-70, -46, 200);
//     // chassis.turnToPoint(-70, -50, 200);
//     // chassis.turnToPoint(-70, -46, 200);
//     // chassis.turnToPoint(-70, -50, 200);
//     // chassis.turnToPoint(-70, -48, 200);
//     correct_position(LL, &chassis, false);
//     chassis.moveToPoint(-24, -48, 1000, {.forwards = false, .maxSpeed = 70});

//     chassis.waitUntilDone();
//     scoreAndHold(80);
//     pros::delay(1000);
//     lowerScoring();
//     matchLoad(false);

//     chassis.moveToPoint(-48, -48, 1000);
//     moveTo(-62, -24, 1000, 3000, true, true);
//     intake(true);
//     chassis.turnToPoint(-66, 0, 1000);
//     chassis.moveToPoint(-66, 0, 3000);
// }

void skills()
{
    chassis.setPose(-46, 0, 90);
    intake(true);
    chassis.moveToPoint(-40, 0, 700);
    chassis.turnToPoint(-24, 24, 700);
    chassis.moveToPoint(-24, 24, 1500);
    chassis.turnToPoint(0, 0, 700, {false});
    liftToggle(false);
    chassis.moveToPoint(-12, 12, 800, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    score(46);
    pros::delay(1000);
    liftToggle(true);
    chassis.moveToPoint(-48, 48, 1500);
    chassis.turnToPoint(-70, 48, 600);
    matchLoad(true);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-70, 48, 800, {.maxSpeed = 67});
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    pros::delay(1400);
    chassis.moveToPoint(-24, 64, 1500, {.forwards = false});
    chassis.turnToPoint(24, 63, 600, {false});
    matchLoad(false);
    chassis.moveToPoint(40, 63, 1800, {.forwards = false});
    liftToggle(true);
    chassis.turnToPoint(40, 48, 600);
    chassis.waitUntilDone();
    correct_position(LL, &chassis, true);
    correct_position(LB, &chassis, false);
    chassis.moveToPoint(40, 48, 1000);
    chassis.turnToPoint(24, 49, 700, {false});
    chassis.waitUntilDone();
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(24, 49, 800, {.forwards = false});
    chassis.waitUntilDone();
    move(-10, -10);
    score([&](double position)
          { return powf(((position - SCORE_ANGLE) / (DOWN_ANGLE - SCORE_ANGLE)), 5) * 95 + 35; });
    pros::delay(1600);
    chassis.moveToPoint(70, 48, 1300, {.maxSpeed = 67});
    matchLoad(true);
    correct_position(LL, &chassis, false);
    chassis.waitUntilDone();
    pros::delay(2000);
    chassis.moveToPoint(24, 49, 1300, {.forwards = false, .maxSpeed = 70});
    chassis.waitUntilDone();
    move(-10, -10);
    score([&](double position)
          { return powf(((position - SCORE_ANGLE) / (DOWN_ANGLE - SCORE_ANGLE)), 5) * 95 + 35; });
    matchLoad(false);
    lemlib::Pose currentPose = chassis.getPose();
    chassis.setPose(31.5, 48, currentPose.theta);
    pros::delay(1600);
    chassis.moveToPoint(48, 48, 1000);
    // chassis.turnToPoint(40, 0, 700);
    // chassis.moveToPoint(40, 0, 1500, {.maxSpeed = 90});
    // chassis.turnToPoint(70, 0, 800);
    // intake(true);
    // // chassis.moveToPoint(70, 0, 4000, {.minSpeed = 80});
    // chassis.waitUntilDone();
    // chassis.arcade(80,0);
    // pros::delay(1000);
    // chassis.arcade(40,0);
    // pros::delay(1000);
    // chassis.moveToPoint(-46, 0, 10000, {.forwards = false});
    // matchLoad(true);
    // pros::delay(650);
    // while (1) {
    //     if (LOCF.get() >= 600) {
    //         chassis.cancelMotion();
    //         break;
    //     }
    //     pros::delay(20);
    // }
    // pros::delay(500);

    // correct_position(LF, &chassis, false);
    // matchLoad(false);
    chassis.turnToPoint(68, 28, 700);
    chassis.moveToPoint(68, 28, 1500);
    chassis.turnToPoint(74, -24, 700);
    chassis.waitUntilDone();
    correct_position(LL, &chassis, true);
    move(67, 67);
    pros::delay(1800);
    move(0, 0);
    correct_position(LL, &chassis, true);
    move(-30, -30);
    pros::delay(1000);
    move(0, 0);
    currentPose = chassis.getPose();
    chassis.setPose(currentPose.x, -20, currentPose.theta);
    chassis.moveToPoint(66, -28, 500);

    chassis.turnToPoint(28, -24, 700, {false});
    chassis.moveToPoint(28, -24, 1000, {.forwards = false});
    liftToggle(false);
    chassis.turnToPoint(4, 4, 700, {false});
    chassis.moveToPoint(16, -10, 1500, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    scoreAndHold([&](double position)
                 { return powf(((position - SCORE_ANGLE) / (DOWN_ANGLE - SCORE_ANGLE)), 5) * 102 + 25; });
    pros::delay(3000);
    chassis.moveToPoint(48, -46, 1500);
    pros::delay(500);
    lowerScoring();
    chassis.turnToPoint(70, -46, 600);
    liftToggle(true);
    matchLoad(true);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(70, -48, 1000, {.maxSpeed = 67});
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    pros::delay(2000);
    chassis.moveToPoint(24, -66, 1500, {.forwards = false});
    chassis.turnToPoint(-24, -64, 600, {false});
    matchLoad(false);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-36, -64, 1500, {.forwards = false});
    chassis.turnToPoint(-36, -48, 600);
    chassis.waitUntilDone();
    correct_position(LL, &chassis, true);
    correct_position(LB, &chassis, false);
    chassis.moveToPoint(-42, -50, 1000);
    chassis.waitUntilDone();
    correct_position(LB, &chassis, false);
    chassis.turnToPoint(-24, -50, 600, {false});
    chassis.moveToPoint(-24, -50, 1000, {.forwards = false});
    chassis.waitUntilDone();
    move(-10, -10);
    score([&](double position)
          { return powf(((position - SCORE_ANGLE) / (DOWN_ANGLE - SCORE_ANGLE)), 5) * 95 + 35; });
    pros::delay(1600);
    matchLoad(true);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-70, -48, 1300, {.maxSpeed = 64});
    chassis.waitUntilDone();
    move(-10, -10);
    pros::delay(200);
    move(20, 20);
    pros::delay(400);
    move(0, 0);
    pros::delay(1000);
    chassis.moveToPoint(-24, -49, 1500, {.forwards = false, .maxSpeed = 70});
    chassis.waitUntilDone();
    move(-10, -10);
    score([&](double position)
          { return powf(((position - SCORE_ANGLE) / (DOWN_ANGLE - SCORE_ANGLE)), 5) * 95 + 35; });
    matchLoad(false);
    currentPose = chassis.getPose();
    chassis.setPose(-31.5, -48, currentPose.theta);
    pros::delay(1600);
    chassis.moveToPoint(-48, -48, 800);
    chassis.turnToPoint(-68, -24, 500);
    chassis.moveToPoint(-68, -24, 1000);
    chassis.turnToPoint(-74, 0, 400);
    chassis.waitUntilDone();
    move(67, 67);
    pros::delay(1300);
    move(0, 0);
}

void sawp4()
{
    chassis.setPose(-48, 0, 0);
    liftToggle(true);
    intake(true);
    chassis.moveToPoint(-46, 5, 400); // push
    chassis.moveToPoint(-46, -32, 700, {.forwards = false, .earlyExitRange = 8});
    chassis.moveToPoint(-46, -48, 550, {.forwards = false, .maxSpeed = 90});
    correct_position(LL, &chassis, false);
    chassis.turnToPoint(-70, -48, 700);
    matchLoad(true);
    chassis.moveToPoint(-60, -48, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-70, -48, 300, {.maxSpeed = 80});
    chassis.arcade(-30, 0);
    pros::delay(1000);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -48, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    score();
    matchLoad(false);
    pros::delay(700);
    intake(true);
    chassis.arcade(-70, 127);
    pros::delay(550);
    chassis.moveToPoint(-24, -24, 1000);
    chassis.turnToPoint(0, 0, 400);
    chassis.moveToPoint(-11, -11, 500);
    chassis.waitUntilDone();
    reverse(true, 50);
    pros::delay(500);
    reverse(false);
    intake(true);
    chassis.moveToPoint(-24, -24, 700, {.forwards = false});
    chassis.turnToPoint(-24, 24, 400);
    chassis.moveToPoint(-24, 12, 700, {.earlyExitRange = 5});
    chassis.moveToPoint(-24, 24, 500, {.maxSpeed = 80});
    pros::delay(100);
    matchLoad(true);
    liftToggle(false);
    chassis.turnToPoint(0, 0, 600, {false});
    chassis.moveToPoint(-12, 12, 700, {.forwards = false});
    chassis.waitUntilDone();
    score(60);
    pros::delay(500);
    chassis.moveToPoint(-48, 48, 2000);
    chassis.waitUntilDone();

    liftToggle(true);
    chassis.turnToPoint(-70, 48, 500);
    chassis.waitUntilDone();
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-60, 48, 200, {.earlyExitRange = 3});
    chassis.moveToPoint(-70, 48, 700, {.maxSpeed = 80});
    chassis.moveToPoint(-24, 48, 800, {.forwards = false});
    chassis.waitUntilDone();
    score();
}

void test()
{
    chassis.arcade(20, 0);
    pros::delay(300);
    chassis.arcade(0, 0);
}