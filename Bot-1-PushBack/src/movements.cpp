
#ifndef MOVEMENTS_CPP
#define MOVEMENTS_CPP

// Main header guard for movements.cpp


#include "globals.hpp" // Global variables and hardware definitions
#include "AutonSelector.hpp" // Autonomous routine selector

// Namespace for roller mechanism control
namespace rollers
{
    // Structure for a roller state
    struct rollerState
    {
        std::string name; // State name
        double bottomSpeed; // Speed for bottom roller
        double middleSpeed; // Speed for middle roller
        double topSpeed; // Speed for top roller
        double bucketSpeed; // Speed for bucket
    };
    // Structure for a temporary roller state
    struct temporaryRollerState
    {
        std::string name; // State name
        double bottomSpeed;
        double middleSpeed;
        double topSpeed;
        double bucketSpeed;
        int importance; // 0 for high importance, 10 for low importance
    };
    // Current and default roller states
    inline rollerState state = {"none", 0, 0, 0, 0};
    inline rollerState currentState = {"none", 0, 0, 0, 0};

    // List of all possible roller states
    inline std::vector<rollerState> rollerStates = {
        {"intake", 80, 80, 80, 0},
        {"intakeC", 127, 127, 127, 0},
        {"outtake", -127, -127, -127, 0},
        {"scoreBottom", -127, -127, -127, 127},
        {"scorePark", -127, -127, -127, 127},

        {"scoreMiddle", 30, -30, 127, 127},
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
    // List of temporary roller states and current temporary state
    inline std::vector<temporaryRollerState> temporaryRollerStates;
    inline temporaryRollerState currentTemporaryState = {"none", 0, 0, 0, 0, 10};
    // Helper to convert rollerState to temporaryRollerState
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

    // Set roller state by name
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
    // Set roller state by rollerState object
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
    // Run the lowest importance temporary state
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
        // Update the current temporary state and roller state to the lowest importance temporary state
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
    // Add a temporary roller state by name and importance
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

    // Add a temporary roller state by rollerState object and importance
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
    // Remove a temporary roller state by name
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
    // Check if a state exists in current or temporary states
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
    // Find the lowest importance temporary state
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

    // Remove all temporary roller states
    inline void removeAllTemporaryState()
    {
        temporaryRollerStates.clear();
        _runLowestTemporaryState();
    }

    // Set roller to intake state
    inline void intake()
    {
        setState("intake");
    }
    // Set roller to outtake state
    inline void outtake()
    {
        setState("outtake");
    }
    // Set roller to stop state
    inline void stop()
    {
        setState("none");
    }
    // Set roller to score middle state
    inline void scoreMiddle()
    {
        setState("scoreMiddle");
    }
    // Set roller to score top state
    inline void scoreTop()
    {
        setState("scoreTop");
    }
    // Set roller to score bottom state
    inline void scoreBottom()
    {
        setState("scoreBottom");
    }
}

// Namespace for color sorting logic
namespace colourSort
{
    // Structure for timeouts used in color sorting
    struct timeout
    {
        int cycleCount; // When to trigger
        std::string name; // State name to trigger
    };
    // Team and sorting flags
    inline std::atomic<bool> redTeam(true);
    inline std::atomic<bool> on(true);
    inline std::atomic<bool> sortML(true);
    // Deque for ball color index (2=red, 1=blue)
    inline std::deque<int> ballIndex = {};

