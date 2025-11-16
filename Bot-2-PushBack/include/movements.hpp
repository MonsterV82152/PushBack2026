/**
 * @file movements.hpp
 * @brief Robot movement and roller control system for VEX High Stakes
 *
 * This file defines the roller and robot control system for managing game piece
 * manipulation in the VEX High Stakes competition. It includes:
 * - Roller state management for intake, scoring, and manipulation
 * - Pre-defined states for common game actions (L1, L2, L3 scoring)
 * - Button mapping system for driver control
 * - Temporary state system with priority management
 *
 * The system controls multiple motors and pistons to handle rings through
 * various stages: intake, transport, and scoring at different goal heights.
 */

#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "main.h"
#include "piston.hpp"

// Piston state constants
#define ON 1      ///< Piston extended/activated
#define OFF 0     ///< Piston retracted/deactivated
#define LEAVE 128 ///< Leave piston in current state

/**
 * @struct rollerState
 * @brief Defines the complete state of all roller motors and pistons
 *
 * This structure encapsulates the state of the entire roller system including
 * four motors (back, middle, intake, front) and three pneumatic pistons.
 * Motor values range from -200 to 200 (or -127 to 127 in voltage mode).
 * Piston values use ON (1), OFF (0), or LEAVE (128) to maintain current state.
 */
struct rollerState
{
    short back = 0;          ///< Back roller motor speed (-200 to 200)
    short middle = 0;        ///< Middle roller motor speed (-200 to 200)
    short intake = 0;        ///< Intake roller motor speed (-200 to 200)
    short front = 0;         ///< Front roller motor speed (-200 to 200)
    short flipPiston = 0;    ///< Flip piston state (ON/OFF/LEAVE)
    short blockerPiston = 0; ///< Blocker piston state (ON/OFF/LEAVE)
    short intakeL = 0;       ///< Intake lift piston state (ON/OFF/LEAVE)
    bool voltage = false;    ///< If true, use voltage mode instead of velocity
};

/**
 * @struct tempState
 * @brief Wrapper for temporary roller states with priority
 *
 * Temporary states allow higher-priority actions to override the default state
 * temporarily. Higher importance values take precedence.
 */
struct tempState
{
    rollerState state; ///< The roller state configuration
    short importance;  ///< Priority level (higher values = higher priority)
};

/**
 * @brief Equality operator for comparing roller states
 * @param lhs Left-hand side rollerState
 * @param rhs Right-hand side rollerState
 * @return true if states are identical, false otherwise
 */
bool operator==(const rollerState &lhs, const rollerState &rhs);

// ============================================================================
// Pre-defined Roller States for Common Game Actions
// ============================================================================

inline const pros::rtos::Clock isAuton = pros::rtos::Clock(); ///< Clock for autonomous timing

// Basic intake and movement states
inline const rollerState INTAKE = {200, 200, 200, 200, OFF, OFF};    ///< Standard intake from ground
inline const rollerState INTAKE2 = {200, 200, 200, 0, OFF, OFF};     ///< Intake variant without front roller
inline const rollerState INTAKE3 = {0, 1, 127, 0, OFF, OFF, true};   ///< Low-power intake (voltage mode)
inline const rollerState BACKINTAKE = {200, 200, 200, 200, ON, OFF}; ///< Intake with flip piston extended
inline const rollerState ONLYINTAKE = {0, 0, 200, 0, OFF, OFF};      ///< Only intake roller active

// L1 (Low Goal) scoring states
inline const rollerState L1SKILLS = {-50, -100, -50, -50, OFF, LEAVE, ON}; ///< L1 scoring optimized for skills
inline const rollerState L1 = {-150, -150, -150, -150, OFF, LEAVE, ON};    ///< Standard L1 scoring (match)
inline const rollerState L1AUTO = {-50, -100, -50, -50, OFF, LEAVE, OFF};  ///< L1 scoring for autonomous

// L2 (Medium Goal) scoring states
inline const rollerState L2 = {200, 200, 200, -200, OFF, LEAVE};                    ///< Standard L2 front scoring
inline const rollerState L2SKILLS = {200, 150, 200, -40, OFF, LEAVE};               ///< L2 scoring optimized for skills
inline const rollerState L2AUTO = {200, 200, 100, -100, OFF, LEAVE};                ///< L2 scoring for autonomous
inline const rollerState L2HELPER = {-127, -127, -127, -60, OFF, LEAVE, OFF, true}; ///< L2 assist (voltage mode)
inline const rollerState BACKL2 = {-200, 200, 200, 0, OFF, OFF};                    ///< L2 back scoring

