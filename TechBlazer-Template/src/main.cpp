#include "globals.hpp"
#include "AutonSelector.hpp"
#include "Autonomous_Paths.hpp"

AutonSelector autonSelector;

void on_center_button() {}	

void initialize() {
    pros::screen::erase();
    pros::delay(1000);
    autonSelector.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "Red Auton 1", "1st Red Auton", redAuton1},
        autonomousRoute{"red", "Red Auton 2", "2nd Red Auton", redAuton2},
        autonomousRoute{"blue", "Blue Auton 1", "1st Blue Auton", blueAuton1},
        autonomousRoute{"blue", "Blue Auton 2", "2nd Blue Auton", blueAuton2}
    });
    autonSelector.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", exampleAuton});
    autonSelector.start();
    // Code to run when the code is started - initialization code
    
}

void disabled() {}
   
void competition_initialize() {}

void autonomous() {
    // Code to run during the autonomous period
    autonSelector.runAuton();
}
 
void opcontrol() {
	while (true) {

        // double rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // double leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        // leftDT.move_velocity(leftY+rightX);
        // rightDT.move_velocity(leftY-rightX);
        
		pros::delay(20);
		
	}
}