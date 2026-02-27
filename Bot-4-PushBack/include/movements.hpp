#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "globals.hpp"
#include "defines.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

enum class ScoringState
{
    IDLE,
    SCORE_HOLD,
    SCORE_RELEASE,
    RESET
};

const double SCORE_ANGLE = 950; // placeholder value, adjust as needed
const double DOWN_ANGLE = 2245; // placeholder value, adjust as needed

inline std::atomic<double> maxRollerSpeed = std::atomic<double>(127);
inline std::atomic<double> maxScoringSpeed = std::atomic<double>(127);
inline std::function<double(double)> scoringSpeedFunction = std::function<double(double)>(std::function<double(double)>([](double position)
                                                                                                                        { return maxScoringSpeed.load(); }));
inline std::atomic<bool> reverseToggle = std::atomic<bool>(false);
inline std::atomic<bool> wingState = std::atomic<bool>(false);
inline std::atomic<bool> intakeToggle = std::atomic<bool>(false);
inline std::atomic<ScoringState> scoringState = std::atomic<ScoringState>(ScoringState::IDLE);

extern void score(double maxSpeed = 127);

extern void score(std::function<double(double)> speedFunction);

extern void scoreAndHold(double maxSpeed = 127);

extern void scoreAndHold(std::function<double(double)> speedFunction);

extern void lowerScoring();

extern void liftToggle();

extern void liftToggle(bool value);

extern void matchLoad();

extern void matchLoad(bool value);

extern void intakeLiftToggle(bool value);

extern void intake();

extern void intake(bool value, double maxSpeed = 127);

extern void reverse(bool value, double maxSpeed = 127);

extern void move(double left, double right);

extern void togglePTO(bool value);

extern void wingToggle(bool value);

extern void periodic();

#endif