// L3 (High Goal) scoring states
inline const rollerState L3 = {200, 200, 200, 200, OFF, ON};     ///< Standard L3 front scoring
inline const rollerState L3HELPER = {0, 0, 0, 200, OFF, ON};     ///< L3 assist with front roller only
inline const rollerState L3AUTO = {200, 200, 200, 150, OFF, ON}; ///< L3 scoring for autonomous
inline const rollerState BACKL3 = {200, 200, 200, 0, ON, ON};    ///< L3 back scoring
inline const rollerState BACKL3HELPER = {200, 0, 0, 0, ON, ON};  ///< L3 back assist

// Special operation states
inline const rollerState DESCORE = {0, 0, 0, 0, LEAVE, ON};                      ///< Descore opponent rings
inline const rollerState STOP = {0, 0, 0, 0, OFF, OFF};                          ///< Stop all rollers and retract pistons
inline const rollerState COLOURSORT = {-200, LEAVE, LEAVE, LEAVE, LEAVE, LEAVE}; ///< Color sorting (reject wrong color)
inline const rollerState PARK = {-200, -200, -200, -200, OFF, OFF};              ///< Park position (high speed reverse)
inline const rollerState PARK2 = {-50, -50, -50, -50, OFF, OFF};                 ///< Park position (low speed reverse)

/**
 * @class Roller
 * @brief Low-level controller for the roller system motors and pistons
 *
 * The Roller class provides direct control over the four roller motors and three
 * pneumatic pistons. It applies rollerState configurations to the physical hardware
 * and tracks the current state.
 *
 * This class handles the hardware interface while the Robot class provides higher-level
 * game logic and driver control integration.
 */
class Roller
{
public:
    /**
     * @brief Constructor for Roller
     * @param front Front roller motor
     * @param middle Middle roller motor
     * @param intake Intake roller motor
     * @param back Back roller motor
     * @param flipPiston Flip piston for redirecting rings
     * @param blockerPiston Blocker piston for controlling ring flow
     * @param intakeLift Intake lift piston for raising/lowering intake
     */
    Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &intake, pros::Motor &back, Piston &flipPiston, Piston &blockerPiston, Piston &intakeLift);

    /**
     * @brief Sets the roller system to a specific state
     *
     * Applies motor velocities (or voltages) and piston positions according to
     * the provided rollerState. Motors set to LEAVE maintain their current state.
     *
     * @param state The desired rollerState configuration
     */
    void setState(rollerState state);

    /**
     * @brief Gets the current roller state
     * @return The current rollerState configuration
     */
    rollerState getState() const;

private:
    pros::Motor &front;  ///< Reference to front roller motor
    pros::Motor &middle; ///< Reference to middle roller motor
    pros::Motor &intake; ///< Reference to intake roller motor
    pros::Motor &back;   ///< Reference to back roller motor

    Piston &flipPiston;    ///< Reference to flip piston
    Piston &blockerPiston; ///< Reference to blocker piston
    Piston &intakeLift;    ///< Reference to intake lift piston
    rollerState state;     ///< Current state of the roller system
};

/**
 * @struct StateControllerMapping
 * @brief Maps controller buttons to roller states for driver control
 *
 * Defines the relationship between controller button inputs and roller states.
 * Supports single buttons, button combinations, temporary states, and importance levels.
 */
struct StateControllerMapping
{
    rollerState state;                                                   ///< Roller state to activate
    pros::controller_digital_e_t button;                                 ///< Primary button trigger
    bool temp;                                                           ///< If true, state is temporary
    bool buttonCombination = false;                                      ///< If true, requires two buttons
    pros::controller_digital_e_t button2 = pros::E_CONTROLLER_DIGITAL_A; ///< Secondary button for combinations
    short importance = 1;                                                ///< Priority level for temporary states
};

/**
 * @class Robot
 * @brief High-level robot control system integrating roller control with driver input
 *
 * The Robot class provides game-logic-level control of the robot's roller system,
 * including:
 * - Predefined actions for scoring at different goal heights (L1, L2, L3)
 * - Driver control through controller button mappings
 * - Priority-based temporary state system for complex sequences
 * - Match loader integration for loading rings during skills
 *
 * This class manages the coordination between user input, game strategy, and
 * the low-level Roller hardware control.
 */
