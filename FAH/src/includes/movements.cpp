#include "movements.hpp"

void moveDT(int leftY, int rightX)
{
    // Turn priority: reduce forward when turning
    int forward = leftY;
    int turn = rightX;

    // Reduce forward movement based on turn magnitude
    if (turn != 0)
    {
        forward = forward * (0.9 - abs(turn) / 127.0);
    }

    int left = forward + turn;
    int right = forward - turn;

    leftDT.move(left);
    rightDT.move(right);
}

// Background thread for scoring - runs continuously
void scoreThreadTask(void *param)
{
    while (true)
    {
        // Wait for score request
        if (scoreRequested && !scoring)
        {
            scoreRequested = false;
            int speed = scoreSpeed;
            int timeout = scoreTimeout;

            scoring = true;

            if (pot.get_value() < LEVER_DOWN_ANGLE)
            {
                hood.setState(true);
                if (speed == 127)
                    lever.setState(true);
                pto.setState(false);

                // Up phase with timeout
                uint32_t startTime = pros::millis();
                while (pot.get_value() < LEVER_UP_ANGLE)
                {
                    if (pros::millis() - startTime > timeout)
                    {
                        break; // Timeout - exit
                    }
                    motors.move(speed);
                    pros::delay(10);
                }

                // Down phase with timeout
                startTime = pros::millis();
                while (pot.get_value() > LEVER_DOWN_ANGLE)
                {
                    if (pros::millis() - startTime > 3000)
                    {
                        break; // Timeout - exit
                    }
                    lever.setState(false);
                    motors.move(-127);
                    pros::delay(10);
                }
            }
            pto.setState(true);
            motors.move(0);

            scoring = false;

            if (intakeOn)
            {
                intake(127);
            }
        }
        pros::delay(20);
    }
}

// Request a score action (just sets flags, thread handles it)
void requestScore(int speed, int timeout)
{
    if (!scoring)
    {
        scoreSpeed = speed;
        scoreTimeout = timeout;
        scoreRequested = true;
    }
}

// Start the background score thread (call this once in initialize)
void startScoreThread()
{
    static bool threadStarted = false;
    if (!threadStarted)
    {
        threadStarted = true;
        pros::Task scoreThread(scoreThreadTask, nullptr);
    }
}

void resetLeverDown()
{
    /**
     * Reset lever to down position
     * Moves lever down until potentiometer reads <= LEVER_DOWN_ANGLE
     * with a 3 second timeout to prevent infinite loops
     * Prevents multiple simultaneous calls with guard flag
     */
    if (leverResetting)
        return; // Prevent multiple calls

    leverResetting = true;

    uint32_t startTime = pros::millis();
    while (pot.get_value() > LEVER_DOWN_ANGLE)
    {
        if (pros::millis() - startTime > 3000)
        {
            break; // Timeout - exit
        }
        lever.setState(false);
        motors.move(-127);
        pros::delay(10);
    }
    motors.move(0);

    leverResetting = false; // Allow future calls
}

void raiseLever()
{
    /**
     * Raise lever to up position
     * Moves lever up until potentiometer reads >= LEVER_UP_ANGLE
     * with a 3 second timeout to prevent infinite loops
     * Prevents multiple simultaneous calls with guard flag
     */
    if (leverRaising)
        return; // Prevent multiple calls

    leverRaising = true;

    uint32_t startTime = pros::millis();
    while (pot.get_value() < LEVER_UP_ANGLE)
    {
        if (pros::millis() - startTime > 3000)
        {
            break; // Timeout - exit
        }
        lever.setState(true);
        motors.move(127);
        pros::delay(10);
    }
    motors.move(0);

    leverRaising = false; // Allow future calls
}

void intake(int speed)
{
    if (scoring)
        return;
    pto.setState(true);
    if (speed < 0)
    {
        intakeLift.setState(true);
    }
    else
        intakeLift.setState(false);

    motors.move(speed);
    hood.setState(false);
}

void scoreLowGoal(int speed)
{
    if (pot.get_value() < LEVER_DOWN_ANGLE)
    {
        pto.setState(false);
        motors.move(speed);
    }
    else
    {
        pto.setState(true);
        motors.move(speed);
    }
}