    // Command counter for color sort
    inline int command = 0;
    // Set command to stop after count
    inline void stopAfter(int count)
    {
        command = count;
    }
    // Main color sorting thread entry point
    inline void start(void *params)
    {
        // Color thresholds and state variables
        double redMax = 40;
        double redMin = 350;
        double blueMax = 240;
        double blueMin = 170;
        bool middle = true;
        bool top = true;
        // Vectors for timeouts and starts
        std::vector<timeout> timeouts = {};
        std::vector<timeout> starts = {};
        short currentState = 0;
        int cycleCount = 0;

        // Set color sensor LED brightness
        bottomColor.set_led_pwm(100);
        // Main color sorting loop
        while (true)
        {
            // Parking routine triggered by RIGHT button
            if (master.get_digital_new_press(buttons::RIGHT)) {
                rollers::setState("scorePark"); // Set rollers for parking
                pros::delay(500); // Wait for mechanism
                while (bottomDS.get_distance() >= 80) {
                    pros::delay(10); // Wait until ball leaves
                }
                while (bottomDS.get_distance() <= 80) {
                    pros::delay(10); // Wait until ball enters
                }
                pros::delay(90); // Final delay

                rollers::setState("none"); // Stop rollers
                park.setState(true); // Set park state
            }
            // Get current roller state and increment cycle counter
            std::string currentRollerState = rollers::findLowestState(2).name;
            cycleCount++;
            // Handle timeouts for removing temporary states
            for (int i = 0; i < timeouts.size(); i++)
            {
                if (timeouts[i].cycleCount == cycleCount)
                {
                    rollers::removeTemporaryState(timeouts[i].name);
                    timeouts.erase(timeouts.begin() + i);
                }
            }
            // Handle starts for adding temporary states
            for (int i = 0; i < starts.size(); i++)
            {
                if (starts[i].cycleCount == cycleCount)
                {
                    rollers::addTemporaryState(starts[i].name, 1);
                    starts.erase(starts.begin() + i);
                }
            }
            // Main color sensing and sorting logic
            // Top sensor triggers colour sensor to read next block - fixes the ghost block issue
            if (rollers::currentState.name != "none" && on.load() && (!autonSelect.isSkills() && !matchLoader.getState() || autonSelect.isSkills()))
            {
                // Get hue value from color sensor
                double bottomHue = bottomColor.get_hue();
                // Only sort if rollers are moving forward
                if (rollers::currentState.bottomSpeed > 0)
                {
                    // Detect red block
                    if (bottomHue > redMin || bottomHue < redMax)
                    {
                        // If not already in red state, push red
                        if (currentState != 1)
                        {
                            ballIndex.push_front(2); // 2 = red
                            std::cout << "Red Block!" << std::endl;
                            currentState = 1;
                            middle = true;
                        }
                    }
                    // Detect blue block
                    else if (bottomHue > blueMin && bottomHue < blueMax)
                    {
                        // If not already in blue state, push blue
                        if (currentState != 2)
                        {
                            ballIndex.push_front(1); // 1 = blue
                            std::cout << "Blue Block!" << std::endl;
                            middle = true;
                            currentState = 2; 
                        }
                    }
                    else
                    {
                        // No color detected
                        currentState = 0;
                    }
                }
                // Middle sensor triggers block handling
                if (middleDS.get_distance() < 45)
                {
                    std::cout << "bottom" << std::endl;
                    if (middle && !ballIndex.empty())
                    {
                        middle = false;
                        // Remove ball if scoring bottom or clearing intake
                        if (currentRollerState == "scoreBottom" || currentRollerState == "clearIntake")
                        {
                            ballIndex.pop_back();
                        }
                        // Check if ball matches team color
                        if ((ballIndex.back() == 2) == !redTeam.load())
                        {
                            std::cout << redTeam.load() << " , " << (ballIndex.back() == 1) << " , " << ballIndex.back() << std::endl;
                            // Autonomous skills routine
                            if (autonSelect.isSkills())
                            {
                                // Score middle block
                                if (currentRollerState == "scoreMiddle")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "cycleC"});
                                    timeouts.push_back(timeout{cycleCount + 30, "cycleC"});
                                }
                                // Cycle or intake block
                                else if ((currentRollerState == "cycle" || currentRollerState == "intake") && !matchLoader.getState())
                                {
                                    starts.push_back(timeout{cycleCount + 5, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    // Remove ball if not empty
                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 30, "scoreMiddleC"});
                                }
                            }
                            else
                            {
                                if (currentRollerState == "scoreMiddle")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "cycleC"});

                                    timeouts.push_back(timeout{cycleCount + 30, "cycleC"});
                                }
                                else if (currentRollerState == "cycle" || currentRollerState == "intake")
                                {
                                    starts.push_back(timeout{cycleCount + 5, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 30, "scoreMiddleC"});
                                }
                            }
                        }
                        else if (currentRollerState == "scoreMiddle")
                        {
                            // Remove ball if scoring middle
                            ballIndex.pop_back();
                        }
                    }
                }
                // Top sensor triggers block handling
                if (topDS.get_distance() < 50)
                {
                    if (top && !ballIndex.empty())
                    {

                        top = false;

                        // Check if ball matches team color
                        if ((ballIndex.back() == 2) == !redTeam.load())
                        {
                            // Increment command counter
                            command++;

                            // Autonomous skills routine
                            if (autonSelect.isSkills())
                            {
                                // If match loader is active and sortML enabled
                                if (matchLoader.getState() && sortML.load())
                                {
                                    rollers::addTemporaryState("reverseTop", 1);
                                    timeouts.push_back(timeout{cycleCount + 5, "reverseTop"});
                                    starts.push_back(timeout{cycleCount + 5, "none"});
                                    timeouts.push_back(timeout{cycleCount + 70, "none"});
                                    starts.push_back(timeout{cycleCount + 70, "clearIntakeC"});
                                    timeouts.push_back(timeout{cycleCount + 170, "clearIntakeC"});
                                }
                                // Score top block
                                else if (currentRollerState == "scoreTop")
                                {
                                    // starts.push_back(timeout{cycleCount + 1, "cycleCT"});
                                    rollers::addTemporaryState("cycleCT", 1);

                                    timeouts.push_back(timeout{cycleCount + 35, "cycleCT"});
                                    std::cout << "Cycled" << std::endl;
                                }
                                // Cycle or intake block
                                else if ((currentRollerState == "cycle" || currentRollerState == "intake") && !matchLoader.getState())
                                {
                                    rollers::addTemporaryState("scoreTopC", 1);
                                    timeouts.push_back(timeout{cycleCount + 20, "scoreTopC"});
                                }
                            }
                            else
                            {
                                // Non-skills autonomous routine
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
                        // Remove ball from back after handling
                        ballIndex.pop_back();
                        std::cout << "Popped!" << std::endl;
                    }
                }
                else
                {
                    // Reset top flag if sensor not triggered
                    top = true;
                }
            }
            // Delay for 10ms to prevent CPU overload
            pros::delay(10);
        }
    }

}

#endif // End of header guard