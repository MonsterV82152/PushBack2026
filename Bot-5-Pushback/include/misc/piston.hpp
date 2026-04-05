#include "main.h"

#ifndef PISTON_CPP
#define PISTON_CPP

/**
 * @class Piston
 * @brief A class to control a pneumatic piston using an ADI Digital Output
 */
class Piston
{
private:
    pros::ADIDigitalOut *piston;
    pros::ADIDigitalOut *piston2;
    bool state;
    bool doublePiston;

public:
    /**
     * @brief Constructor for Piston class
     * @param piston Pointer to the ADIDigitalOut object controlling the piston
     */
    Piston(pros::ADIDigitalOut *piston)
        : piston(piston),
          piston2(nullptr),
          state(false),
          doublePiston(false)
    {
    }
    /**
     * @brief Constructor for Piston class
     * @param piston Pointer to the ADIDigitalOut object controlling the piston
     */
    Piston(pros::ADIDigitalOut *piston, pros::ADIDigitalOut *piston2)
        : piston(piston),
          piston2(piston2),
          state(false),
          doublePiston(true)
    {
    }
    /**
     * @brief Initializes the piston to a default state (retracted)
     */
    void init();
    /**
     * @brief Toggles the state of the piston
     */
    void toggle();
    /**
     * @brief Sets the state of the piston
     * @param Istate The desired state of the piston (true for extended, false for retracted)
     */
    void setState(bool Istate);
    /**
     * @brief Gets the current state of the piston
     * @return The current state of the piston (true for extended, false for retracted)
     */
    bool getState() const;
};

#endif