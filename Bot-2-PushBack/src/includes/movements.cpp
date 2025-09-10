#include "movements.hpp"

bool operator==(const rollerState &lhs, const rollerState &rhs)
{
    return lhs.front == rhs.front &&
           lhs.middle == rhs.middle &&
           lhs.back == rhs.back &&
           lhs.flipPiston == rhs.flipPiston &&
           lhs.blockerPiston == rhs.blockerPiston;
}

Roller::Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &intake, pros::Motor &back, Piston &flipPiston, Piston &blockerPiston)
    : front(front), middle(middle), intake(intake), back(back), flipPiston(flipPiston), blockerPiston(blockerPiston), state(STOP)
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
    if (state.intake != LEAVE)
    {
        intake.move(state.intake);
    }
    if (state.back != LEAVE)
    {
        back.move(state.back);
    }
    if (state.flipPiston != LEAVE)
    {
        flipPiston.setState(state.flipPiston == 1);
    }
    if (state.blockerPiston != LEAVE)
    {
        blockerPiston.setState(state.blockerPiston == 1);
    }
    this->state = state;
}

rollerState Roller::getState() const
{
    return state;
}

Robot::Robot(Roller &roller, Piston &matchLoader, pros::Controller &master)
    : roller(roller), matchLoader(matchLoader), master(master), defaultState(STOP)
{
}

void Robot::intake()
{
    defaultState = INTAKE;
    runImportance();
}

void Robot::stop()
{
    defaultState = STOP;
    runImportance();
}

void Robot::toggleIntake()
{
    if (defaultState == INTAKE || defaultState == INTAKE2 || defaultState == INTAKE3)
        defaultState = STOP;
    else
        defaultState = INTAKE;
    runImportance();
}

void Robot::scoreL1()
{
    defaultState = L1;
    runImportance();
}

void Robot::scoreL2(bool front)
{
    if (front)
    {
        defaultState = L2;
    }
    else
    {
        defaultState = BACKL2;
    }
    runImportance();
}

void Robot::scoreL3(bool front)
{
    if (front)
    {
        defaultState = L3;
    }
    else
    {
        defaultState = BACKL3;
    }
    runImportance();
}

void Robot::matchLoad(bool value, bool intake)
{
    if (matchLoader.getState() != value)
    {
        matchLoader.setState(value);
        if (intake && value)
        {
            defaultState = INTAKE;
        }
    }
    runImportance();
}

void Robot::toggleMatchLoad()
{
    matchLoader.toggle();
    if (matchLoader.getState())
    {
        defaultState = INTAKE;
    }
    runImportance();
}

void Robot::mapButtons(StateControllerMapping mapping)
{
    stateMappings.push_back(mapping);
}

void Robot::runImportance()
{
    // run the temp state with highest importance -> if none, run default state
    tempState highest = {defaultState, -1};
    for (const auto &temp : tempStates)
    {
        if (temp.importance > highest.importance)
        {
            highest = temp;
        }
    }
    roller.setState(highest.state);
}

void Robot::setState(rollerState state)
{
    defaultState = state;
    runImportance();
}

void Robot::addTempState(rollerState state, short importance)
{
    tempStates.push_back({state, importance});
}

void Robot::addTempState(tempState state)
{
    tempStates.push_back(state);
}

void Robot::clearTempStates()
{
    tempStates.clear();
    runImportance();
}

void Robot::clearTempState(rollerState tempState)
{
    for (auto it = tempStates.begin(); it != tempStates.end();)
    {
        if (it->state == tempState)
        {
            it = tempStates.erase(it);
            runImportance();
        }
        else
        {
            ++it;
        }
    }
}

void Robot::removeTempState(rollerState tempState)
{
    for (auto it = tempStates.begin(); it != tempStates.end();)
    {
        if (it->state == tempState)
        {
            it = tempStates.erase(it);
            runImportance();
            break;
        }
        else
        {
            ++it;
        }
    }
}

void Robot::driverControl()
{
    for (const auto &mapping : stateMappings)
    {
        bool combinationPressed = mapping.buttonCombination ? master.get_digital(mapping.button2) : true;
        if (master.get_digital_new_press(mapping.button) && combinationPressed)
        {
            if (mapping.temp)
            {
                tempStates.push_back({mapping.state, mapping.importance});
                runImportance();
            }
            else
            {
                if (defaultState == mapping.state)
                {
                    defaultState = STOP;
                }
                else
                {
                    defaultState = mapping.state;
                }
                runImportance();
            }
        }
        else if (mapping.temp && !(master.get_digital(mapping.button) && combinationPressed) && roller.getState() == mapping.state)
        {
            for (auto it = tempStates.begin(); it != tempStates.end();)
            {
                if (it->state == mapping.state)
                {
                    it = tempStates.erase(it);
                    if (defaultState == INTAKE2 || defaultState == INTAKE3)
                    {
                        defaultState = INTAKE;
                    }
                    runImportance();
                }
                else
                {
                    ++it;
                }
            }
        }
    }
}

rollerState Robot::getRollerState() const
{
    return roller.getState();
}
