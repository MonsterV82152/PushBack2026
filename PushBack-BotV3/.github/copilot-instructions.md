# VEX V5 Robotics - PushBack Bot v3 Copilot Instructions

## Project Overview
VEX V5 competition robot using **PROS 4.2.1** kernel with a custom motion library (**limelib**). The robot uses a differential drivetrain with PTO (Power Take-Off) mechanisms for intake/hook systems.

## Architecture

### Key Components
- **Robot** ([movements.hpp](include/movements.hpp)) - Main controller orchestrating subsystems, motion, and teleop
- **Helper** - Hardware abstraction bundling motors, pistons, and sensors
- **limelib** ([include/limelib/](include/limelib/)) - Custom odometry/motion library with `Locator`, `Chassis`, `PID`, and tracking wheels
- **AutonSelector** ([autonomous_selector.hpp](include/autonomous_selector.hpp)) - Touchscreen-based routine picker for pre-match

### File Organization
```
include/
  defines.hpp      # All hardware ports, motor groups, sensors, pistons
  globals.hpp      # Runtime config (team color, thresholds), button aliases
  movements.hpp    # Robot class, PTOState, ScoringAction enum
src/includes/
  autonomous_paths.cpp  # Named autonomous routines (left, right, soloAWP, skills)
  movements.cpp         # Robot methods implementation
```

## Hardware Configuration

All ports defined in [defines.hpp](include/defines.hpp):
- **Drivetrain**: Blue cartridge motors - Left: {-15, -14}, Right: {19, 20}
- **Sensors**: IMU(12), Rotation(8), Distance sensors for localization
- **Pistons**: ADI ports A-H for intake/hook PTOs, lift, descore, match loader

Port changes require updating `defines.hpp` inline variables only.

## Motion Control Patterns

### Autonomous Movement
```cpp
robot.setPose(-48, 16, 90);                    // Set initial position (x, y, heading°)
robot.moveToPoint(-24, 24, 1000);              // Move to point with timeout ms
robot.moveToPoint(-48, 48, 1000, {false});     // Reversed movement
robot.turnToPoint(-70, 48, 700);               // Turn to face point
robot.turnToHeading(90, 800);                  // Turn to absolute heading
robot.waitUntilDone();                         // Block until motion completes
```

### PTO State Management
```cpp
robot.moveState({ON, LEAVE, -100, LEAVE});  // {intakePTO, hookPTO, intakeSpeed, hookSpeed}
// ON=engaged, OFF=disengaged, LEAVE=unchanged
```

### Mechanism Actions
```cpp
robot.intake();           // Start intake
robot.lift();             // Raise lift
robot.score(1800);        // Score at rotation position
robot.matchLoad(true);    // Engage match loader
```

## Build & Deploy (PROS CLI)

```bash
pros make              # Compile project
pros mu                # Make and upload to robot
pros terminal          # Open serial terminal for debugging
pros make clean        # Clean build artifacts
```

Default make target is `quick` (incremental build).

## Autonomous Routines

Routines registered in `initialize()` in [main.cpp](src/main.cpp):
- Named by alliance + position: `left`, `right`, `left2`, `right2`, `soloAWP`, `skills`
- Each is a void function in [autonomous_paths.cpp](src/includes/autonomous_paths.cpp)
- Field coordinates: 144"×144" field, origin at center, positive X right, positive Y up

## Coding Conventions

1. **Hardware**: All motor/sensor objects are `inline` in `defines.hpp`
2. **Button aliases**: Use `buttons::R1`, `buttons::A` instead of `pros::E_CONTROLLER_DIGITAL_*`
3. **Brake modes**: Use `brake`, `coast`, `hold` constants from `globals.hpp`
4. **Piston wrapper**: Use `Piston` class with `.setState(bool)` and `.toggle()` for state tracking
5. **Delays**: Use `pros::delay(ms)` for timing in autonomous, 20ms loop in opcontrol

## PID Tuning

PIDs are motor-count dependent (4/6/8 motors on drivetrain via PTOs):
```cpp
linearPID4(5, 0, 59.75)    // 4 motors
linearPID6(5.75, 0, 43.5)  // 6 motors (1 PTO engaged)
linearPID8(8, 0, 38.5)     // 8 motors (both PTOs engaged)
```

## Debugging

- `robot.debug()` - Print diagnostics (commented in opcontrol loop)
- Distance sensor position correction: `correct_position(sensor, &locator, bool)`
- LCD/Brain screen used by `AutonSelector` for routine selection
