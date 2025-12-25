#include "autonomous_paths.hpp"

void left()
{
    robot.setPose(-46, 16, 90);
    robot.intake();
    robot.moveToPoint(-24,24,1000);
    robot.moveToPoint(-9,48,1000);
    robot.moveToPoint(-24,30,1000,{false});
    robot.moveToPoint(-48,48,1000,{false});
    robot.turnToPoint(-70,48,700);
    robot.matchLoad(true);
    robot.moveToPoint(-70,48,1000);
    robot.moveToPoint(-48,48,700,{false});
    robot.turnToPoint(-28,48,700);
    robot.moveToPoint(-28,48,700);
    robot.waitUntilDone();
    robot.score();
}

void left2()
{
    robot.setPose(-46, 16, 90);
    robot.intake();
    robot.moveToPoint(-24,24,1000);
    robot.moveToPoint(-9,48,1000);
    robot.moveToPoint(-24,24,1000,{false});
    robot.turnToPoint(-12,12,700);
    robot.moveToPoint(-12,12,700);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(1000);
    robot.moveToPoint(-48,48,1000,{false});
    robot.turnToPoint(-70,48,700);
    robot.matchLoad(true);
    robot.moveToPoint(-70,48,1000);
    robot.moveToPoint(-48,48,700,{false});
    robot.turnToPoint(-28,48,700);
    robot.moveToPoint(-28,48,700);
    robot.waitUntilDone();
    robot.score();
}

void printPose()
{
}
void soloAWP()
{
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
    robot.moveToPoint(-24,-24,1000);
    robot.moveToPoint(-9,-48,1000);
    robot.moveToPoint(-24,-30,1000,{false});
    robot.moveToPoint(-48,-48,1000,{false});
    robot.turnToPoint(-70,-48,700);
    robot.matchLoad(true);
    robot.moveToPoint(-70,-48,1000);
    robot.moveToPoint(-48,-48,700,{false});
    robot.turnToPoint(-28,-48,700);
    robot.moveToPoint(-28,-48,700);
    robot.waitUntilDone();
    robot.score();
}

void right2()
{
    robot.setPose(-46, -16, 90);
    robot.intake();
    robot.moveToPoint(-24,-24,1000);
    robot.moveToPoint(-9,-48,1000);
    robot.moveToPoint(-24,-24,1000,{false});
    robot.turnToPoint(-12,-12,700);
    robot.moveToPoint(-12,-12,700);
    robot.waitUntilDone();
    robot.score(1000);
    pros::delay(1000);
    robot.moveToPoint(-48,-48,1000,{false});
    robot.turnToPoint(-70,-48,700);
    robot.matchLoad(true);
    robot.moveToPoint(-70,-48,1000);
    robot.moveToPoint(-48,-48,700,{false});
    robot.turnToPoint(-28,-48,700);
    robot.moveToPoint(-28,-48,700);
    robot.waitUntilDone();
    robot.score();
}

void skills()
{
    robot.setPose(-36,0,270);
    robot.intake();
    robot.moveToPoint(-70,0,2000);
    robot.moveToPoint(-36,0,2000,{false});
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