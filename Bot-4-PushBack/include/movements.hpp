#include "includes.hpp"

enum class ScoringState
{
    IDLE,
    SCORE_HOLD,
    SCORE_RELEASE,
    RESET
};

const double SCORE_ANGLE = 100; // placeholder value, adjust as needed
const double DOWN_ANGLE = 50; // placeholder value, adjust as needed

std::atomic<double> maxScoringSpeed = std::atomic<double>(127);

std::atomic<bool> reverseToggle = std::atomic<bool>(false);
std::atomic<bool> wingState = std::atomic<bool>(false);
std::atomic<bool> intakeToggle = std::atomic<bool>(false);
std::atomic<ScoringState> scoringState = std::atomic<ScoringState>(ScoringState::IDLE);

void score(double maxSpeed = 127);

void scoreAndHold(double maxSpeed = 127);

void lowerScoring();

void liftToggle();

void liftToggle(bool value);

void matchLoad();

void matchLoad(bool value);

void intakeLiftToggle(bool value);

void intake();

void intake(bool value);

void reverse(bool value);

void move(double left, double right);

void wingToggle(bool value);

void periodic();