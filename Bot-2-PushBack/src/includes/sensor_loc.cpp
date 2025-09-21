#include "sensor_loc.hpp"

void correct_position(dist_sensor sensor, lemlib::Chassis *chassis, bool x, bool forced, double correct_rate) {
    double wall_dist = 71.5;
    lemlib::Pose currentPos = chassis->getPose(true);
    double distanceValue = sensor.sensor->get_distance();

    if (distanceValue == 9999) {
        return;
    } else {
        distanceValue = distanceValue * 0.0393701;
        pros::lcd::print(3, "Distance: %f", distanceValue);

    }
    /*double offset_x = offset.x * cos(theta_rad) - offset.y * sin(theta_rad);
        double offset_y = offset.x * sin(theta_rad) + offset.y * cos(theta_rad);*/
    double offset_y = -sensor.offset.x * sin(currentPos.theta) + sensor.offset.y * cos(currentPos.theta);
    double offset_x = sensor.offset.x * cos(currentPos.theta) + sensor.offset.y * sin(currentPos.theta);
    pros::lcd::print(4, "offset_y: %f", offset_y);
    pros::lcd::print(5, "offset_x: %f", offset_x);
    double x_value = distanceValue * sin(currentPos.theta + sensor.offset.theta*M_PI/180) + offset_x;
    double y_value = distanceValue * cos(currentPos.theta + sensor.offset.theta*M_PI/180) + offset_y;
    pros::lcd::print(6, "x_value: %f", x_value);
    pros::lcd::print(7, "y_value: %f", y_value);
    if (x) {
        x_value = wall_dist*x_value/abs(x_value) - x_value;
        if (abs(x_value-currentPos.x) < correct_rate || forced) {
            chassis->setPose(x_value, currentPos.y, currentPos.theta, true);
        }
    } else {
        y_value = wall_dist*y_value/abs(y_value) - y_value;
        if (abs(y_value-currentPos.y) < correct_rate || forced) {
            chassis->setPose(currentPos.x, y_value, currentPos.theta, true);
        }
    }





}