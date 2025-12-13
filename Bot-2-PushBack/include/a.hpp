int leftLoc = 0;
int chassis = 0;
int rightLoc = 0;


/**
 * @brief Autonomous routine for left side 2 goals
 */
void Left2Goals()
{
    chassis.setPose(-60.3, 18.5, 0);   // Starting position
    chassis.moveToPoint(-48, 45, 700); // Move to matchloader
    // Correct position with left distance sensor
    correct_position(leftLoc, &chassis, true);
    chassis.turnToPoint(-70, 46, 700); // Turn towards matchloader
    matchLoader.setState(true); // Activate matchloader piston
    chassis.moveToPoint(-70, 46, 700); // Move into matchloader
    rollers::setState("intake"); // Start intake
    chassis.waitUntilDone();
    for (int i = 0; i < 3; i++) // Shake to ensure blocks are secured
    {
        chassis.arcade(-20, 0);
        pros::delay(100);
        chassis.arcade(60, 0);
        pros::delay(200);
    }
    chassis.moveToPoint(-48, 47, 700, {false}); // Move out of matchloader
    matchLoader.setState(false);                // Deactivate matchloader piston
    chassis.waitUntilDone();
    correct_position(rightLoc, &chassis, false); // Correct position with right distance sensor
    // Drive to intake the group of 3 blocks
    chassis.turnToPoint(-48, 24, 400);
    chassis.moveToPoint(-48, 24, 700);
    chassis.turnToPoint(-24, 24, 400);
    rollers::setState("intake"); // Start intake
    chassis.moveToPoint(-16, 24, 1400, {.maxSpeed = 40}); // Intake blocks
    chassis.turnToPoint(-24, 22, 700, {false}); // Move to scoring position
    chassis.moveToPoint(-24, 22, 700, {false});
    chassis.turnToPoint(-7, 5, 700); // Turn towards scoring zone
    chassis.moveToPoint(-7, 5, 400, {.maxSpeed = 40}); // Move into scoring zone
    chassis.waitUntilDone();
    rollers::setState("scoreMiddleAuton"); // Score middle block
    pros::delay(1000); // Wait for scoring to complete
    rollers::setState("intake"); // Start intake for next blocks
    chassis.turnToPoint(-48, 48, 700, {false}); // Back to high goal position
    chassis.moveToPoint(-48, 48, 1000, {false}); // Move to high goal position
    chassis.turnToPoint(-24, 47.5, 700); // Turn towards high goal
    chassis.moveToPoint(-38, 47.5, 1000); // Move into high goal position
    correct_position(leftLoc, &chassis, false); // Correct position with left distance sensor
    chassis.turnToHeading(90, 700); // Face high goal
    chassis.waitUntilDone();
    rollers::setState("scoreTop"); // Score high goals
}
