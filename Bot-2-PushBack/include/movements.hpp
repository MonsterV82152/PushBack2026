#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "main.h"
#include "piston.hpp"

#define ON 1
#define OFF 0
#define LEAVE 128

struct rollerState
{
    short back = 0;
    short middle = 0;
    short front = 0;
    short flipPiston = 0;
    short blockerPiston = 1;
};

struct tempState
{
    rollerState state;
    short importance;
};

// Example operator== for rollerState
bool operator==(const rollerState& lhs, const rollerState& rhs);
inline const rollerState INTAKE = {127, 127, 127, OFF, ON};
inline const rollerState INTAKE2 = {127, 127, 0, OFF, ON};
inline const rollerState INTAKE3 = {0, 127, 0, OFF, ON};
inline const rollerState L1 = {0, -127, -127, OFF, LEAVE};
inline const rollerState L2 = {127, 127, -127, OFF, LEAVE};
inline const rollerState L3 = {127, 127, 127, OFF, OFF};
inline const rollerState BACKL2 = {-127, 127, 0, OFF, ON};
inline const rollerState BACKL3 = {127, 127, 0, ON, ON};
inline const rollerState DESCORE = {0, 0, 0, LEAVE, OFF};
inline const rollerState STOP = {0, 0, 0, OFF, ON};


class Roller
{
public:
    Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &back, Piston &flipPiston, Piston &blockerPiston);
    void setState(rollerState state);
    rollerState getState() const;
private:
    pros::Motor &front;
    pros::Motor &middle;
    pros::Motor &back;
    Piston &flipPiston;
    Piston &blockerPiston;
    rollerState state;
};
struct StateControllerMapping
{
    rollerState state;
    pros::controller_digital_e_t button;
    bool temp;
    bool buttonCombination = false;
    pros::controller_digital_e_t button2 = pros::E_CONTROLLER_DIGITAL_A;
    short importance = 1;
};
class Robot {
public:
    Robot(Roller &roller, Piston &matchLoader, pros::Controller &master);
    void intake();
    void setState(rollerState state);
    void addTempState(rollerState state, short importance);
    void addTempState(tempState state);
    void scoreL1();
    void scoreL2(bool front = true);
    void scoreL3(bool front = true);
    void matchLoad(bool value, bool intake = true);
    void toggleMatchLoad();
    void mapButtons(StateControllerMapping mapping);
    void runImportance();
    void clearTempStates();
    void clearTempState(rollerState tempState);
    void removeTempState(rollerState tempState);
    void driverControl();
    rollerState getRollerState() const;
private:
    Roller &roller;
    Piston &matchLoader;
    pros::Controller &master;
    std::vector<StateControllerMapping> stateMappings;
    std::vector<tempState> tempStates;
    rollerState defaultState;
};

#endif