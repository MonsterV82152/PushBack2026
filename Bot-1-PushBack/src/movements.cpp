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
    inline rollerState currentState = {"none", 0, 0, 0, 0};

    inline std::vector<rollerState> rollerStates = {
        {"intake", 80, 80, 80, 0},
        {"intakeC", 127, 127, 127, 0},
        {"outtake", -127, -127, -127, 0},
        {"scoreBottom", -127, -127, -127, 127},
        {"scoreMiddle", 30, -30, 0, 127},
        {"scoreMiddleC", 127, -127, 127, 0},
        {"scoreTop", 60, 60, -60, 127},
        {"scoreTopC", 127, 127, -127, 0},
        {"scoreTopCT", 127, 127, -127, 0},
        {"directIntake", 127, 0, 0, -127},
        {"cycle", 127, 127, 127, 127},
        {"cycleC", 127, 127, 127, 127},
        {"cycleCT", 127, 127, 127, 127},
        {"none", 0, 0, 0, 0},
        {"ejectMiddle", 127, -127, 0, 127}
    };
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
                currentState = rollerState;
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
                currentState = newState;
                return;
            }
        }
        rollerStates.push_back(newState);
        bottom.move(newState.bottomSpeed);
        middle.move(newState.middleSpeed);
        top.move(newState.topSpeed);
        bucket.move(newState.bucketSpeed);
        state = newState;
        currentState = newState;
    }
    inline void _runLowestTemporaryState()
    {
        if (temporaryRollerStates.empty())
        {
            bottom.move(state.bottomSpeed);
            middle.move(state.middleSpeed);
            top.move(state.topSpeed);
            bucket.move(state.bucketSpeed);
            currentState = state;
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
        currentState.name = lowest.name;
        currentState.bottomSpeed = lowest.bottomSpeed;
        currentState.middleSpeed = lowest.middleSpeed;
        currentState.topSpeed = lowest.topSpeed;
        currentState.bucketSpeed = lowest.bucketSpeed;
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
    inline bool findState(std::string a) {
        if (state.name == a) {
            return true;
        }
        for (const auto &tempState : temporaryRollerStates) {
            if (tempState.name == a) {
                return true;
            }
        }
        return false;
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
    inline double redMax = 40;
    inline double redMin = 350;
    inline double blueMax = 240;
    inline double blueMin = 170;
    inline bool redTeam = true;
    inline bool on = true;
    inline void start(void *param)
    {
        bool top = false;
        bool middle = false;
        topColor.set_led_pwm(100);
        bottomColor.set_led_pwm(100);
        while (true)
        {
            if (rollers::currentState.name != "none" && !matchLoader.getState() && on);
            {
                double topHue = topColor.get_hue();
                double bottomHue = bottomColor.get_hue();

                bool bottomRed = bottomHue < redMax || bottomHue > redMin;
                bool bottomBlue = bottomHue < blueMax && bottomHue > blueMin;
                if (bottomRed) {
                    if (!redTeam) {
                        if (autonSelect.isSkills()) {
                            if (rollers::findState("scoreMiddle")) {
                                rollers::addTemporaryState("cycleC", 1);
                            }
                        } else {
                            if (rollers::findState("scoreMiddle")) {
                                rollers::addTemporaryState("scoreTopC", 1);
                            } else if (rollers::findState("intake")) {
                                rollers::addTemporaryState("scoreMiddleC", 1);
                            }
                        }
                    } else {
                        rollers::removeTemporaryState("cycleC");
                        rollers::removeTemporaryState("scoreTopC");
                        rollers::removeTemporaryState("scoreMiddleC");
                    }
                }
                if (bottomBlue) {
                    if (redTeam) {
                        if (autonSelect.isSkills()) {
                            if (rollers::findState("scoreMiddle")) {
                                rollers::addTemporaryState("cycleC", 1);
                            }
                        } else {
                            if (rollers::findState("scoreMiddle")) {
                                rollers::addTemporaryState("scoreTopC", 1);
                            } else if (rollers::findState("intake")) {
                                rollers::addTemporaryState("scoreMiddleC", 1);
                            }
                        }
                    } else {
                        rollers::removeTemporaryState("cycleC");
                        rollers::removeTemporaryState("scoreTopC");
                        rollers::removeTemporaryState("scoreMiddleC");
                    }
                }

                bool topRed = topHue < redMax || topHue > redMin;
                bool topBlue = topHue < blueMax && topHue > blueMin;
                if (topRed) {
                    if (!redTeam) {
                        if (autonSelect.isSkills()) {
                            if (rollers::findState("scoreTop")) {
                                rollers::addTemporaryState("cycleC", 1);
                            }
                        } else {
                            if (rollers::findState("scoreTop")) {
                                rollers::addTemporaryState("cycleC", 1);
                            } else if (rollers::findState("intake")) {
                                rollers::addTemporaryState("scoreTopC", 1);
                            }
                        }
                    } else {
                        rollers::removeTemporaryState("cycleC");
                        rollers::removeTemporaryState("scoreTopC");
                    }
                }
                if (topBlue) {
                    if (redTeam) {
                        if (autonSelect.isSkills()) {
                            if (rollers::findState("scoreTop")) {
                                rollers::addTemporaryState("cycleC", 1);
                            }
                        } else {
                            if (rollers::findState("scoreTop")) {
                                rollers::addTemporaryState("cycleC", 1);
                            } else if (rollers::findState("intake")) {
                                rollers::addTemporaryState("scoreTopC", 1);
                            }
                        }
                    } else {
                        rollers::removeTemporaryState("cycleC");
                        rollers::removeTemporaryState("scoreTopC");
                    }
                }
            }           
            pros::delay(10);
        }
    }

}

#endif