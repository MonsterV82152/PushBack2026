#ifndef MOVEMENTS_CPP
#define MOVEMENTS_CPP
#include "globals.hpp"

// Roller control functions
namespace rollers {
    inline std::string state = "none"; // Current roller state
    // Sets the roller to intake state
    inline void intake() {
        bottom.move(127);
        middle.move(127);
        top.move(127);
        state = "intake";
    }
    // Sets the roller to outtake state
    inline void outtake() {
        bottom.move(-127);
        middle.move(-127);
        top.move(-127);
        bucket.move(-127);
        state = "outtake";
    }
    // Stops all roller motors
    inline void stop() {
        bottom.move(0);
        middle.move(0);
        top.move(0);
        bucket.move(0);
        state = "none";
    }
    // Sets the roller to scoreMiddle state
    inline void scoreMiddle() {
        bottom.move(127);
        bucket.move(127);
        middle.move(-127);
        top.move(0);
        state = "scoreMiddle";
    }
    // Sets the roller to scoreTop state
    inline void scoreTop() {
        bottom.move(127);
        middle.move(127);
        top.move(-127);
        bucket.move(127);
        state = "scoreTop";
    }
}

namespace colourSort {
    inline std::string state = "none";
    inline std::string colour = "none";
    inline void start(void *param) {
        // This function is a placeholder for the colour sorting logic
        // You can implement the logic to sort colours based on your requirements
        // For now, it will just print a message to the console
        std::cout << "Colour sorting logic goes here." << std::endl;
    }
    inline void setState(std::string state) {
        
    }
}




namespace colourSort
{
    /**
     * @brief Structure to manage timeouts for colour sorting
     * @param cycleCount The cycle count when the timeout should occur
     * @param name The name associated with the timeout
     */
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

