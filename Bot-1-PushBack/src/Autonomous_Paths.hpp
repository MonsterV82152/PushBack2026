#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "movements.cpp"

void SAWP()
{

    chassis.setPose(-60.3, -18.5, 180);
    correct_position(rightLoc, &chassis, true);
    chassis.moveToPoint(-44, -47, 700);
    

    chassis.turnToPoint(-70, -48, 700);
    matchLoader.setState(true);
    chassis.moveToPoint(-70, -48, 1200);
    rollers::setState("intake");
    chassis.waitUntilDone();
    pros::delay(200);
    chassis.moveToPoint(-48, -48.5, 700, {false});
    matchLoader.setState(false);
    chassis.waitUntilDone();
    correct_position(leftLoc, &chassis, false);

    chassis.turnToHeading(90, 700);
    chassis.moveToPoint(-40, -48.5, 700);
    pros::delay(200);
    rollers::setState("scoreTop");
    chassis.turnToHeading(90, 400);

    chassis.waitUntilDone();
    pros::delay(700);
    rollers::setState("intake");
    pros::delay(300);
    chassis.moveToPoint(-48, -26, 500, {false});
    chassis.waitUntilDone();
    // correct_position(rightLoc, &chassis, true);
    chassis.turnToPoint(-24, -26, 400);
    rollers::setState("directIntake");
    chassis.moveToPoint(-24, -26, 700);
    chassis.turnToPoint(-9, -7, 500);
    chassis.moveToPoint(-9, -7, 600, {.maxSpeed = 40});
    // chassis.waitUntilDone();
    rollers::setState("scoreBottom");

    chassis.waitUntilDone();
    pros::delay(1000);
    rollers::setState("none");
    chassis.moveToPoint(-24, -24, 300, {false});
    chassis.turnToPoint(-24, 24, 400);` 
    chassis.moveToPoint(-24, 16, 400, {.minSpeed = 90, .earlyExitRange = 4});
    rollers::setState("intake");

    chassis.moveToPoint(-26, 24, 1000, {.maxSpeed = 50});
    chassis.turnToPoint(0, 0, 700);
    chassis.moveToPoint(-15, 10.5, 900);
    // chassis.turnToPoint(0, -5, 300);
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
    chassis.moveToPoint(-38, 48, 1000);
    chassis.turnToHeading(90, 600);
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
    chassis.moveToPoint(-16, 24, 1400, {.maxSpeed = 40});
    chassis.turnToPoint(-24, 22, 700, {false});
    chassis.moveToPoint(-24, 22, 700, {false});
    chassis.turnToPoint(-7, 5, 700);
    chassis.moveToPoint(-7, 5, 400, {.maxSpeed = 40});
    chassis.waitUntilDone();
    rollers::setState("scoreMiddleAuton");
    pros::delay(1000);
    rollers::setState("intake");
    chassis.turnToPoint(-48, 48, 700, {false});
    chassis.moveToPoint(-48, 48, 1000, {false});
    chassis.turnToPoint(-24, 47.5, 700);
    chassis.moveToPoint(-38, 47.5, 1000);
    correct_position(leftLoc, &chassis, false);
    chassis.turnToHeading(90, 700);
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
    chassis.moveToPoint(-39, -48, 800);
    chassis.turnToHeading(90, 600);
    chassis.waitUntilDone();
    rollers::setState("scoreTop");
}

void Right2Goals()
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
    chassis.moveToPoint(-37.5, -48, 700);
    pros::delay(200);
    rollers::setState("scoreTop");
    chassis.turnToHeading(90, 1100);

    chassis.waitUntilDone();
    rollers::setState("intake");
    pros::delay(300);
    chassis.moveToPoint(-48, -26, 700, {false});
    chassis.waitUntilDone();
    // correct_position(rightLoc, &chassis, true);
    chassis.turnToPoint(-24, -24, 400);
    rollers::setState("directIntake");
    chassis.moveToPoint(-24, -24, 1000);
    chassis.turnToPoint(-7, -7, 700);
    chassis.moveToPoint(-7, -7, 600, {.maxSpeed = 40});
    // chassis.waitUntilDone();
    rollers::setState("scoreBottom");
}
void skills()
{
    chassis.setPose(-61, -17, 0);
    colourSort::redTeam = false;
    colourSort::on = false;

    { // Clearing Red Park zone
        correct_position(leftLoc, &chassis, true);
        master.rumble("*");
        matchLoader.setState(true);
        pros::delay(200);
        chassis.moveToPoint(-65, 36, 2800, {.maxSpeed = 90});
        rollers::setState("intake");
        pros::delay(200);
        matchLoader.setState(false);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        master.rumble("*");
        chassis.turnToHeading(30, 600);
        chassis.waitUntilDone();
    }

    { // Matchload Clearing - First

        correct_position(frontLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(-44, 48, 1000);
        matchLoader.setState(true);
    chassis.turnToPoint(-70, 48, 700);
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }
        chassis.moveToPoint(-70, 48, 900);
        chassis.waitUntilDone();
        for (int i = 0; i < 8; i++)
        {
            chassis.arcade(0, 0);
            pros::delay(100);
            chassis.arcade(127, 0);
            pros::delay(200);
        }
        correct_position(rightLoc, &chassis, false);
        master.rumble("*");
    }

    { // Score Long Goal - First
        chassis.moveToPoint(-48, 48, 900, {false});
        matchLoader.setState(false);
        chassis.turnToHeading(90, 700);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(-40, 48.5, 700);
        colourSort::on = true;
        pros::delay(400);
        rollers::setState("scoreTop");
        chassis.turnToHeading(90, 300);
        chassis.moveToPoint(-30, 48.5, 350, {.maxSpeed = 40});
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }

        pros::delay(4000);
        colourSort::on = false;
        rollers::setState("scoreTopAuton");
        pros::delay(3000);
        rollers::setState("intake");
        chassis.moveToPoint(-48, 48.5, 800, {false});
    }

    { // Traveling to 2nd Match Load
        chassis.turnToPoint(-36, 58, 400);
        chassis.moveToPoint(-36, 58, 700);
        chassis.swingToPoint(38, 65, lemlib::DriveSide::RIGHT, 400);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(36, 65, 1500);
        chassis.waitUntilDone();
        // correct_position(frontLoc, &chassis, true);

        chassis.moveToPoint(44, 48, 700);
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        matchLoader.setState(true);
        chassis.turnToPoint(70, 50, 500);
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }
    }

    { // Clearing 2nd Matchload
        colourSort::on = true;
        colourSort::redTeam = true;
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(70, 50, 800);
        chassis.waitUntilDone();
        for (int i = 0; i < 8; i++)
        {
            chassis.arcade(0, 0);
            pros::delay(100);
            chassis.arcade(127, 0);
            pros::delay(200);
        }
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(48, 48, 1200, {false});
        chassis.waitUntilDone();
        matchLoader.setState(false);
        chassis.turnToPoint(65, 24, 600);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, true);
        master.rumble("*");
    }

    { // Drive through park zone
        chassis.moveToPoint(61, 24, 700);
        chassis.swingToPoint(75, -28, lemlib::DriveSide::RIGHT, 700);
        correct_position(leftLoc, &chassis, true);
        master.rumble("*");

        chassis.moveToPoint(75, -38, 3000, {.maxSpeed = 70});

        rollers::setState("intake");
        pros::delay(200);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, true);
        master.rumble("*");
        chassis.turnToHeading(210, 700);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(44, -48, 1000);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, false);
        master.rumble("*");
    }

    { // Clearing 3rd Matchload
        matchLoader.setState(true);
        chassis.turnToPoint(70, -48, 700);
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }
        chassis.moveToPoint(70, -48, 800);
        chassis.waitUntilDone();
        for (int i = 0; i < 8; i++)
        {
            chassis.arcade(0, 0);
            pros::delay(100);
            chassis.arcade(127, 0);
            pros::delay(200);
        }
        chassis.moveToPoint(48, -48, 700, {false});
        matchLoader.setState(false);
        correct_position(rightLoc, &chassis, false);
        master.rumble("*");
    }

    { // Score long goal - 2
        chassis.turnToHeading(270, 700);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(42, -48, 700);
        colourSort::on = true;
        
        chassis.turnToHeading(270, 700);
        chassis.waitUntilDone();
        rollers::setState("scoreTop");
        chassis.moveToPoint(30, -48.5, 700, {.maxSpeed = 40});
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }

        pros::delay(4000);
        colourSort::on = false;
        rollers::setState("scoreTopAuton");
        pros::delay(3000);
        rollers::setState("intake");
        chassis.moveToPoint(48, -48.5, 700, {false});
    }

    { // Traveling to 4th Match Load
        chassis.turnToPoint(36, -58, 400);
        chassis.moveToPoint(36, -58, 700);
        chassis.swingToPoint(-38, -62, lemlib::DriveSide::RIGHT, 400);
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(-37, -62, 1500);
        chassis.waitUntilDone();
        // correct_position(frontLoc, &chassis, true);

        chassis.moveToPoint(-40, -50, 700);
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        matchLoader.setState(true);
        chassis.turnToPoint(-70, -50, 500);
    // chassis.waitUntilDone();
    // while (!master.get_digital_new_press(buttons::A)) {
    //     pros::delay(20);
    // }
    }

    { // Clearing 2nd Matchload
        colourSort::on = true;
        colourSort::redTeam = true;
        chassis.waitUntilDone();
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(-70, -50, 800);
        chassis.waitUntilDone();
        for (int i = 0; i < 8; i++)
        {
            chassis.arcade(0, 0);
            pros::delay(100);
            chassis.arcade(127, 0);
            pros::delay(200);
        }
        correct_position(leftLoc, &chassis, false);
        master.rumble("*");
        chassis.moveToPoint(-44, -48, 1200, {false});
        chassis.waitUntilDone();
        matchLoader.setState(false);
        chassis.turnToPoint(-65, -24, 600);
        chassis.waitUntilDone();
        correct_position(frontLoc, &chassis, true);
        master.rumble("*");
    }
    { // Drive through park zone
        chassis.moveToPoint(-60, -24, 700);
        chassis.swingToPoint(-70, 12, lemlib::DriveSide::RIGHT, 700);
        correct_position(leftLoc, &chassis, true);
        master.rumble("*");

        chassis.moveToPoint(-70, 12, 1000, {.maxSpeed = 90});
        // chassis.waitUntilDone();
        // rollers::setState("scorePark");
        // pros::delay(500);
        // while (bottomDS.get_distance() >= 60)
        // {
        //     pros::delay(10);
        // }
        // rollers::setState("scorePark2");
        // while (bottomDS.get_distance() <= 60)
        // {
        //     pros::delay(10);
        // }
        // // pros::delay(100);
        // // rollers::setState("intakeC");
        // // pros::delay(50);
        // rollers::setState("none");
        // park.setState(true);
    }

    // {
    //     colourSort::on = true;
    //     colourSort::redTeam = true;
    //     chassis.turnToPoint(24, -24, 700);
    //     chassis.moveToPoint(24, -24, 700);
    //     chassis.turnToPoint(-24, -24, 700);
    //     chassis.moveToPoint(-24, -24, 1000);
    //     pros::delay(300);
    //     correct_position(leftLoc, &chassis, false);
    //     chassis.waitUntilDone();
    //     correct_position(frontLoc, &chassis, true);
    //     chassis.turnToPoint(-40, -49, 700);
    //     chassis.moveToPoint(-40, -49, 1000);
    //     chassis.turnToPoint(-70, -49, 700);
    //     matchLoader.setState(true);
    //     chassis.moveToPoint(-70, -49, 1000);
    //     chassis.waitUntilDone();
    //     for (int i = 0; i < 6; i++)
    //     {
    //         chassis.arcade(0, 0);
    //         pros::delay(100);
    //         chassis.arcade(127, 0);
    //         pros::delay(200);
    //     }
    //     correct_position(leftLoc, &chassis, false);
    //     master.rumble("*");
    //     chassis.moveToPoint(-48, -48, 1200, {false});
    //     chassis.waitUntilDone();
    //     matchLoader.setState(false);
    //     matchLoader.setState(false);
    //     chassis.turnToPoint(-24, 24, 700);
    //     chassis.moveToPoint(-24, 24, 1800);
    //     correct_position(leftLoc, &chassis, true);
    //     master.rumble("*");
    //     chassis.turnToHeading(135, 700);
    //     chassis.moveToPoint(-12, 12, 1000, {.maxSpeed = 40});
    //     pros::delay(600);
    //     rollers::setState("scoreMiddle");
    //     pros::delay(3000);
    //     chassis.moveToPoint(-24, 24, 700, {false});
    //     chassis.turnToPoint(-32, -9, 700);
    //     chassis.moveToPoint(-32, -9, 1000);
    //     chassis.turnToPoint(-70, -9, 700);
    //     chassis.moveToPoint(-70, -9, 1000, {.minSpeed = 80});
    // }
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