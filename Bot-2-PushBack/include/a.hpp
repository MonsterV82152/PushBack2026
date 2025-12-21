#include "includes.hpp"

void colourSort(void *params)
{
    bool red = false;
    bool blue = false;
    int currentTime = 0;
    std::vector<timeouts> timeouts = {};
    while (true)
    {
        currentTime = pros::millis(); // Current time in milliseconds
        for (int i = 0; i < timeouts.size(); i++)
        {
            if (timeouts[i].timestamp <= currentTime) // Check if timeout has expired
            {
                timeouts[i].function();               // Execute scheduled function
                timeouts.erase(timeouts.begin() + i); // Remove executed timeout
                i--;                                  // Adjust index after removal
            }
        }
        hue = middleCS.get_hue();                // Get current hue from color sensor
        int distance = middleCS.get_proximity(); // Get proximity distance
        red = (hue > redMin || hue < redMax) && distance < 100;
        blue = (hue > blueMin && hue < blueMax) && distance < 100;
        // Detect if block is stalling motors
        if (
            !intakeTask && robot.getRollerState() == INTAKE &&
            frontDS.get_distance() < 30 && front.get_actual_velocity() < 30)
        {
            intakeTask = true;
            // Schedule transition to INTAKE2
            timeouts.push_back({currentTime + 300, []()
                                { robot.setState(INTAKE2); intakeTask = false; }});
        }
        // Transition to INTAKE3 when velocity stabilizes
        else if (robot.getRollerState() == INTAKE2 && middle.get_actual_velocity() < 20)
        {
            robot.setState(INTAKE3); // Set roller state to INTAKE3
        }
        if (colourSortOn) // Color sorting logic
        {
            if ((!isRedTeam && red) || (isRedTeam && blue)) // Opponent color detected
            {
                colourSorting = true;               // Flag that color sorting is active
                robot.addTempState(COLOURSORT, 10); // Eject opponent ring
                pros::delay(300);
                robot.removeTempState(COLOURSORT); // Resume normal operation
                colourSorting = false;             // Clear sorting flag
            }
        }
        if (master.get_digital_new_press(buttons::LEFT))
        {
            chassis.setBrakeMode(brake);
            brakeChassis = true;
            robot.setState(PARK);
            park.setState(false);
            while (intakeDS.get_distance() > 50)
            {
                pros::delay(10);
            }
            robot.setState(PARK2);
            while (intakeDS.get_distance() < 50)
            {
                pros::delay(10);
            }
            pros::delay(400);
            robot.stop();
            park.setState(true);
        }
        if (master.get_digital_new_press(buttons::L1))
        {
            master.rumble(".");

            robot.addTempState(L2HELPER, 1);
            if (autonSelect.isSkills())
            {
                timeouts.push_back({currentTime + 200, []()
                                    { if (robot.getRollerState() == L2HELPER) robot.removeTempState(L2HELPER); robot.addTempState(L2SKILLS, 1); }});
            }
            else
            {
                timeouts.push_back({currentTime + 200, []()
                                    { if (robot.getRollerState() == L2HELPER) robot.removeTempState(L2HELPER); robot.addTempState(L2, 1); }});
            }
        }
        if (master.get_digital_new_press(buttons::L2))
        {
            // master.rumble("..");
            // robot.addTempState(L3HELPER, 1);
            // timeouts.push_back({currentTime + 200, []()
            //                     { if (robot.getRollerState() == L3HELPER) robot.removeTempState(L3HELPER); robot.addTempState(L3, 1); }});
            robot.addTempState(L3, 1);
        }
        if (master.get_digital_new_press(buttons::RIGHT))
        {
            robot.addTempState(BACKL3HELPER, 1);
            timeouts.push_back({currentTime + 200, []()
                                { if (robot.getRollerState() == BACKL3HELPER) robot.removeTempState(BACKL3HELPER); robot.addTempState(BACKL3, 1); }});
        }
        if (!master.get_digital(buttons::RIGHT) && !master.get_digital_new_press(buttons::RIGHT))
        {

            if (robot.getRollerState() == BACKL3)
            {
                robot.removeTempState(BACKL3);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == BACKL3HELPER)
            {
                robot.removeTempState(BACKL3HELPER);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }
        if (!master.get_digital(buttons::L2) && !master.get_digital_new_press(buttons::L2))
        {

            if (robot.getRollerState() == L3)
            {
                robot.removeTempState(L3);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L3HELPER)
            {
                robot.removeTempState(L3HELPER);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }
        if (!master.get_digital(buttons::L1) && !master.get_digital_new_press(buttons::L1))
        {

            if (robot.getRollerState() == L2)
            {
                robot.removeTempState(L2);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L2HELPER)
            {
                robot.removeTempState(L2HELPER);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
            else if (robot.getRollerState() == L2SKILLS)
            {
                robot.removeTempState(L2SKILLS);
                if (robot.getDefaultState() == INTAKE2 || robot.getDefaultState() == INTAKE3)
                {
                    robot.setState(INTAKE);
                }
            }
        }

        pros::delay(5);
    }
}