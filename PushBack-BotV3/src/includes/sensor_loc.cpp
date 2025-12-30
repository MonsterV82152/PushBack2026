#include "sensor_loc.hpp"
#include <string>

void correct_position(limelib::MCLDistance sensor, limelib::Locator *locator, bool x, bool forced, double correct_rate)
{
    double wall_dist = 71.5;
    limelib::Pose2D currentPos = locator->getPose(true);
    // std::cout << "Current Position: X: " << currentPos.x << " Y: " << currentPos.y << " Theta (rad): " << currentPos.theta << "\n";
    double distanceValue = sensor.sensor.get_distance();

    if (distanceValue == 9999)
    {
        return;
    }
    else
    {
        distanceValue = distanceValue * 0.0393701;
        std::cout << "Distance Sensor Reading: " << distanceValue << " inches\n";
        // pros::lcd::print(3, "Distance: %f", distanceValue);
    }
    /*double offset_x = offset.x * cos(theta_rad) - offset.y * sin(theta_rad);
        double offset_y = offset.x * sin(theta_rad) + offset.y * cos(theta_rad);*/
    double offset_y = -sensor.pose.x * sin(currentPos.theta) + sensor.pose.y * cos(currentPos.theta);
    double offset_x = sensor.pose.x * cos(currentPos.theta) + sensor.pose.y * sin(currentPos.theta);
    double x_value = distanceValue * sin(currentPos.theta + sensor.pose.theta * M_PI / 180) + offset_x;
    double y_value = distanceValue * cos(currentPos.theta + sensor.pose.theta * M_PI / 180) + offset_y;
    if (x)
    {
        x_value = wall_dist * x_value / abs(x_value) - x_value;
        std::cout << "Calculated X Position: " << x_value << " deg " << currentPos.theta << "\n";
        if (abs(x_value - currentPos.x) < correct_rate || forced)
        {
            std::cout << "Corrected X Position from " << currentPos.x << " to " << x_value << " deg " << currentPos.theta << "\n";
            locator->setPose(x_value, currentPos.y, currentPos.theta, true);
        }
    }
    else
    {
        y_value = wall_dist * y_value / abs(y_value) - y_value;
        std::cout << "Calculated Y Position: " << y_value << " deg " << currentPos.theta << "\n";
        if (abs(y_value - currentPos.y) < correct_rate || forced)
        {
            std::cout << "Corrected Y Position from " << currentPos.y << " to " << y_value << " deg " << currentPos.theta << "\n";
            locator->setPose(currentPos.x, y_value, currentPos.theta, true);
        }
    }
}