#include "piston.hpp"

void Piston::init()
{
    piston->set_value(state);
    if (doublePiston && piston2 != nullptr)
    {
        piston2->set_value(!state);
    }
}

void Piston::toggle()
{
    state = !state;
    if (state)
    {
        piston->set_value(true);
        if (doublePiston && piston2 != nullptr)
        {
            piston2->set_value(false);
        }
    }
    else
    {
        piston->set_value(false);
    }
}

void Piston::setState(bool Istate)
{
    if (Istate == state)
    {
        return;
    }
    state = Istate;
    piston->set_value(state);
    if (doublePiston && piston2 != nullptr)
    {
        piston2->set_value(!state);
    }
}

bool Piston::getState() const
{
    return state;
}
