#include "includes.hpp"

using namespace pros;

void colourSort(void *params)
{
    double hue;
    double redMax = 40;
    double redMin = 350;
    double blueMax = 240;
    double blueMin = 170;
    std::vector<rollerState> timeouts = {};
    while (true)
    {
        hue = middleCS.get_hue();
        if (robot.getRollerState() == INTAKE && frontDS.get_distance() < 60)
        {
            pros::delay(400);
            robot.setState(INTAKE2);
        }
        else if (robot.getRollerState() == INTAKE2 && back.get_actual_velocity() < 100)
        {
            robot.setState(INTAKE3);
        }
        if (colourSortOn)
        {
            if ((!isRedTeam && (hue > redMin || hue < redMax)) || (isRedTeam && hue > blueMin && hue < blueMax))
            {
                robot.addTempState(COLOURSORT, 10);
                pros::delay(400);
                robot.removeTempState(COLOURSORT);
                pros::delay(200);
            }
        }
        if (master.get_digital_new_press(buttons::LEFT))
        {
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
            robot.stop();
            park.setState(true);
        }

        pros::delay(5);
    }
}

void on_center_button() {}

void initialize()
{
    autonSelect.setAutons(std::vector<autonomousRoute>{
        autonomousRoute{"red", "Left", "Position: Left", left},
        autonomousRoute{"red", "Right", "Position: Right", right},
        autonomousRoute{"red", "Left2", "Position: Left", left2},
        autonomousRoute{"red", "Right2", "Position: Right", right2},
        autonomousRoute{"blue", "Left", "Position: Left", left},
        autonomousRoute{"blue", "Right", "Position: Right", right},
        autonomousRoute{"blue", "Left2", "Position: Left", left2},
        autonomousRoute{"blue", "Right2", "Position: Right", right2}});
    autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills});

    pros::delay(500);
    autonSelect.start();
    middleCS.set_integration_time(5);

    pros::Task colourSortTask(colourSort);

    chassis.calibrate();
    master.clear();
    chassis.setPose(0, 0, 0);
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{
    middleCS.set_led_pwm(100);

    isRedTeam = autonSelect.isRedTeam();
    autonSelect.runAuton();
    // test();
}

void opcontrol()
{
    middleCS.set_led_pwm(100);
    std::vector<int> teamColorQueue = {1, 2, 0};
    short teamColorIndex = 0;
    isRedTeam = autonSelect.isRedTeam();
    robot.mapButtons({L1, buttons::R2, true});
    robot.mapButtons({L2, buttons::L1, true});
    robot.mapButtons({L3, buttons::L2, true});
    robot.mapButtons({DESCORE, buttons::DOWN, true, false, buttons::A, 2});
    // robot.mapButtons({BACKL2, buttons::L1, true});
    // robot.mapButtons({BACKL3, buttons::L2, true});
    while (true)
    {
        chassis.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), false, 0.54);
        robot.driverControl();
        if (master.get_digital_new_press(buttons::Y))
        {
            teamColorIndex++;
            teamColorIndex %= 3;
            if (teamColorIndex == 0)
            {
                colourSortOn = false;
                master.print(1, 1, "Off ");
            }
            else if (teamColorIndex == 1)
            {
                isRedTeam = true;
                colourSortOn = true;
                master.print(1, 1, "Red ");
            }
            else if (teamColorIndex == 2)
            {
                isRedTeam = false;
                colourSortOn = true;
                master.print(1, 1, "Blue");
            }
        }
        if (master.get_digital(buttons::B))
        {
            robot.matchLoad(true);
        }
        else
        {
            robot.matchLoad(false);
        }
        if (master.get_digital_new_press(buttons::X))
        {
            park.toggle();
        }
        if (master.get_digital_new_press(buttons::R1))
        {
            robot.toggleIntake();
        }
        if (master.get_digital_new_press(buttons::UP))
        {
            park.toggle();
        }

        // if (master.get_digital_new_press(buttons::R1)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(L1);
        //     else roller.setState(INTAKE);
        // }
        // if (master.get_digital_new_press(buttons::L2)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(BACKL3);
        //     else roller.setState(L3);
        // }
        // if (master.get_digital_new_press(buttons::L1)) {
        //     if (master.get_digital(buttons::R2)) roller.setState(BACKL2);
        //     else roller.setState(L2);
        // }
        // if (master.get_digital_new_press(buttons::A)) {
        //     roller.setState(STOP);
        // }

        pros::delay(20);
    }
}