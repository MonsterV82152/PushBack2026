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


#endif