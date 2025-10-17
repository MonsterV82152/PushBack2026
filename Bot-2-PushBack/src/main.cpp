#include "includes.hpp"

using namespace pros;
bool intakeTask = false;
bool colourSorting = false;

struct timeouts
{
    int timestamp;
    void (*function)();
};

void colourSort(void *params)
{
    double hue;
    double redMax = 40;
    double redMin = 350;
    double blueMax = 240;
    double blueMin = 170;
    bool red = false;
    bool blue = false;
    int currentTime = 0;
    std::vector<timeouts> timeouts = {};
    while (true)
    {
        currentTime = pros::millis();
        for (int i = 0; i < timeouts.size(); i++)
        {
            if (timeouts[i].timestamp <= currentTime)
            {
                timeouts[i].function();
                timeouts.erase(timeouts.begin() + i);
                i--;
            }
        }
        red = (hue > redMin || hue < redMax);
        blue = (hue > blueMin && hue < blueMax);
        hue = middleCS.get_hue();
        if (!intakeTask && robot.getRollerState() == INTAKE && frontDS.get_distance() < 70 && front.get_actual_velocity() < 30)
        {
            intakeTask = true;
            timeouts.push_back({currentTime + 300, []()
                                { robot.setState(INTAKE2); intakeTask = false; }});
            // robot.setState(INTAKE2);
        }
        else if (robot.getRollerState() == INTAKE2 && middle.get_actual_velocity() < 20)
        {
            robot.setState(INTAKE3);
        }
        if (colourSortOn)
        {
            if (!colourSorting && ((!isRedTeam && red) || (isRedTeam && blue)))
            {

                colourSorting = true;
                robot.addTempState(COLOURSORT, 10);
                timeouts.push_back({currentTime + 200, []()
                                    { robot.removeTempState(COLOURSORT); pros::delay(100); colourSorting = false; }});
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
            pros::delay(350);
            robot.stop();
            park.setState(true);
        }
        if (master.get_digital_new_press(buttons::L1))
        {
            master.rumble(".");
            robot.addTempState(L2HELPER, 1);
            timeouts.push_back({currentTime + 100, []()
                                { if (robot.getRollerState() == L2HELPER) robot.removeTempState(L2HELPER); robot.addTempState(L2, 1); }});
        }
        if (master.get_digital_new_press(buttons::L2))
        {
            master.rumble("..");
            robot.addTempState(L3HELPER, 1);
            timeouts.push_back({currentTime + 200, []()
                                { if (robot.getRollerState() == L3HELPER) robot.removeTempState(L3HELPER); robot.addTempState(L3, 1); }});
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
        }

        pros::delay(5);
    }
}

void on_center_button() {}

void initialize()
{
    pros::lcd::initialize();
    pros::Task screen_task([&]()
                           {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // theta
            // delay to save resources
            pros::delay(20);
        } });
    // autonSelect.setAutons(std::vector<autonomousRoute>{
    //     autonomousRoute{"red", "Left", "Position: Left", left},
    //     autonomousRoute{"red", "Right", "Position: Right", right},
    //     autonomousRoute{"red", "Left2", "Position: Left", left2},
    //     autonomousRoute{"red", "Right2", "Position: Right", right2},
    //     autonomousRoute{"blue", "Left", "Position: Left", left},
    //     autonomousRoute{"blue", "Right", "Position: Right", right},
    //     autonomousRoute{"blue", "Left2", "Position: Left", left2},
    //     autonomousRoute{"blue", "Right2", "Position: Right", right2}});
    // autonSelect.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", skills});

    pros::delay(500);
    intake.set_brake_mode(brake);
    // autonSelect.start();
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
    // middleCS.set_led_pwm(100);

    // isRedTeam = autonSelect.isRedTeam();
    // autonSelect.runAuton();

    // test();
    // angular awr
    // double tot = 0;
    // for (double i = 9.99; i <= 180; i += 10) {
    // 	double target = chassis.getPose().theta + i;
    // 	chassis.turnToHeading(target, 1500);
    // 	delay(2000);
    // 	tot += target - chassis.getPose().theta;
    // }

    // delay(500);
    // master.print(1, 0, "%.5f", tot/18*1.5);
    // lcd::print(6, 0, "%.5f", tot);
    // delay(3000);

    // int tar = 90;
    // chassis.turnToHeading(tar, 3000);
    // delay(2500);
    // master.print(1, 0, "%.3f", tar - chassis.getPose().theta);
    // delay(3000);

    // lateral awr
    // double tot = 0;
    // chassis.setPose(0, 0, 0);
    // for (double i = 32; i <= 48; i += 8) {
    // 	double target = chassis.getPose().y + i;
    // 	chassis.moveToPoint(0, target, 4000);
    // 	pros::delay(4050);
    // 	master.print(0, 0, "%f", target-chassis.getPose().y);
    // 	tot += target-chassis.getPose().y;
    // }
    // pros::delay(1000);
    // master.print(1, 0, "tot: %.2f", tot);
    // pros::delay(5000);

    // double tar = 48;
    // int time = 4000;
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, tar, time);
    // pros::delay(time+100);
    // master.print(1, 0, "%.2f", tar-chassis.getPose().y);
    // pros::delay(100);
    // master.print(2, 0, "%.2f", chassis.getPose().y);

    // chassis.turnToHeading(90, 1500);
    // delay(2000);
    // master.print(1, 0, "%.2f", chassis.getPose().theta);
    // delay(1000);

    // int tar = 48;
    // chassis.moveToPoint(0, tar, 2000);
    // delay(2000);
    // master.print(1, 0, "%.2f", tar - chassis.getPose().y);
    // delay(1000);

    // chassis.setPose(0, 0, 0);0000
    // chassis.moveToPoint(0, 48, 1500);
    // chassis.turnToPoint(-48, 48, 1000);
    // chassis.moveToPoint(-48, 48, 1500);
    // chassis.turnToPoint(-48, 0, 1000);
    // chassis.moveToPoint(-48, 0, 1500);
    // chassis.turnToPoint(0, 0, 1000);
    // chassis.moveToPoint(0, 0, 1500);
    soloAWP();
}

void opcontrol()
{
    middleCS.set_led_pwm(100);
    std::vector<int> teamColorQueue = {0, 1, 2};
    short teamColorIndex = 0;
    isRedTeam = autonSelect.isRedTeam();
    robot.mapButtons({L1, buttons::R2, true});
    robot.mapButtons({L3, buttons::L2, true});
    robot.mapButtons({DESCORE, buttons::DOWN, true});
    robot.mapButtons({BACKL2, buttons::A, true});
    robot.mapButtons({BACKINTAKE, buttons::X, false});
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
            robot.matchLoad(true, false);
        }
        else
        {
            robot.matchLoad(false);
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