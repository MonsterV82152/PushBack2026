#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void redSAWP()
{

    chassis.setPose(-60.3, -18.5, 180);

    chassis.moveToPoint(-44, -47, 700);
    correct_position(rightLoc, &chassis, true);

    chassis.turnToPoint(-70, -46, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, -46, 1200);
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, -46, 700, {false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);

    chassis.turnToPoint(0, -48, 500);
    chassis.moveToPoint(-36, -48, 700);
    pros::delay(200);
    rollers::setState("scoreTop");
    chassis.turnToHeading(90, 300);

    chassis.waitUntilDone();
    pros::delay(1000);
    rollers::setState("intake");
    pros::delay(300);
    chassis.moveToPoint(-48, -26, 700, {false});
    chassis.turnToPoint(-24, -26, 400);
    rollers::setState("directIntake");
    chassis.moveToPoint(-24, -26, 1000);
    chassis.turnToPoint(-17, -17, 700);
    chassis.moveToPoint(-17, -17, 700, {.maxSpeed = 40});
    pros::delay(200);
    rollers::setState("scoreBottom");

    chassis.waitUntilDone();
    pros::delay(1000);
    rollers::setState("none");
    chassis.moveToPoint(-24, -24, 300, {false});
    chassis.turnToPoint(-24, 24, 400);
    chassis.moveToPoint(-24, 16, 400, {.minSpeed = 90, .earlyExitRange = 4});
    rollers::setState("intake");

    chassis.moveToPoint(-24, 24, 1000, {.maxSpeed = 50});
    chassis.turnToPoint(0, 0, 700);
    chassis.moveToPoint(-12, 7, 700);
    chassis.turnToPoint(0, -5, 300);
    rollers::setState("scoreMiddleAuton");

    chassis.waitUntilDone();
}
void redAuton2()
{
    chassis.setPose(-60.3, 18.5, 0);
    chassis.moveToPoint(-48, 47, 700);
    correct_position(leftLoc, &chassis, true);

    chassis.turnToPoint(-70, 47, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, 47, 700);
    rollers::setState("intake");
    chassis.waitUntilDone();
    for (int i = 0; i < 3; i++)
    {
        chassis.arcade(-20, 0);
        pros::delay(100);
        chassis.arcade(60, 0);
        pros::delay(200);
    }
    chassis.moveToPoint(-48, 47, 700, {false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(rightLoc, &chassis, false);
    chassis.turnToPoint(-48, 24, 400);
    chassis.moveToPoint(-48, 24, 700);
    chassis.turnToPoint(-24, 24, 400);
    rollers::setState("intake");
    chassis.moveToPoint(-16, 24, 1800, {.maxSpeed = 40});
    chassis.turnToPoint(-48, 48, 700, {false});
    chassis.moveToPoint(-48, 48, 1000, {false});
    chassis.turnToPoint(-24, 48, 700);
    chassis.moveToPoint(-37, 48, 1000);
    chassis.turnToHeading(90, 300);
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}
void blueSAWP()
{
    chassis.setPose(60.3, -17.5, 180);
    chassis.moveToPoint(48, -48, 1000);
    chassis.turnToPoint(70, -48, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(70, -48, 1000, {.maxSpeed = 70});
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(500);
    chassis.moveToPoint(48, -48, 1000, {false});
    chassis.turnToPoint(0, -48, 1000);
    chassis.moveToPoint(28, -48, 1000, {.maxSpeed = 60});
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}
void blueAuton2()
{
}

void exampleAuton()
{
}

void skills()
{
    chassis.setPose(-61, -17, 0);
    colourSort::redTeam = false;
    colourSort::on = false;

    { // Clearing Red Park zone
        correct_position(leftLoc, &chassis, true);
        chassis.moveToPoint(-70, 28, 2000, {.maxSpeed = 90});
        rollers::setState("intake");
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        chassis.turnToHeading(30, 700);
        chassis.waitUntilDone();
    }

    { // Matchload Clearing - First
        correct_position(frontLoc, &chassis, false);
        chassis.moveToPoint(-40, 58, 1000);
        matchLoader.setState(true);
        chassis.turnToPoint(-70, 58, 700);
        chassis.moveToPoint(-70, 58, 1000, {.minSpeed = 127});
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-30, 0);
            pros::delay(100);
            // chassis.arcade(40, 0);
            pros::delay(200);
        }
    }

    { // Score Long Goal - First
        chassis.moveToPoint(-48, 58, 700, {false});
        matchLoader.setState(false);
        chassis.turnToHeading(90, 700);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(-36, 48, 700);
        colourSort::on = true;
        pros::delay(400);
        rollers::setState("scoreTop");
        chassis.turnToHeading(90, 400);
        chassis.moveToPoint(-30, 46, 300);

        pros::delay(4000);
        colourSort::on = false;
        pros::delay(2000);
        rollers::setState("intake");
        chassis.moveToPoint(-48, 48, 700, {false});
    }

    { // Traveling to 2nd Match Load
        chassis.turnToPoint(-36, 56, 700);
        chassis.moveToPoint(-36, 56, 700, {.minSpeed = 90, .earlyExitRange = 4});
        chassis.turnToPoint(38, 60, 700);
        chassis.moveToPoint(38, 60, 900, {.minSpeed = 90, .earlyExitRange = 4});
        pros::delay(200);
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(40, 46, 1000);
        chassis.turnToPoint(70, 46, 700);
    }

    { // Clearing 2nd Matchload
        matchLoader.setState(true);
        colourSort::sortML = true;
        colourSort::on = true;
        colourSort::redTeam = true;
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(70, 46, 1000, {.minSpeed = 127});
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-30, 0);
            pros::delay(100);
            // chassis.arcade(40, 0);
            pros::delay(200);
        }
        chassis.moveToPoint(48, 46, 700, {false});
        chassis.turnToPoint(65, 24, 1000);
        matchLoader.setState(false);
    }

    { // Drive through park zone
        chassis.moveToPoint(65, 24, 700);
        chassis.turnToHeading(180, 700);
        correct_position(leftLoc, &chassis, true);
        chassis.moveToPoint(70, -28, 2000, {.maxSpeed = 90});
        rollers::setState("intake");
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        chassis.turnToHeading(210, 700);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, false);
        chassis.moveToPoint(40, -58, 1000);
    }

    { // Clearing 3rd Matchload
        matchLoader.setState(true);
        chassis.turnToPoint(70, -58, 700);
        chassis.moveToPoint(70, -58, 1000, {.minSpeed = 127});
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-30, 0);
            pros::delay(100);
            // chassis.arcade(40, 0);
            pros::delay(200);
        }
        chassis.moveToPoint(48, -58, 700, {false});
        matchLoader.setState(false);
    }

    { // Score long goal - 2
        chassis.turnToHeading(270, 700);
        chassis.waitUntilDone(); 
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(36, -48, 700);
        colourSort::on = true;
        pros::delay(400);
        rollers::setState("scoreTop");
        chassis.turnToHeading(270, 400);
        chassis.moveToPoint(30, -48, 300);

        pros::delay(4000);
        colourSort::on = false;
        pros::delay(2000);
        rollers::setState("intake");
        chassis.moveToPoint(48, -48, 700, {false});
    }
}
#endif