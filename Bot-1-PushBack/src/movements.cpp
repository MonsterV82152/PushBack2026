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
        {"scoreMiddle", 60, -60, 127, 127},
        {"scoreMiddleAuton", 127, -127, 127, 127},

        {"scoreMiddleC", 127, -127, 127, 0},
        {"scoreTop", 60, 60, -60, 127},
        {"scoreTopC", 127, 127, -127, 0},
        {"scoreTopCT", 127, 127, -127, 0},
        {"directIntake", 127, 0, 0, -127},
        {"cycle", 127, 127, 127, 127},
        {"cycleC", 127, 127, 127, 127},
        {"cycleCT", 0, 127, 127, 0},
        {"none", 0, 0, 0, 0},
        {"reverseTop", 0, 0, -127, 0},

        {"clearIntake", -127, -127, -127, 0},
        {"clearIntakeC", -127, -127, -127, 0},

        {"ejectMiddle", 127, -127, 0, 127}};
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
    inline bool findState(std::string a)
    {
        if (state.name == a)
        {
            return true;
        }
        for (const auto &tempState : temporaryRollerStates)
        {
            if (tempState.name == a)
            {
                return true;
            }
        }
        return false;
    }
    inline temporaryRollerState findLowestState(int importance = 0)
    {
        for (temporaryRollerState i : temporaryRollerStates)
        {
            if (i.importance >= importance)
            {
                return i;
            }
        }
        return temporaryRollerState{state.name, state.bottomSpeed, state.middleSpeed, state.topSpeed, state.bucketSpeed, 0};
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
    struct timeout
    {
        int cycleCount;
        std::string name;
    };
    inline std::atomic<bool> redTeam(true);
    inline std::atomic<bool> on(true);
    inline std::atomic<bool> sortML(true);
    inline int command = 0;
    inline void stopAfter(int count)
    {
        command = count;
    }
    inline void start(void *params)
    {
        double redMax = 40;
        double redMin = 350;
        double blueMax = 240;
        double blueMin = 170;
        bool middle = true;
        bool top = true;
        
        std::vector<timeout> timeouts = {};
        std::vector<timeout> starts = {};
        std::deque<int> ballIndex = {};
        short currentState = 0;
        int cycleCount = 0;

        bottomColor.set_led_pwm(100);
        while (true)
        {
            std::string currentRollerState = rollers::findLowestState(2).name;
            cycleCount++;
            for (int i = 0; i < timeouts.size(); i++)
            {
                if (timeouts[i].cycleCount == cycleCount)
                {
                    rollers::removeTemporaryState(timeouts[i].name);
                    timeouts.erase(timeouts.begin() + i);
                }
            }
            for (int i = 0; i < starts.size(); i++)
            {
                if (starts[i].cycleCount == cycleCount)
                {
                    rollers::addTemporaryState(starts[i].name, 1);
                    starts.erase(starts.begin() + i);
                }
            }
            // Top sensor triggers colour sensor to read next block - fixes the ghost block issue
            if (rollers::currentState.name != "none" && on.load() && (!autonSelect.isSkills() && !matchLoader.getState() || autonSelect.isSkills()))
            {
                double bottomHue = bottomColor.get_hue();
                if (rollers::currentState.bottomSpeed > 0)
                {
                    if (bottomHue > redMin || bottomHue < redMax)
                    {
                        if (currentState != 1)
                        {
                            ballIndex.push_front(2);
                            std::cout << "Red Block!" << std::endl;
                            currentState = 1;
                            middle = true;
                        }
                    }
                    else if (bottomHue > blueMin && bottomHue < blueMax)
                    {
                        if (currentState != 2)
                        {
                            ballIndex.push_front(1);
                            std::cout << "Blue Block!" << std::endl;
                            middle = true;

                            currentState = 2;
                        }
                    }
                    else
                    {
                        currentState = 0;
                    }
                }
                if (bottomDS.get_distance() < 45)
                {
                    std::cout << "bottom" << std::endl;
                    if (middle && !ballIndex.empty())
                    {
                        middle = false;
                        if (currentRollerState == "scoreBottom" || currentRollerState == "clearIntake")
                        {
                            ballIndex.pop_back();
                        }
                        if ((ballIndex.back() == 2) == !redTeam.load())
                        {
                            std::cout << redTeam.load() << " , " << (ballIndex.back() == 1) << " , " << ballIndex.back() << std::endl;
                            if (autonSelect.isSkills())
                            {
                                if (currentRollerState == "scoreMiddle")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "cycleC"});
                                    timeouts.push_back(timeout{cycleCount + 40, "cycleC"});
                                }
                                else if ((currentRollerState == "cycle" || currentRollerState == "intake") && !matchLoader.getState())
                                {
                                    starts.push_back(timeout{cycleCount + 5, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 40, "scoreMiddleC"});
                                }
                            }
                            else
                            {
                                if (currentRollerState == "scoreMiddle")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "cycleC"});

                                    timeouts.push_back(timeout{cycleCount + 40, "cycleC"});
                                }
                                else if (currentRollerState == "cycle" || currentRollerState == "intake")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 40, "scoreMiddleC"});
                                }
                            }
                        }
                        else if (currentRollerState == "scoreMiddle")
                        {
                            ballIndex.pop_back();
                        }
                    }
                }
                if (topDS.get_distance() < 50)
                {
                    if (top && !ballIndex.empty())
                    {

                        top = false;

                        if ((ballIndex.back() == 2) == !redTeam.load())
                        {
                            command++;

                            if (autonSelect.isSkills())
                            {
                                if (matchLoader.getState() && sortML.load())
                                {
                                    rollers::addTemporaryState("reverseTop", 1);
                                    timeouts.push_back(timeout{cycleCount + 5, "reverseTop"});
                                    starts.push_back(timeout{cycleCount + 5, "none"});
                                    timeouts.push_back(timeout{cycleCount + 70, "none"});
                                    starts.push_back(timeout{cycleCount + 70, "clearIntakeC"});
                                    timeouts.push_back(timeout{cycleCount + 170, "clearIntakeC"});
                                }
                                else if (currentRollerState == "scoreTop")
                                {
                                    // starts.push_back(timeout{cycleCount + 1, "cycleCT"});
                                    rollers::addTemporaryState("cycleCT", 1);

                                    timeouts.push_back(timeout{cycleCount + 35, "cycleCT"});
                                    std::cout << "Cycled" << std::endl;
                                }
                                else if ((currentRollerState == "cycle" || currentRollerState == "intake") && !matchLoader.getState())
                                {
                                    rollers::addTemporaryState("scoreTopC", 1);
                                    timeouts.push_back(timeout{cycleCount + 20, "scoreTopC"});
                                }
                            }
                            else
                            {
                                if (currentRollerState == "scoreTop")
                                {
                                    // starts.push_back(timeout{cycleCount + 1, "cycleC"});
                                    rollers::addTemporaryState("cycleCT", 1);

                                    timeouts.push_back(timeout{cycleCount + 35, "cycleCT"});
                                }
                                else if (currentRollerState == "cycle" || currentRollerState == "intake")
                                {
                                    rollers::addTemporaryState("scoreTopC", 1);
                                    timeouts.push_back(timeout{cycleCount + 20, "scoreTopC"});
                                }
                            }
                        }
                        ballIndex.pop_back();
                        std::cout << "Popped!" << std::endl;
                    }
                }
                else
                {
                    top = true;
                }
            }
            pros::delay(10);
        }
    }

}

#endif