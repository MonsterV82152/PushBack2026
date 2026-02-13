#include "movements.hpp"

void score(double maxSpeed)
{
    maxScoringSpeed.store(maxSpeed);
    scoringState.store(ScoringState::SCORE_RELEASE);
}

void scoreAndHold(double maxSpeed)
{
    maxScoringSpeed.store(maxSpeed);
    scoringState.store(ScoringState::SCORE_HOLD);
}

void lowerScoring()
{
    scoringState.store(ScoringState::RESET);
}

void liftToggle(bool value)
{
    lift.setState(value);
}

void liftToggle()
{
    lift.toggle();
}

void intake()
{
    maxRollerSpeed.store(127);
    intakeToggle.store(!intakeToggle.load());
}

void intake(bool value, double maxSpeed)
{
    maxRollerSpeed.store(maxSpeed);
    intakeToggle.store(value);
}

void reverse(bool value, double maxSpeed)
{
    reverseToggle.store(value);
    maxRollerSpeed.store(maxSpeed);
}

void move(double left, double right)
{
    chassis.tank(left, right);
}

void wingToggle(bool value)
{
    wingState.store(value);
    wing.setState(value);
}

void matchLoad()
{
    matchLoader.toggle();
}

void matchLoad(bool value)
{
    matchLoader.setState(value);
}

void intakeLiftToggle(bool value)
{
    intakeLift.setState(value);
}

void togglePTO(bool value)
{
    if (pto.getState() != value)
    {
        pto.setState(value);
        pros::delay(500); // debounce delay
    }
}

void periodic()
{
    double currentAngle = potentiometer.get_value();
    ScoringState currentState = scoringState.load();
    switch (currentState)
    {
    case ScoringState::IDLE:
        break;
    case ScoringState::SCORE_HOLD:
        if (currentAngle > SCORE_ANGLE + 30)
        {
            togglePTO(false);
            systemMotors.move(abs(maxScoringSpeed.load()));
        }
        else if (currentAngle < SCORE_ANGLE)
        {
            togglePTO(false);
            systemMotors.move(20);
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
            systemMotors.move(abs(maxScoringSpeed.load()));
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
        } else {
            systemMotors.move(0);
        }
    }
    wing.setState(wingState.load());
}