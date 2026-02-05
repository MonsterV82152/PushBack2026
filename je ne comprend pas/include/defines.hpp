#include "main.h"


//controller
extern pros::Controller master;

//motors
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;

extern pros::MotorGroup system_mg;

//sensors
extern pros::Imu imu;

extern pros::Distance fds;
extern pros::Distance bds;
extern pros::Distance lds;
extern pros::Distance rds;

extern pros::Rotation vEnc;
extern pros::Rotation hEnc;

extern pros::ADIPotentiometer selector;

//pneumatics
extern pros::ADIDigitalOut pto;
extern pros::ADIDigitalOut lift;
extern pros::ADIDigitalOut matchload;
extern pros::ADIDigitalOut intakeLift;
extern pros::ADIDigitalOut wing;
extern pros::ADIDigitalOut hood;

//odom
extern lemlib::TrackingWheel hTc;
extern lemlib::TrackingWheel vTc;

extern lemlib::OdomSensors odom;

//PIDs
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

extern lemlib::PID lever;

//chassis
extern lemlib::Drivetrain drivetrain;
extern lemlib::Chassis chassis;

extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve turnCurve;

//auton selector
extern int left;
extern int left2;
extern int right;
extern int right2;
extern int solo;
extern int moveOff;

extern int selector;

extern void left();
extern void left2();
extern void right();
extern void right2();
extern void solo();
extern void moveOff();

//lever
extern int oneBall;
extern int twoBall;
extern int threeBall;
extern int fourBall;
extern int fiveBall;
extern int sixBall;

//movements