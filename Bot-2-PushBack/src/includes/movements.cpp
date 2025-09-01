#include "movements.hpp"

Roller::Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &back, pros::ADIDigitalOut &switchPiston, pros::ADIDigitalOut &scorePiston)
    : front(front), middle(middle), back(back), switchPiston(switchPiston), scorePiston(scorePiston), state(STOP)
{
}

void Roller::setState(rollerState state)
{
    if (state.front != LEAVE)
    {
        front.move(state.front);
    }
    if (state.middle != LEAVE)
    {
        middle.move(state.middle);
    }
    if (state.back != LEAVE)
    {
        back.move(state.back);
    }
    if (state.switchPiston != LEAVE)
    {
        switchPiston.set_value(state.switchPiston);
    }
    if (state.scorePiston != LEAVE)
    {
        scorePiston.set_value(state.scorePiston);
    }
    this->state = state;
}

rollerState Roller::getState() const
{
    return state;
}

