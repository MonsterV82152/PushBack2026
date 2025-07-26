#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void SAWP()
{

    chassis.setPose(-60.3, -18.5, 180);

    chassis.moveToPoint(-44, -47, 700);
    correct_position(rightLoc, &chassis, true);

    chassis.turnToPoint(-70, -48, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, -48, 1200);
    rollers::setState("intake");
    chassis.waitUntilDone();
    // pros::delay(200);
    chassis.moveToPoint(-48, -48, 700, {false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);

    chassis.turnToHeading(90, 500);
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
    chassis.turnToPoint(-7, -7, 700);
    chassis.moveToPoint(-7, -7, 700, {.maxSpeed = 40});
    // pros::delay(200);
    chassis.waitUntilDone();
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
    chassis.moveToPoint(-12, 9, 700);
    chassis.turnToPoint(0, -5, 300);
    rollers::setState("scoreMiddleAuton");

    chassis.waitUntilDone();
}
void Left()
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

void Left2Goals()
{
    chassis.setPose(-60.3, 18.5, 0);
    chassis.moveToPoint(-48, 45, 700);
    correct_position(leftLoc, &chassis, true);

    chassis.turnToPoint(-70, 46, 700);
    // correct_position(frontLoc, &chassis, false);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, 46, 700);
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
    chassis.turnToPoint(-24, 22, 700, {false});
    chassis.moveToPoint(-24, 22, 1000, {false});
    chassis.turnToPoint(-7, 5, 700);
    chassis.moveToPoint(-7, 5, 700, {.maxSpeed = 40});
    chassis.waitUntilDone();
    rollers::setState("scoreMiddleAuton");
    pros::delay(1000);
    rollers::setState("intake");
    chassis.turnToPoint(-48, 48, 700, {false});
    chassis.moveToPoint(-48, 48, 1000, {false});
    chassis.turnToPoint(-24, 48, 700);
    chassis.moveToPoint(-37, 48, 1000);
    chassis.turnToHeading(90, 300);
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}

void Right()
{
    chassis.setPose(-60.3, -18.5, 180);
    chassis.moveToPoint(-48, -47, 700);
    correct_position(rightLoc, &chassis, true);

    chassis.turnToPoint(-70, -47, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, -47, 700);
    rollers::setState("intake");
    chassis.waitUntilDone();
    for (int i = 0; i < 3; i++)
    {
        chassis.arcade(-20, 0);
        pros::delay(100);
        chassis.arcade(60, 0);
        pros::delay(200);
    }
    chassis.moveToPoint(-48, -47, 700, {false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);
    chassis.turnToPoint(-48, -24, 400);
    chassis.moveToPoint(-48, -24, 700);
    chassis.turnToPoint(-24, -24, 400);
    rollers::setState("intake");
    chassis.moveToPoint(-16, -24, 1800, {.maxSpeed = 40});
    chassis.turnToPoint(-48, -48, 700, {false});
    chassis.moveToPoint(-48, -48, 1000, {false});
    chassis.turnToPoint(-24, -48, 700);
    chassis.moveToPoint(-37, -48, 1000);
    chassis.turnToHeading(90, 300);
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}
void skills()
{
    chassis.setPose(-61, -17, 0);
    colourSort::redTeam = false;
    colourSort::on = false;

    { // Clearing Red Park zone
        correct_position(leftLoc, &chassis, true);
      matchLoader.setState(true);
      pros::delay(200);
        chassis.moveToPoint(-60, 28, 2300, {.maxSpeed = 90});
        rollers::setState("intake");
        pros::delay(200);
        matchLoader.setState(false);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        chassis.turnToHeading(30, 700);
        chassis.waitUntilDone();
    }

    { // Matchload Clearing - First

        correct_position(frontLoc, &chassis, false);
        chassis.moveToPoint(-44, 48, 1200);
        matchLoader.setState(true);
        chassis.turnToPoint(-70, 48, 700);
        chassis.moveToPoint(-70, 48, 1000);
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-20, 0);
            pros::delay(100);
            chassis.arcade(40, 0);
            pros::delay(200);
        }
        correct_position(rightLoc, &chassis, false);
    }

    { // Score Long Goal - First
        chassis.moveToPoint(-48, 48, 900, {false});
        matchLoader.setState(false);
        chassis.turnToHeading(90, 700);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(-40, 48, 700);
        colourSort::on = true;
        pros::delay(400);
        rollers::setState("scoreTop");
        chassis.turnToHeading(90, 300);
        chassis.moveToPoint(-30, 48, 300, {.maxSpeed = 40});

        pros::delay(4000);
        colourSort::on = false;
        pros::delay(2000);
        rollers::setState("intake");
        chassis.moveToPoint(-48, 48, 800, {false});
    }

    { // Traveling to 2nd Match Load
        chassis.turnToPoint(-36, 56, 400);
        chassis.moveToPoint(-36, 56, 700, {.minSpeed = 90, .earlyExitRange = 4});
        chassis.turnToPoint(40, 62, 400, {.minSpeed = 90, .earlyExitRange = 15});
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(38, 62, 1500, {.minSpeed = 90, .earlyExitRange = 2});

        chassis.moveToPoint(44, 46, 900);
        matchLoader.setState(true);
        chassis.turnToPoint(70, 48, 500);
    }

    { // Clearing 2nd Matchload
        colourSort::sortML = true;
        colourSort::on = true;
        colourSort::redTeam = true;
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(70, 48, 800);
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-20, 0);
            pros::delay(100);
            chassis.arcade(40, 0);
            pros::delay(200);
        }
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(44, 48, 1200, {false});
        chassis.waitUntilDone();
        pros::delay(500);
        matchLoader.setState(false);
        colourSort::sortML = false;
        chassis.turnToPoint(65, 24, 600);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, true);
    }

    { // Drive through park zone
        chassis.moveToPoint(58, 28, 1200, {.minSpeed = 90, .earlyExitRange = 2});
        chassis.swingToPoint(70, -28, lemlib::DriveSide::RIGHT, 700, {.minSpeed = 90, .earlyExitRange = 4});
        correct_position(leftLoc, &chassis, true);
        chassis.moveToPoint(60, -32, 2400, {.maxSpeed = 80});
        rollers::setState("intake");
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        chassis.turnToHeading(210, 700);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, false);
        chassis.moveToPoint(44, -48, 700);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, false);
    }

    { // Clearing 3rd Matchload
        matchLoader.setState(true);
        chassis.turnToPoint(70, -48, 700);
        chassis.moveToPoint(70, -48, 800);
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-10, 0);
            pros::delay(100);
            chassis.arcade(40, 0);
            pros::delay(200);
        }
        chassis.moveToPoint(48, -48, 700, {false});
        matchLoader.setState(false);
        correct_position(rightLoc, &chassis, false);
    }

    { // Score long goal - 2
        chassis.turnToHeading(270, 700);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(48, -48, 700);
        colourSort::on = true;
        pros::delay(400);
        rollers::setState("scoreTop");
        chassis.turnToHeading(270, 400);
        chassis.moveToPoint(30, -48, 700, {.maxSpeed = 50});

        pros::delay(4000);
        colourSort::on = false;
        pros::delay(2000);
        rollers::setState("intake");
        chassis.moveToPoint(48, -48, 700, {false});
    }

    {
        colourSort::on = true;
        colourSort::redTeam = true;
        colourSort::sortML = true;
        chassis.turnToPoint(24, -24, 700);
        chassis.moveToPoint(24, -24, 700);
        chassis.turnToPoint(-24, -24, 700);
        chassis.moveToPoint(-24, -24, 1000);
        pros::delay(300);
        correct_position(leftLoc, &chassis, false);
        chassis.turnToPoint(-44, -48, 700);
        chassis.moveToPoint(-44, -48, 1000);
        chassis.turnToPoint(-70, -48, 700);
        matchLoader.setState(true);
        chassis.moveToPoint(-70, -48, 1000);
        chassis.waitUntilDone();
        for (int i = 0; i < 6; i++)
        {
            chassis.arcade(-20, 0);
            pros::delay(100);
            chassis.arcade(40, 0);
            pros::delay(200);
        }
        correct_position(leftLoc, &chassis, false);
        chassis.moveToPoint(-48, -48, 1200, {false});
        chassis.waitUntilDone();
        pros::delay(1000);
        matchLoader.setState(false);
        colourSort::sortML = false;
        matchLoader.setState(false);
        chassis.turnToPoint(-24, 24, 700);
        chassis.moveToPoint(-24, 24, 1200);
        correct_position(leftLoc, &chassis, true);
        chassis.turnToPoint(0, 0, 700);
        chassis.moveToPoint(-12, 12, 1000, {.maxSpeed = 40});
        pros::delay(600);
        rollers::setState("scoreMiddle");
        pros::delay(1000);
        chassis.moveToPoint(-24, 24, 700, {false});
        chassis.turnToPoint(-32, -9, 700);
        chassis.moveToPoint(-32, -9, 1000);
        chassis.turnToPoint(-70, -9, 700);
        chassis.moveToPoint(-70, -9, 1000, {.minSpeed = 80});
    }
}

void example()
{
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 48, 1500);
    chassis.turnToPoint(-48, 48, 1000);
    chassis.moveToPoint(-48, 48, 1500);
    chassis.turnToPoint(-48, 0, 1000);
    chassis.moveToPoint(-48, 0, 1500);
    chassis.turnToPoint(0, 0, 1000);
    chassis.moveToPoint(0, 0, 1500);
}

#endif