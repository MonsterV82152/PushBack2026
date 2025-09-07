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
inline const rollerState L1 = {-127, -127, -127, OFF, LEAVE};
inline const rollerState L2 = {127, 127, -127, OFF, LEAVE};
inline const rollerState L3 = {127, 127, 127, OFF, OFF};
inline const rollerState BACKL2 = {-127, 127, 0, OFF, ON};
inline const rollerState BACKL3 = {127, 127, 0, ON, ON};
inline const rollerState DESCORE = {0, 0, 0, LEAVE, OFF};
inline const rollerState STOP = {0, 0, 0, OFF, ON};
inline const rollerState COLOURSORT = {-127, LEAVE, LEAVE, LEAVE, LEAVE};
inline const rollerState PARK = {-100, -100, -100, OFF, ON};
inline const rollerState PARK2 = {-20, -20, -20, OFF, ON};


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
    /**
     * Constructs a new Robot object
     * @param roller the Roller object to control the rollers
     * @param matchLoader the Piston object to control the match loader piston
     * @param master the Controller object to get button inputs from
     */
    Robot(Roller &roller, Piston &matchLoader, pros::Controller &master);
    /**
     * Sets the roller to intake state
     */
    void intake();
    /**
     * Stops the roller
     * Sets the roller to STOP state
     */
    void stop();
    /**
     * Toggles intake on and off
     */
    void toggleIntake();
    /**
     * Sets the roller to a specific state
     * @param state the rollerState to set the roller to
     */
    void setState(rollerState state);
    /**
     * Adds a temporary state with given importance
     */
    void addTempState(rollerState state, short importance);
    /**
     * Adds a temporary state
     */
    void addTempState(tempState state);
    /**
     * Score on L1 goal
     */
    void scoreL1();
    /**
     * Score on L2 goal
     * @param front true to score on front flip side, false to score on back side (default true)
     */
    void scoreL2(bool front = true);
    /**
     * Score on L3 goal
     * @param front true to score on front flip side, false to score on back side (default true)
     */
    void scoreL3(bool front = true);
    /**
     * Set match loader piston state
     * @param value true to set match loader piston to on position, false for off position
     * @param intake if true, set roller to intake state when match loader is set to on position
     * Default true
     */
    void matchLoad(bool value, bool intake = true);
    /**
     * Toggles match loader piston state
     */
    void toggleMatchLoad();
    /**
     * Maps a button to a roller state
     * @param mapping the StateControllerMapping struct containing the mapping information
     */
    void mapButtons(StateControllerMapping mapping);
    /**
     * Runs the highest importance temporary state, or default state if no temporary states exist
     */
    void runImportance();
    /**
     * Clears all temporary states
     */
    void clearTempStates();
    /**
     * Clears all instances of a specific temporary state
     * @param tempState the temporary state to clear
     */
    void clearTempState(rollerState tempState);
    /**
     * Removes the first instance of a specific temporary state
     * @param tempState the temporary state to remove
     */
    void removeTempState(rollerState tempState);
    /**
     * Call this function in opcontrol to enable button mappings
     */
    void driverControl();
    /**
     * Gets the current roller state
     * @return the current rollerState
     */
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