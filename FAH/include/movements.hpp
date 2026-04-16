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
#include <atomic>

void moveDT(int leftY, int rightX);

void requestScore(int speed, int timeout);  // Request score (sets flags)

void startScoreThread();  // Start background score thread (call once in initialize)

void resetLeverDown();  // Reset lever to down position

void scoreLowGoal(int speed);  // Move lever for low goal scoring

void intake(int speed);

#endif