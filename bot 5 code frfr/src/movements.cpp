#include "includes.hpp"

void moveDT(int leftY, int rightX) {
  // Turn priority: reduce forward when turning
  int forward = leftY;
  int turn = rightX;
  
  // Reduce forward movement based on turn magnitude
  if (turn != 0) {
    forward = forward * (0.8 - abs(turn) / 127.0);
  }
  
  int left = forward + turn;
  int right = forward - turn;
  
  leftDT.move(left);
  rightDT.move(right);
}

void score() {
    if (scoring) return;  // Prevent overlapping calls
    
    scoring = true;
    
    if (pot.get_value() < SCORING_DOWN_ANGLE) {
        hood.setState(true);
        lever.setState(true);
        pto.setState(false);
        
        // Up phase with timeout
        uint32_t startTime = pros::millis();
        while(pot.get_value() < SCORING_UP_ANGLE) {
            if (pros::millis() - startTime > SCORING_TIMEOUT) {
                break;  // Timeout - exit
            }
            motors.move(127);
            pros::delay(10);
        }
        
        // Down phase with timeout
        startTime = pros::millis();
        while(pot.get_value() > SCORING_DOWN_ANGLE) {
            if (pros::millis() - startTime > SCORING_TIMEOUT) {
                break;  // Timeout - exit
            }
            lever.setState(false);
            motors.move(-127);
            pros::delay(10);
        }
    }
    pto.setState(true);
    motors.move(0);
    
    scoring = false;
}

void intake(int speed) {
    if(!scoring) {
        if(!pto.getState()) {
            pto.setState(true);
        }
        motors.move(speed);
    }
}

