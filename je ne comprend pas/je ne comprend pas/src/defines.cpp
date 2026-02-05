#include "main.h"

//controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//motors
pros::MotorGroup left_mg({1, -2, 3});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({-4, 5, -6});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6

pros::MotorGroup system_mg({7, -8}); //system motors

//sensors
pros::Imu imu(9);

pros::Distance fds(10);
pros::Distance bds(11);
pros::Distance lds(12);
pros::Distance rds(13);

pros::Rotation vEnc(14);
pros::Rotation hEnc(15);

pros::ADIPotentiometer selector('G');

//pneumatics
pros::ADIDigitalOut pto('A');
pros::ADIDigitalOut lift('B');
pros::ADIDigitalOut matchload('C');
pros::ADIDigitalOut intakeLift('D');
pros::ADIDigitalOut wing('E');
pros::ADIDigitalOut hood('F');

//odom
lemlib::TrackingWheel hTc(&hEnc, lemlib::Omniwheel::NEW_2, 0);
lemlib::TrackingWheel vTc(&vEnc, lemlib::Omniwheel::NEW_2, 0);

lemlib::OdomSensors odom(&hTc,nullptr, &vTc, nullptr, &imu);

//PIDs
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::PID lever(5, // kP
                  0.01, // kI
                  20, // kD
                  5, // integral anti windup range
                  false // don't reset integral when sign of error flips
);

//chassis
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 10.85, lemlib::Omniwheel::NEW_275, 600, 2);
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, odom, &throttleCurve, &turnCurve);

lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                       3, // minimum output where drivetrain will move out of 127
                       1 // expo curve gain
);

lemlib::ExpoDriveCurve turnCurve(3, // joystick deadband out of 127
                       3, // minimum output where drivetrain will move out of 127
                       1 // expo curve gain
);

//lever
int oneBall = 0;
int twoBall = 0;
int threeBall = 0;
int fourBall = 0;
int fiveBall = 0;
int sixBall = 0;


