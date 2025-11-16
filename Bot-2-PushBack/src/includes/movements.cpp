#include "movements.hpp"

bool operator==(const rollerState &lhs, const rollerState &rhs)
{
    return lhs.front == rhs.front &&
           lhs.middle == rhs.middle &&
           lhs.back == rhs.back &&
           lhs.flipPiston == rhs.flipPiston &&
           lhs.blockerPiston == rhs.blockerPiston;
}

Roller::Roller(pros::Motor &front, pros::Motor &middle, pros::Motor &intake, pros::Motor &back, Piston &flipPiston, Piston &blockerPiston, Piston &intakeLift)
    : front(front), middle(middle), intake(intake), back(back), flipPiston(flipPiston), blockerPiston(blockerPiston), intakeLift(intakeLift), state(STOP)
{
}

void Roller::setState(rollerState state)
{
    if (state.voltage)
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
    }
    else
    {
        if (state.front != LEAVE)
        {
            front.move_velocity(state.front);
        }
        if (state.middle != LEAVE)
        {
            middle.move_velocity(state.middle);
        }
        if (state.intake != LEAVE)
        {
            intake.move_velocity(state.intake);
        }
        if (state.back != LEAVE)
        {
            back.move_velocity(state.back);
        }
    }
    if (state.flipPiston != LEAVE)
    {
        flipPiston.setState(state.flipPiston == 1);
    }
    if (state.blockerPiston != LEAVE)
    {
        blockerPiston.setState(state.blockerPiston == 1);
    }
    if (state.intakeL != LEAVE)
    {
        intakeLift.setState(state.intakeL == 1);
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
    runImportance("Intake");
}

void Robot::stop()
{
    defaultState = STOP;
    runImportance("Stop");
}

void Robot::toggleIntake()
{
    if (defaultState == INTAKE || defaultState == INTAKE2 || defaultState == INTAKE3)
        defaultState = STOP;
    else
        defaultState = INTAKE;
    runImportance("Toggle Intake");
}

void Robot::scoreL1()
{
    defaultState = L1SKILLS;
    runImportance("Score L1");
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
    runImportance("Score L2");
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
    runImportance("Score L3");
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
        runImportance("Match Load");
    }
}

void Robot::toggleMatchLoad()
{
    matchLoader.toggle();
    if (matchLoader.getState())
    {
        defaultState = INTAKE;
    }
    runImportance("Toggle Match Load");
}

void Robot::mapButtons(StateControllerMapping mapping)
{
    stateMappings.push_back(mapping);
}

void Robot::runImportance(std::string reason)
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
    std::cout << reason << ": Current State: " << highest.state.front << ", " << highest.state.middle << ", " << highest.state.back << ", " << highest.state.flipPiston << ", " << highest.state.blockerPiston << std::endl;
}

void Robot::setState(rollerState state)
{
    defaultState = state;
    runImportance("Manual Set State");
}

void Robot::addTempState(rollerState state, short importance)
{
    tempStates.push_back({state, importance});
    runImportance("Add Temp State");
}

void Robot::addTempState(tempState state)
{
    tempStates.push_back(state);
    runImportance("Add Temp State");
}

void Robot::clearTempStates()
{
    tempStates.clear();
    runImportance("Clear All Temp States");
}

void Robot::clearTempState(rollerState tempState)
{
    for (auto it = tempStates.begin(); it != tempStates.end();)
    {
        if (it->state == tempState)
        {
            it = tempStates.erase(it);
            runImportance("Clear Temp State");
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
            runImportance("Remove Temp State");
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
                runImportance("Driver Control Temp State");
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
                runImportance("Driver Control Set State");
            }
        }
        else if (mapping.temp && !(master.get_digital(mapping.button) && combinationPressed) && roller.getState() == mapping.state)
        {
            if (roller.getState() == mapping.state)
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
                        runImportance("Driver Control Remove Temp State");
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
}

rollerState Robot::getRollerState() const
{
    return roller.getState();
}

rollerState Robot::getDefaultState() const
{
    return defaultState;
}