        topColour.set_led_pwm(100);
        while (true)
        {
            if (master.get_digital_new_press(buttons::RIGHT))
            {
                rollers::setState("scorePark");
                pros::delay(500);
                while (bottomDS.get_distance() >= 80)
                {
                    pros::delay(10);
                }
                while (bottomDS.get_distance() <= 80)
                {
                    pros::delay(10);
                }
                pros::delay(90);

                rollers::setState("none");
                park.setState(true);
            }
            std::string currentRollerState = rollers::findLowestState(2).name;
            
            // Manage timeouts and starts
            cycleCount++; // Increment cycle count
            for (int i = 0; i < timeouts.size(); i++) // Check for timeouts
            {
                if (timeouts[i].cycleCount == cycleCount) // Timeout reached
                {
                    rollers::removeTemporaryState(timeouts[i].name); // Remove temporary state
                    timeouts.erase(timeouts.begin() + i); // Erase timeout
                }
            }
            for (int i = 0; i < starts.size(); i++) // Check for starts
            {
                if (starts[i].cycleCount == cycleCount) // Start reached
                {
                    rollers::addTemporaryState(starts[i].name, 1); // Add temporary state
                    starts.erase(starts.begin() + i); // Erase start
                }
            }
            // Top sensor triggers colour sensor to read next block - fixes the ghost block issue
            if (rollers::currentState.name != "none" && on.load() && (!autonSelect.isSkills() && !matchLoader.getState() || autonSelect.isSkills()))
            {
                double topColor = topColour.get_hue(); // Get hue from top colour sensor
                if (topColor > redMin || topColor < redMax) // Detected red colour
                {
                    if (!redTeam.load() && currentState != 1) // If on blue team and state has changed
                    {
                        currentState = 1; // Update current state
                        if (autonSelect.isSkills()) // Check if in skills mode
                        {
                            if (currentRollerState == "scoreTop") // Check if current roller state is "scoreTop"
                            {
                                rollers::addTemporaryState("cycleC", 1); // Add "cycleC" temporary state
                                timeouts.push_back(timeout{cycleCount + 50, "cycleC"}); // Set timeout for "cycleC"
                            }
                            else if (currentRollerState == "intake") // Check if current roller state is "intake"
                            {
                                rollers::addTemporaryState("scoreTopC", 1); // Add "scoreTopC" temporary state
                                timeouts.push_back(timeout{cycleCount + 50, "scoreTopC"}); // Set timeout for "scoreTopC"
                            }
                        }
                        else
                        {
                            if (currentRollerState == "intake") // Check if current roller state is "intake"
                            {
                                rollers::addTemporaryState("scoreTopC", 1); // Add "scoreTopC" temporary state
                                timeouts.push_back(timeout{cycleCount + 60, "scoreTopC"}); // Set timeout for "scoreTopC"
                            }
                        }
                    }
                }
                else if (topColor > blueMin && topColor < blueMax)
                {
                    if (redTeam.load() && currentState != 2)
                    {
                        currentState = 2;

                        if (autonSelect.isSkills())
                        {
                            if (currentRollerState == "scoreTop")
                            {
                                rollers::addTemporaryState("cycleC", 1);
                                timeouts.push_back(timeout{cycleCount + 60, "cycleC"});
                            }
                            else if (currentRollerState == "intake")
                            {
                                rollers::addTemporaryState("scoreTopC", 1);
                                timeouts.push_back(timeout{cycleCount + 60, "scoreTopC"});
                            }
                        }
                        else
                        {
                            if (currentRollerState == "intake")
                            {
                                rollers::addTemporaryState("scoreTopC", 1);
                                timeouts.push_back(timeout{cycleCount + 60, "scoreTopC"});
                            }
                        }
                    }
                } 
                else {
                    currentState = 0;
                }
            }
            pros::delay(10);
        }
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
            if (master.get_digital_new_press(buttons::RIGHT)) {
                rollers::setState("scorePark");
                pros::delay(500);
                while (bottomDS.get_distance() >= 80) {
                    pros::delay(10);
                }
                while (bottomDS.get_distance() <= 80) {
                    pros::delay(10);
                }
                pros::delay(90);

                rollers::setState("none");
                park.setState(true);
            }
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
                double bottomHue = bottomColor.get_hue(); // Get hue from bottom colour sensor
                if (rollers::currentState.bottomSpeed > 0) // Only read when intaking
                {
                    if (bottomHue > redMin || bottomHue < redMax) // Detected red colour
                    {
                        if (currentState != 1) // If state has changed
                        {
                            ballIndex.push_front(2); // Add red ball to index
                            std::cout << "Red Block!" << std::endl; // Print message
                            currentState = 1; // Update current state
                        }
                    }
                    else if (bottomHue > blueMin && bottomHue < blueMax) // Detected blue colour
                    {
                        if (currentState != 2) // If state has changed
                        {
                            ballIndex.push_front(1); // Add blue ball to index
                            std::cout << "Blue Block!" << std::endl; // Print message
                            currentState = 2; // Update current state
                        }
                    }
                    else
                    {
                        currentState = 0; // Reset current state
                    }
                }
                if (middleDS.get_distance() < 45)
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
                                    starts.push_back(timeout{cycleCount + 1, "cycleC"});
                                    timeouts.push_back(timeout{cycleCount + 1, "cycleC"});
                                }
                                else if ((currentRollerState == "cycle" || currentRollerState == "intake") && !matchLoader.getState())
                                {
                                    starts.push_back(timeout{cycleCount + 1, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 1, "scoreMiddleC"});
                                }
                            }
                            else
                            {
                                if (currentRollerState == "scoreMiddle")
                                {
                                    starts.push_back(timeout{cycleCount + 1, "cycleC"});

                                    timeouts.push_back(timeout{cycleCount + 1, "cycleC"});
                                }
                                else if (currentRollerState == "cycle" || currentRollerState == "intake")
                                {
                                    starts.push_back(timeout{cycleCount + 1, "scoreMiddleC"});
                                    std::cout << "Popped1!" << std::endl;

                                    if (!ballIndex.empty())
                                        ballIndex.pop_back();

                                    timeouts.push_back(timeout{cycleCount + 1, "scoreMiddleC"});
                                }
                            }
                        }
                        else if (currentRollerState == "scoreMiddle")
                        {
                            ballIndex.pop_back();
                        }
                    }
                } else {
                    middle = true;
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