class Robot
{
public:
    /**
     * @brief Constructs a new Robot object
     * @param roller The Roller object to control the roller system
     * @param matchLoader The Piston object to control the match loader
     * @param master The Controller object for driver input
     */
    Robot(Roller &roller, Piston &matchLoader, pros::Controller &master);

    /**
     * @brief Sets the roller to intake state
     *
     * Activates all rollers to bring rings from the ground into the robot.
     */
    void intake();

    /**
     * @brief Stops the roller system
     *
     * Sets all rollers to zero velocity and retracts all pistons.
     */
    void stop();

    /**
     * @brief Toggles intake on and off
     *
     * Switches between INTAKE and STOP states based on current state.
     */
    void toggleIntake();

    /**
     * @brief Sets the roller to a specific state
     * @param state The desired rollerState configuration
     */
    void setState(rollerState state);

    /**
     * @brief Adds a temporary state with given importance
     *
     * Temporary states override the default state until cleared. Multiple temporary
     * states can coexist, with the highest importance taking precedence.
     *
     * @param state The rollerState to add as temporary
     * @param importance Priority level (higher values override lower)
     */
    void addTempState(rollerState state, short importance);

    /**
     * @brief Adds a temporary state
     * @param state The tempState structure containing state and importance
     */
    void addTempState(tempState state);

    /**
     * @brief Score on L1 (low) goal
     *
     * Configures rollers to score rings into the low goal (18 inches).
     */
    void scoreL1();

    /**
     * @brief Score on L2 (medium) goal
     *
     * Configures rollers to score rings into the medium goal (36 inches).
     *
     * @param front If true, score from front side; if false, score from back (default: true)
     */
    void scoreL2(bool front = true);

    /**
     * @brief Score on L3 (high) goal
     *
     * Configures rollers to score rings into the high goal (60 inches).
     *
     * @param front If true, score from front side; if false, score from back (default: true)
     */
    void scoreL3(bool front = true);

    /**
     * @brief Set match loader piston state
     *
     * Controls the match loader for loading pre-loaded rings during skills runs.
     *
     * @param value If true, extend match loader; if false, retract
     * @param intake If true, automatically activate intake when extending (default: true)
     */
    void matchLoad(bool value, bool intake = true);

    /**
     * @brief Toggles match loader piston state
     *
     * Switches match loader between extended and retracted positions.
     */
    void toggleMatchLoad();

    /**
     * @brief Maps a controller button to a roller state
     *
     * Registers a button mapping that will be processed during driverControl().
     * Supports single buttons, button combinations, and temporary/permanent states.
     *
     * @param mapping The StateControllerMapping structure defining the button-state relationship
     */
    void mapButtons(StateControllerMapping mapping);

    /**
     * @brief Runs the highest importance temporary state, or default state if none exist
     *
     * Evaluates all temporary states and applies the one with highest importance.
     * If no temporary states exist, applies the default state.
     *
     * @param reason Debug string describing why this was called (for logging)
     */
    void runImportance(std::string reason);

    /**
     * @brief Clears all temporary states
     *
     * Removes all temporary states, causing the default state to be applied.
     */
    void clearTempStates();

    /**
     * @brief Clears all instances of a specific temporary state
     *
     * Removes all occurrences of the specified state from the temporary states list.
     *
     * @param tempState The rollerState to remove all instances of
     */
    void clearTempState(rollerState tempState);

    /**
     * @brief Removes the first instance of a specific temporary state
     *
     * Removes only the first occurrence of the specified state from the list.
     *
     * @param tempState The rollerState to remove one instance of
     */
    void removeTempState(rollerState tempState);

    /**
     * @brief Main driver control loop - call this in opcontrol()
     *
     * Continuously monitors controller button inputs and applies mapped roller states.
     * This should be called repeatedly in the operator control loop.
     */
    void driverControl();

    /**
     * @brief Gets the current roller state
     * @return The current rollerState of the roller system
     */
    rollerState getRollerState() const;

    /**
     * @brief Gets the default roller state
     * @return The default rollerState used when no temporary states are active
     */
    rollerState getDefaultState() const;

private:
    Roller &roller;                                    ///< Reference to Roller object
    Piston &matchLoader;                               ///< Reference to match loader piston
    pros::Controller &master;                          ///< Reference to driver controller
    std::vector<StateControllerMapping> stateMappings; ///< List of button-to-state mappings
    std::vector<tempState> tempStates;                 ///< List of active temporary states
    rollerState defaultState;                          ///< Default state when no temp states active
};

#endif