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
    chassis.arcade(-50,0);
    pros::delay(100);
    chassis.arcade(50,0);
    pros::delay(100);
    chassis.arcade(-50,0);
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
void left2() {
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
void driveOff() {
    chassis.setPose(0, 0, 0); 
    chassis.moveToPoint(0, 5, 1000);
}
void soloAWP() {
    chassis.setPose(-46, 0, 0); // initial position
    liftToggle(true);
    intake(true);

    //chassis.moveToPoint(-46, 8, 800);


    //drive to matchloader
    chassis.moveToPoint(-46, -32, 700, {.forwards = false, .earlyExitRange = 8});
    chassis.moveToPoint(-46, -47, 550, {.forwards = false, .maxSpeed = 90});


    //matchload
    correct_position(LL, &chassis, false);
    chassis.turnToPoint(-70, -47, 700);
    matchLoad(true);
    chassis.moveToPoint(-65, -47, 200, {.earlyExitRange = 8});
    chassis.moveToPoint(-70, -47, 300, {.maxSpeed = 90});

    //score
    chassis.arcade(-30, 0);
    pros::delay(1000);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -47, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    score(100);
    pros::delay(1000);
    matchLoad(false);
    intake(true);

    //low goal
    chassis.arcade(-70, 127);
    pros::delay(550);
    chassis.moveToPoint(-24, -24, 1000);
    chassis.turnToPoint(0, 0, 400);
    chassis.moveToPoint(-11, -11, 500);
    chassis.waitUntilDone();
    reverse(true, 50); //change the number to make scoring lowgoal fast/slow. 0-127
    pros::delay(1000);
    reverse(false);
    intake(true);

    //middle goal
    chassis.moveToPoint(-24, -24, 700, {.forwards = false});
    chassis.turnToPoint(-24, 24, 400);
    chassis.moveToPoint(-24, 12, 700, {.earlyExitRange = 5});
    chassis.moveToPoint(-24, 24, 500, {.maxSpeed = 80});
    pros::delay(100);
    matchLoad(true);
    liftToggle(false);
    chassis.turnToPoint(0, 0, 700, {false});
    chassis.moveToPoint(-12, 12, 700, {.forwards = false});
    chassis.waitUntilDone();
    scoreAndHold(60);
    pros::delay(2000);
    lowerScoring();
    

}
void right() //tuned
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
void right2() {
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

void skills() {
    chassis.setPose(-46.5, 0, 0);
    intake(true);
    liftToggle(true);
    matchLoad(true);
    
    chassis.moveToPoint(-48, 46, 5000);
    chassis.turnToPoint(-70, 46, 1000);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(-65, 46, 5000, {.maxSpeed = 70}, true);
    pros::delay(500);
    // chassis.turnToPoint(-70, 50, 200);
    // chassis.turnToPoint(-70, 46, 200);
    // chassis.turnToPoint(-70, 50, 200);
    // chassis.turnToPoint(-70, 46, 200);
    // chassis.turnToPoint(-70, 50, 200);
    // chassis.turnToPoint(-70, 48, 200);

    chassis.moveToPoint(-48, 46, 3000, {.forwards = false});
    chassis.turnToPoint(-26.5, 58.5, 1000, {false});
    chassis.moveToPoint(-26.5, 58.5, 2000, {.forwards = false});
    moveTo(37, 58.5, 1000, 5000, false, false);
    correct_position(LR, &chassis, false);
    matchLoad(false);
    moveTo(37, 46, 1000, 1000, false, false);
    moveTo(24, 46, 1000, 1500, false, false);
    chassis.waitUntilDone();
    scoreAndHold(80);
    pros::delay(250);
    lowerScoring();
    pros::delay(250);
    intake(true);
    matchLoad(true);
    chassis.moveToPoint(70, 46, 5000, {.maxSpeed = 70}, true);
    pros::delay(500);
    // chassis.turnToPoint(70, 50, 200);
    // chassis.turnToPoint(70, 46, 200);
    // chassis.turnToPoint(70, 50, 200);
    // chassis.turnToPoint(70, 46, 200);
    // chassis.turnToPoint(70, 50, 200);
    // chassis.turnToPoint(70, 48, 200);

    chassis.moveToPoint(24, 46, 3000, {.forwards = false});
    chassis.waitUntilDone();
    scoreAndHold(80);
    pros::delay(250);
    lowerScoring();
    matchLoad(false);
    moveTo(48, 48, 500, 1000, true, true);
    moveTo(36, 0, 1000, 5000, true, true);
    intake(true);
    moveTo(65, 0, 1000, 3500, true, true);
    moveTo(40, 0, 1000, 4000, false, false);
    moveTo(65, 0, 1000, 5000, true, true);

    chassis.moveToPoint(36, 0, 3000, {.forwards = false});
    chassis.turnToPoint(36, 36, 1000);
    correct_position(LR, &chassis, true);
    chassis.moveToPoint(36, 36, 3000);
    chassis.turnToPoint(0, 36, 1000);
    correct_position(LR, &chassis, false);
    chassis.turnToPoint(16, 16, 1500);
    chassis.moveToPoint(14, 14, 3000);
    reverse(true, 40);
    pros::delay(2000);
    chassis.moveToPoint(16, 16, 3000, {.forwards = false});
    moveTo(48, -48, 1000, 5000, true, true);
    intake(true);
    matchLoad(true);

    chassis.turnToPoint(70, -46, 1000);
    correct_position(LR, &chassis, false);
    chassis.moveToPoint(65, -46, 4000, {.maxSpeed = 70}, true);
    pros::delay(500);
    // chassis.turnToPoint(70, -50, 200);
    // chassis.turnToPoint(70, -46, 200);
    // chassis.turnToPoint(70, -50, 200);
    // chassis.turnToPoint(70, -46, 200);
    // chassis.turnToPoint(70, -50, 200);
    // chassis.turnToPoint(70, -48, 200);
    
    chassis.moveToPoint(48, -46, 1500, {.forwards = false});
    moveTo(36, -58.5, 1000, 3000, false, false);
    moveTo(-37, -58.5, 1000, 4000, false, false);
    matchLoad(false);
    moveTo(-48, -48, 1000, 2000, false, false);
    chassis.turnToPoint(-70, -46, 1000);
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-24, -46, 2000, {.forwards = false});
    chassis.waitUntilDone();
    scoreAndHold(80);
    pros::delay(250);
    lowerScoring();
    matchLoad(true);
    
    correct_position(LL, &chassis, false);
    chassis.moveToPoint(-70, -46, 5000, {.maxSpeed = 70}, true);
    pros::delay(500);
    // chassis.turnToPoint(-70, -50, 200);
    // chassis.turnToPoint(-70, -46, 200);
    // chassis.turnToPoint(-70, -50, 200);
    // chassis.turnToPoint(-70, -46, 200);
    // chassis.turnToPoint(-70, -50, 200);
    // chassis.turnToPoint(-70, -48, 200);

    chassis.moveToPoint(-24, -46, 3000, {.forwards = false});
    chassis.waitUntilDone();
    scoreAndHold(80);
    pros::delay(250);
    lowerScoring();
    matchLoad(false);

    chassis.moveToPoint(-48, -46, 3000);
    moveTo(-62, -24, 1000, 3000, true, true);
    intake(true);
    chassis.turnToPoint(-62, 5, 1000);
    chassis.moveToPoint(-62, 5, 5000);

}

void sawp4(){
    chassis.setPose(-46, 0, 0);
    liftToggle(true);
    intake(true);
    chassis.moveToPoint(-46, 5, 400);
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
    chassis.setPose(0, 0, 0);
    moveTo(0, 48, 3000, 3000, true, true);
    moveTo(-48, 48, 3000, 3000, true, true);
    moveTo(-48, 0, 3000, 3000, true, true);
    moveTo(0, 0, 3000, 3000, true, true);
}