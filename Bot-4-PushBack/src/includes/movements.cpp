#include "movements.hpp"

/// Initiates scoring sequence with release motion
/// @param maxSpeed Maximum speed for scoring motors
void score(double maxSpeed)
{
    maxScoringSpeed.store(maxSpeed);
    scoringSpeedFunction = [](double position)
    { return maxScoringSpeed.load(); };
    scoringState.store(ScoringState::SCORE_RELEASE);
}

void score(std::function<double(double)> speedFunction)
{
    scoringSpeedFunction = speedFunction;
    scoringState.store(ScoringState::SCORE_RELEASE);
}

/// Initiates scoring sequence while holding position at target angle
/// @param maxSpeed Maximum speed for scoring motors
void scoreAndHold(double maxSpeed)
{
    maxScoringSpeed.store(maxSpeed);
    scoringSpeedFunction = [](double position)
    { return maxScoringSpeed.load(); };
    scoringState.store(ScoringState::SCORE_HOLD);
}

void scoreAndHold(std::function<double(double)> speedFunction)
{
    scoringSpeedFunction = speedFunction;
    scoringState.store(ScoringState::SCORE_HOLD);
}

/// Resets scoring system to down position
void lowerScoring()
{
    scoringState.store(ScoringState::RESET);
}

/// Sets lift to specified state
/// @param value Desired lift state (true = up, false = down)
void liftToggle(bool value)
{
    lift.setState(value);
}

/// Toggles lift between up and down states
void liftToggle()
{
    lift.toggle();
}

/// Toggles intake at maximum speed (127)
void intake()
{
    maxRollerSpeed.store(127);
    intakeToggle.store(!intakeToggle.load());
}

/// Sets intake state and speed
/// @param value Intake enabled state
/// @param maxSpeed Maximum speed for intake roller
void intake(bool value, double maxSpeed)
{
    maxRollerSpeed.store(maxSpeed);
    intakeToggle.store(value);
}

/// Sets reverse (outtake) state and speed
/// @param value Reverse enabled state
/// @param maxSpeed Maximum speed for reverse
void reverse(bool value, double maxSpeed)
{
    reverseToggle.store(value);
    maxRollerSpeed.store(maxSpeed);
}

/// Tank drive control
/// @param left Left side motor power (-127 to 127)
/// @param right Right side motor power (-127 to 127)
void move(double left, double right)
{
    chassis.tank(left, right);
}

/// Sets wing state
/// @param value Desired wing state (true = extended, false = retracted)
void wingToggle(bool value)
{
    wingState.store(value);
    wing.setState(value);
}

/// Toggles match loader
void matchLoad()
{
    matchLoader.toggle();
}

/// Sets match loader state
/// @param value Desired loader state
void matchLoad(bool value)
{
    matchLoader.setState(value);
}

/// Sets intake lift state
/// @param value Desired intake lift state
void intakeLiftToggle(bool value)
{
    intakeLift.setState(value);
}

/// Toggles PTO (Power Take-Off) state with debounce delay
/// @param value Desired PTO state
void togglePTO(bool value)
{
    if (pto.getState() != value)
    {
        pto.setState(value);
        pros::delay(300); // debounce delay
    }
}

/// Main control loop for scoring system and intake
/// Handles state machine for scoring positions and intake/reverse control
void periodic()
{
    double currentAngle = potentiometer.get_value();
    ScoringState currentState = scoringState.load();
    switch (currentState)
    {
    case ScoringState::IDLE:
        break;
    case ScoringState::SCORE_HOLD:
        if (currentAngle > SCORE_ANGLE)
        {
            togglePTO(false);
            systemMotors.move(abs(scoringSpeedFunction(currentAngle)));
        }
        else
        {
            togglePTO(false);
            systemMotors.move(0);
        }
        break;
    case ScoringState::SCORE_RELEASE:
        if (currentAngle > SCORE_ANGLE)
        {
            togglePTO(false);
            systemMotors.move(abs(scoringSpeedFunction(currentAngle)));
        }
        else
        {
            scoringState.store(ScoringState::RESET);
        }
        break;
    case ScoringState::RESET:
        if (currentAngle < DOWN_ANGLE)
        {
            togglePTO(false);
            systemMotors.move(-127);
        }
        else
        {
            togglePTO(false);
            systemMotors.move(0);
            scoringState.store(ScoringState::IDLE);
        }
        break;
    }
    if (currentState == ScoringState::IDLE)
    {
        if (reverseToggle.load())
        {
            togglePTO(true);
            systemMotors.move(maxRollerSpeed.load());
        }
        else if (intakeToggle.load())
        {
            togglePTO(true);
            systemMotors.move(-maxRollerSpeed.load());
        }
        else
        {
            systemMotors.move(0);
        }
    }
    wing.setState(wingState.load());
}