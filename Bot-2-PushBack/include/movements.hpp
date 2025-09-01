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
    short switchPiston = 0;
    short scorePiston = 1;
};

inline const rollerState INTAKE = {127, 127, 40, OFF, ON};
inline const rollerState L1 = {0, -127, -127, OFF, LEAVE};
inline const rollerState L2 = {127, 127, -127, OFF, LEAVE};
inline const rollerState L3 = {127, 127, 127, OFF, OFF};
inline const rollerState BACKL2 = {-127, 127, 0, OFF, ON};
inline const rollerState BACKL3 = {127, 127, 0, ON, ON};
inline const rollerState STOP = {0, 0, 0, OFF, LEAVE};


class Roller
{
public:
    Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &back, pros::ADIDigitalOut &switchPiston, pros::ADIDigitalOut &scorePiston);
    void setState(rollerState state);
    rollerState getState() const;
private:
    pros::Motor &front;
    pros::Motor &middle;
    pros::Motor &back;
    pros::ADIDigitalOut &switchPiston;
    pros::ADIDigitalOut &scorePiston;
    rollerState state;
};

class Robot {
public:
    Robot(Roller &roller, Piston &matchLoader);
    void intake();
    void scoreL1();
    void scoreL2(bool front = true);
    void scoreL3(bool front = true);

    rollerState getRollerState() const;
private:
    Roller &roller;
    Piston &matchLoader;
    
};

#endif