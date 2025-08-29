#include "piston.hpp"

void Piston::init()
{
    piston->set_value(state);
}

void Piston::toggle()
{
    state = !state;
    if (state)
    {
        piston->set_value(true);
    }
    else
    {
        piston->set_value(false);
    }
}

void Piston::setState(bool Istate)
{
    state = Istate;
    piston->set_value(state);
}

bool Piston::getState() const
{
    return state;
}
