#ifndef MOVEMENTS_CPP
#define MOVEMENTS_CPP

#include "globals.hpp"
#include "AutonSelector.hpp"

namespace rollers
{
    struct rollerState
    {
        std::string name;
        double bottomSpeed;
        double middleSpeed;
        double topSpeed;
        double bucketSpeed;
    };
    struct temporaryRollerState
    {
        std::string name;
        double bottomSpeed;
        double middleSpeed;
        double topSpeed;
        double bucketSpeed;
        int importance; // 0 for high importance, 10 for low importance
    };
    inline rollerState state = {"none", 0, 0, 0, 0};

    inline std::vector<rollerState> rollerStates = {
        {"intake", 127, 127, 127, 0},
        {"outtake", -127, -127, -127, 0},
        {"scoreBottom", -127, -127, -127, 127},
        {"scoreMiddle", 30, -30, 0, 127},
        {"scoreTop", 60, 60, -60, 127},
        {"directIntake", 127, 0, 0, -127},
        {"cycle", 127, 127, 127, 127},
        {"cycleC", 127, 127, 127, 127},
        {"none", 0, 0, 0, 0}};
    inline std::vector<temporaryRollerState> temporaryRollerStates;
    inline temporaryRollerState currentTemporaryState = {"none", 0, 0, 0, 0, 10};
    inline temporaryRollerState _stateToTemp(rollerState state, int importance = 0)
    {
        return temporaryRollerState{
            state.name,
            state.bottomSpeed,
            state.middleSpeed,
            state.topSpeed,
            state.bucketSpeed,
            importance};
    }

    inline void setState(std::string newState)
    {
        for (const auto &rollerState : rollerStates)
        {
            if (rollerState.name == newState)
            {
                if (temporaryRollerStates.empty())
                {
                    bottom.move(rollerState.bottomSpeed);
                    middle.move(rollerState.middleSpeed);
                    top.move(rollerState.topSpeed);
                    bucket.move(rollerState.bucketSpeed);
                }
                state = rollerState;
                return;
            }
        }

        std::cerr << "Invalid roller state: " << newState << std::endl;
    }
    inline void setState(rollerState newState)
    {
        for (const auto &rollerState : rollerStates)
        {
            if (rollerState.name == newState.name)
            {
                bottom.move(newState.bottomSpeed);
                middle.move(newState.middleSpeed);
                top.move(newState.topSpeed);
                bucket.move(newState.bucketSpeed);
                state = newState;
                return;
            }
        }
        rollerStates.push_back(newState);
        bottom.move(newState.bottomSpeed);
        middle.move(newState.middleSpeed);
        top.move(newState.topSpeed);
        bucket.move(newState.bucketSpeed);
        state = newState;
    }
    inline void _runLowestTemporaryState()
    {
        if (temporaryRollerStates.empty())
        {
            bottom.move(state.bottomSpeed);
            middle.move(state.middleSpeed);
            top.move(state.topSpeed);
            bucket.move(state.bucketSpeed);
            return;
        }
        temporaryRollerState lowest = temporaryRollerStates[0];
        for (const auto &tempState : temporaryRollerStates)
        {
            if (tempState.importance <= lowest.importance)
            {
                lowest = tempState;
            }
        }
        currentTemporaryState = lowest;
        bottom.move(lowest.bottomSpeed);
        middle.move(lowest.middleSpeed);
        top.move(lowest.topSpeed);
        bucket.move(lowest.bucketSpeed);
    }
    inline void addTemporaryState(std::string newState, int importance)
    {
        for (const auto &rollerState : rollerStates)
        {
            if (rollerState.name == newState)
            {
                temporaryRollerStates.push_back(_stateToTemp(rollerState, importance));
                break;
            }
        }
        _runLowestTemporaryState();
    }

    inline void addTemporaryState(rollerState newState, int importance)
    {
        temporaryRollerStates.push_back(_stateToTemp(newState, importance));
        _runLowestTemporaryState();
        for (const auto &rollerState : rollerStates)
        {
            if (rollerState.name == newState.name)
            {
                return;
            }
        }
        rollerStates.push_back(newState);
    }
    inline void removeTemporaryState(std::string stateName)
    {
        for (auto it = temporaryRollerStates.begin(); it != temporaryRollerStates.end(); ++it)
        {
            if (it->name == stateName)
            {
                temporaryRollerStates.erase(it);
                break;
            }
        }
        _runLowestTemporaryState();
    }
    inline void removeAllTemporaryState()
    {
        temporaryRollerStates.clear();
        _runLowestTemporaryState();
    }

    inline void intake()
    {
        setState("intake");
    }
    inline void outtake()
    {
        setState("outtake");
    }
    inline void stop()
    {
        setState("none");
    }
    inline void scoreMiddle()
    {
        setState("scoreMiddle");
    }
    inline void scoreTop()
    {
        setState("scoreTop");
    }
    inline void scoreBottom()
    {
        setState("scoreBottom");
    }
}

namespace colourSort
{
    inline double redMax = 30;
    inline double redMin = 350;
    inline double blueMax = 240;
    inline double blueMin = 170;
    inline bool redTeam = true;
    inline void start(void *param)
    {

        topColor.set_led_pwm(100);
        while (true)
        {
            double topHue = topColor.get_hue();

            if (!redTeam)
            {
                if (autonSelect.isSkills())
                {
                    if ((topHue > redMin || topHue < redMax) && rollers::currentTemporaryState.name == "scoreTop")
                    {
                        rollers::addTemporaryState("cycleC", 1);
                        pros::delay(350);
                        rollers::removeTemporaryState("cycleC");
                    }
                }
            }
            else
            {
                if (autonSelect.isSkills())
                {
                    if (topHue > blueMin && topHue < blueMax && rollers::currentTemporaryState.name == "scoreTop")
                    {
                        rollers::addTemporaryState("cycleC", 1);
                        pros::delay(350);
                        rollers::removeTemporaryState("cycleC");
                    }
                }
                        }
            pros::delay(20);
        }
    }

}

#endif