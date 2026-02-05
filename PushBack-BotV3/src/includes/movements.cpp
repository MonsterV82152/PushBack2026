#include "movements.hpp"

Helper::Helper(
    std::int8_t left1,
    std::int8_t left2,
    std::int8_t right1,
    std::int8_t right2,
    std::int8_t leftIntake,
    std::int8_t rightIntake,
    std::int8_t leftHook,
    std::int8_t rightHook,
    pros::Rotation &scoringRotation,
    pros::Distance &intakeDS,
    Piston &scoreLift,
    Piston &matchLoader,
    Piston &descore,
    Piston &intakePTO,
    Piston &hookPTO,
    Piston &intakeLift,
    AutonSelector &autonSelector) : leftHookMotorPort(leftHook),
                                    rightHookMotorPort(rightHook),
                                    leftIntakeMotorPort(leftIntake),
                                    rightIntakeMotorPort(rightIntake),
                                    leftSensorMotor({left1, left2}),
                                    rightSensorMotor({right1, right2}),
                                    leftIntakeMotor(leftIntake),
                                    rightIntakeMotor(rightIntake),
                                    leftHookMotor(leftHook),
                                    rightHookMotor(rightHook),
                                    intakePTO(intakePTO),
                                    intakeLift(intakeLift),
                                    scoringRotation(scoringRotation),
                                    intakeDS(intakeDS),
                                    leftDT({left1, left2, leftIntake, leftHook}),
                                    rightDT({right1, right2, rightIntake, rightHook}),
                                    hookPTO(hookPTO),
                                    scoreLift(scoreLift),
                                    matchLoader(matchLoader),
                                    descore(descore),
                                    autonSelector(autonSelector)
{
}

Robot::Robot(Helper &helper, limelib::Locator &locator, pros::Controller &controller)
    : helper(helper), locator(locator), master(controller),
      intakePTOState(false),
      hookPTOState(false),
      intakePTOTask(nullptr),
      hookPTOTask(nullptr),
      scoringTask(nullptr),
      hookPID(0.45, 0, 0),
      angularPID4(3, 0, 25.5),
      linearPID4(5, 0, 59.75),
      angularPID6(3, 0.0000001, 21, 0.59),
      linearPID6(5.75, 0, 43.5),
      angularPID8(3, 0.0000001, 21, 1.32),
      linearPID8(8, 0, 38.5),
      chassis(locator, helper.leftDT, helper.rightDT,
              linearPID4,
              angularPID4)
{
}

void Robot::init()
{
    // pros::lcd::initialize();
    helper.autonSelector.start();
    hookPTOTask = std::make_unique<pros::Task>([&]() {});
    intakePTOTask = std::make_unique<pros::Task>([&]() {});
    scoringTask = std::make_unique<pros::Task>([&]() {});
    descoreTask = std::make_unique<pros::Task>([&]() {});
    intakePTOTask->remove();
    scoringTask->remove();
    locator.calibrate();
    helper.scoringRotation.reset_position();
    helper.scoringRotation.reset();
    helper.scoringRotation.set_position(0);
    descore(true);
}

void Robot::moveState(PTOState state)
{
    if (state.intakePTOState != LEAVE && helper.intakePTO.getState() != (state.intakePTOState == ON))
    {
        helper.intakePTO.setState(state.intakePTOState == ON);
        motorCount += state.intakePTOState == ON ? -1 : 1;
    }
    if (state.hookPTOState != LEAVE && helper.hookPTO.getState() != (state.hookPTOState == ON))
    {
        helper.hookPTO.setState(state.hookPTOState == ON);
        motorCount += state.hookPTOState == ON ? -1 : 1;
    }
    if (motorCount == 4)
    {
        chassis.setPID(linearPID8, angularPID8);
    }
    else if (motorCount == 3)
    {
        chassis.setPID(linearPID6, angularPID6);
    }
    else
    {
        chassis.setPID(linearPID4, angularPID4);
    }
    bool intakePTOCurrentState = intakePTOState.load();
    bool hookPTOCurrentState = hookPTOState.load();
    if (state.hookPTOState == ON && !hookPTOCurrentState)
    {
        if (hookTaskQueued.load())
        {
            return;
        }
        hookTaskQueued.store(true);
        hookPTOTask->create([this, state]()
                            {
                pros::delay(100);
                hookPTOState.store(true);
                hookTaskQueued.store(false);
                helper.leftDT.erase_port(helper.leftHookMotorPort);
                helper.rightDT.erase_port(helper.rightHookMotorPort);
                
                if (state.hookSpeed == 0) {
                    helper.leftHookMotor.brake();
                    helper.rightHookMotor.brake();
                } else
                if (state.hookSpeed != LEAVE)
                {
                    helper.leftHookMotor.move(state.hookSpeed);
                    helper.rightHookMotor.move(state.hookSpeed);
                } });
    }
    else if (state.hookPTOState == OFF && hookPTOCurrentState)
    {
        if (hookTaskQueued.load())
        {
            return;
        }
        hookTaskQueued.store(true);
        hookPTOTask->create([this, state]()
                            {
                pros::delay(200);
                hookTaskQueued.store(false);
                helper.leftDT.append(helper.leftHookMotor);
                helper.rightDT.append(helper.rightHookMotor);
                hookPTOState.store(false); });
    }
    else if (state.hookPTOState == ON)
    {
        if (state.hookSpeed == 0)
        {
            helper.leftHookMotor.brake();
            helper.rightHookMotor.brake();
        }
        else if (state.hookSpeed != LEAVE)
        {
            helper.leftHookMotor.move(state.hookSpeed);
            helper.rightHookMotor.move(state.hookSpeed);
        }
    }

    if (state.intakePTOState == ON && !intakePTOCurrentState)
    {
        if (intakeTaskQueued.load())
        {
            return;
        }
        intakeTaskQueued.store(true);
        intakePTOTask->create([this, state]()
                              {
                pros::delay(50);
                intakePTOState.store(true);
                intakeTaskQueued.store(false);
                helper.leftDT.erase_port(helper.leftIntakeMotorPort);
                helper.rightDT.erase_port(helper.rightIntakeMotorPort);
                
                if (state.intakeSpeed == 0) {
                    helper.leftIntakeMotor.brake();
                    helper.rightIntakeMotor.brake();
                } else   
                if (state.intakeSpeed != LEAVE)
                {
                    helper.leftIntakeMotor.move(-state.intakeSpeed);
                    helper.rightIntakeMotor.move(-state.intakeSpeed);
                } });
    }
    else if (state.intakePTOState == OFF && intakePTOCurrentState)
    {
        helper.leftDT.append(helper.leftIntakeMotor);
        helper.rightDT.append(helper.rightIntakeMotor);
        intakePTOState.store(false);
    }
    else if (state.intakePTOState == ON)
    {
        if (state.intakeSpeed == 0)
        {
            helper.leftIntakeMotor.brake();
            helper.rightIntakeMotor.brake();
        }
        else if (state.intakeSpeed != LEAVE)
        {
            helper.leftIntakeMotor.move(-state.intakeSpeed);
            helper.rightIntakeMotor.move(-state.intakeSpeed);
        }
    }
}

void Robot::debug()
{
    int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    helper.leftDT.move(forward + turn);
    helper.rightDT.move(forward - turn);
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
    {
        master.clear();
        chassis.setPose(0, 0, 0);
        chassis.moveToPoint(0, 48, 2000);
        chassis.waitUntilDone();
        master.print(0, 0, "Error X: %.2f Y: %.2f", locator.getPose().x, locator.getPose().y - 48);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        master.clear();
        chassis.setPose(0, 0, 0);
        chassis.turnToHeading(90, 2000);
        chassis.waitUntilDone();
        master.print(0, 0, "Error Heading: %.2f", locator.getPose().theta - 90);
    }

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    {
        moveState({OFF, OFF, LEAVE, LEAVE});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
    {
        moveState({ON, ON, 0, 0});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
    {
        moveState({OFF, ON, LEAVE, 0});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        float error = 0.0;
        for (int i = 10; i < 180; i += 10)
        {
            chassis.setPose(0, 0, 0);
            chassis.turnToHeading(i, 2500);
            chassis.waitUntilDone();
            error += locator.getPose().theta - i;
        }
        master.print(0, 0, "Error: %.2f", error / 18 * 1.5);
    }
}

void Robot::teleopControl()
{
    int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int input = pow(abs(turn) / 127.0, 1.7) * 127;
    input = turn < 0 ? -input : input;
    // // Curvature drive implementation
    // int left, right;
    // if (abs(forward) > TURN_THRESHOLD)
    // {
    //     // Curvature-based turning when moving
    //     double curvature = input * TURN_SENSITIVITY;
    //     left = forward + abs(forward) * curvature;
    //     right = forward - abs(forward) * curvature;
    // }
    // else
    // {
    //     // In-place turning when stationary
    //     left = input;
    //     right = -input;
    // }

    // helper.leftDT.move(left);
    // helper.rightDT.move(right);

    helper.leftDT.move(forward + input);
    helper.rightDT.move(forward - input);

    // if (forward > 15 || forward < -15 || turn > 15 || turn < -15)
    //     std::cout << "Forward: " << forward << " Turn: " << turn << std::endl;
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A) && pros::lcd::is_initialized())
    {
        limelib::Pose2D currentPose = locator.getPose();
        pros::lcd::print(4, "X: %.2f Y: %.2f H: %.2f", currentPose.x, currentPose.y, currentPose.theta);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
    {
        intaking = !intaking;
    }
    ScoringAction currentAction = currentScoringAction.load();
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2) && lowGoalPosition.load())
    {
        scoreLow();
    }
    if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && !descoring.load())
    {
        helper.intakeLift.setState(false);
    }
    if (!parking.load())
    {
        if (descoring.load())
        {
            moveState({ON, LEAVE, -40, LEAVE});
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            if (helper.autonSelector.isSkills())
                moveState({ON, LEAVE, -40, LEAVE});
            else
                moveState({ON, LEAVE, -127, LEAVE});
        }
        else if (intaking && currentAction == ScoringAction::IDLE && !liftState)
        {
            moveState({ON, LEAVE, 127, LEAVE});
        }
        else
        {
            moveState({OFF, LEAVE, LEAVE, LEAVE});
        }
    }
    else
    {
        intaking = false;
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
    {
        matchLoad();
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_B))
    {
        matchLoad(false);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
    {
        lift(true);
        descore();
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_DOWN) && liftState)
    {
        descore(false);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
    {
        liftState = !liftState;
        lift(liftState);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    {
        if (liftState)
        {
            scoringPosition = 1850;
            if (helper.autonSelector.isSkills())
                maxSpeed = 100;
        }
        else
        {
            if (helper.autonSelector.isSkills())
            {
                maxSpeed = 55;
                approaching.store(true);
            }
        }
        setScoringAction(ScoringAction::SCOREANDHOLD);
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1))
    {
        setScoringAction(ScoringAction::RESET);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        setLow();
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        matchLoad(false);
        setScoringAction(ScoringAction::RESET);
    }
    // if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
    // {
    //     setScoringAction(ScoringAction::UNJAM);
    // }
}

void Robot::setScoringAction(ScoringAction action)
{
    currentScoringAction.store(action);
    fullSpeed.store(false);
    if (!scoringTaskRunning.load())
    {
        scoringTask->create([this]()
                            { scoringLoop(); });
    }
}

void Robot::scoringLoop()
{
    scoringTaskRunning.store(true);
    double currentAngle = 0;
    double unjamAngle = 0;
    int parkState = 0;
    int velo = 0;
    while (scoringTaskRunning.load())
    {
        ScoringAction action = currentScoringAction.load();
        currentAngle = helper.scoringRotation.get_position() / 100.0;
        velo = abs(helper.scoringRotation.get_velocity() / 100);
        switch (action)
        {
        case ScoringAction::SCOREANDHOLD:
            lastState = ScoringAction::SCOREANDRESET;
            if (abs(currentAngle - scoringPosition) > 5)
            {
                if (approaching.load() && abs(scoringPosition - currentAngle) < 50)
                {
                    maxSpeed = 30;
                }
                double speed = hookPID.update(scoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                if (velo > 15)
                    fullSpeed.store(true);
                if (fullSpeed.load() && velo < 1 && speed > 20)
                {
                    unjamAngle = currentAngle - 90;
                    currentScoringAction.store(ScoringAction::UNJAM);
                    hookPID.reset();
                }
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::HOLD);
                hookPID.reset();
            }

            break;
        case ScoringAction::SCOREANDRESET:
            lastState = ScoringAction::SCOREANDRESET;
            if (abs(scoringPosition - currentAngle) > 5)
            {
                if (approaching.load() && abs(scoringPosition - currentAngle) < 50)
                {
                    maxSpeed = 30;
                }
                double speed = hookPID.update(scoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                if (velo > 15)
                    fullSpeed.store(true);
                if (fullSpeed.load() && velo < 1 && speed > 20)
                {
                    unjamAngle = currentAngle - 90;
                    currentScoringAction.store(ScoringAction::UNJAM);
                    hookPID.reset();
                }
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::RESET);
                hookPID.reset();
            }
            break;
        case ScoringAction::LOWGOAL:

            if (abs(LOWGOAL_POSITION - currentAngle) > 5)
            {
                double speed = hookPID.update(LOWGOAL_POSITION - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                moveState({LEAVE, OFF, LEAVE, LEAVE});
                currentScoringAction.store(ScoringAction::IDLE);
                lowGoalPosition.store(true);
                hookPID.reset();
            }
            break;
        case ScoringAction::SCORELOW:
            lastState = ScoringAction::SCORELOW;
            if (abs(SCORELOW_POSITION - currentAngle) > 5)
            {
                lowGoalPosition.store(false);
                double speed = hookPID.update(SCORELOW_POSITION - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (velo > 3)
                    fullSpeed.store(true);
                if (fullSpeed.load() && velo < 1 && abs(speed) > 10 && currentAngle > 20)
                {
                    unjamAngle = currentAngle + 120;
                    currentScoringAction.store(ScoringAction::UNJAM);
                    hookPID.reset();
                }
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                matchLoad(false);
                intaking = false;
                currentScoringAction.store(ScoringAction::RESET);
                descoring.store(false);
                hookPID.reset();
            }
            break;
        case ScoringAction::RESET:
        {
            scoringPosition = DEFAULT_SCORING_POSITION;
            descoringPosition = DEFAULT_DESCORING_POSITION;
            maxSpeed = 127;
            parking.store(false);
            descoring.store(false);
            approaching.store(false);
            parkState = 0;
            if (currentAngle > 5 || velo > 5)
            {
                double speed =
                    /*hookPID.update(-currentAngle);
                    speed += speed > 0 ? 10 : -10;
                    if (speed > 127)
                        speed = 127;
                    else if (speed < -127)
                        speed = -127;*/
                    -100;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                moveState({LEAVE, ON, LEAVE, 0});
                pros::delay(400);
                moveState({LEAVE, OFF, LEAVE, 0});
                currentScoringAction.store(ScoringAction::IDLE);
                hookPID.reset();
            }
            break;
        }
        case ScoringAction::HOLD:
        {
            // Hold position - stop motors but keep PTO engaged
            double speed = hookPID.update(scoringPosition - currentAngle);
            if (speed > 127)
                speed = 127;
            else if (speed < -127)
                speed = -127;
            moveState({LEAVE, ON, LEAVE, (short)speed});
            break;
        }
        case ScoringAction::IDLE:

            break;
        case ScoringAction::UNJAM:
            moveState({LEAVE, ON, LEAVE, -127});
            if (abs(currentAngle - unjamAngle) > 10)
            {
                double speed = hookPID.update(unjamAngle - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                fullSpeed.store(false);
                currentScoringAction.store(lastState);
                hookPID.reset();
            }
            break;
        }

        pros::delay(20); // Small delay to prevent CPU hogging
    }
}

void Robot::setLow()
{
    setScoringAction(ScoringAction::LOWGOAL);
}
void Robot::scoreLow(int maxSpeed)
{
    this->maxSpeed = maxSpeed;
    descoring.store(true);
    matchLoad(true);
    helper.intakeLift.setState(true);
    descoreTask->create([this]()
                        {
        pros::delay(500);
        matchLoad(false); });
    setScoringAction(ScoringAction::SCORELOW);
}
void Robot::lowerIntake()
{
    helper.intakeLift.setState(false);
}
void Robot::raiseIntake()
{
    helper.intakeLift.setState(true);
}
void Robot::score(int position, int maxSpeed, bool approach)
{
    scoringPosition = position;
    this->maxSpeed = maxSpeed;
    approaching.store(approach);
    setScoringAction(ScoringAction::SCOREANDRESET);
}
void Robot::lift(bool up)
{
    helper.scoreLift.setState(up);
    if (!up)
    {
        descore(true);
    //     descoreTask->create([this]()
    //                         {
    //         pros::delay(500);
    //         liftState = false; });
    // } else {
    //     liftState = true;
    }
    liftState = up;
}
void Robot::reset()
{
    setScoringAction(ScoringAction::RESET);
}
void Robot::intake(bool on)
{
    if (on)
    {
        moveState({ON, LEAVE, 127, LEAVE});
    }
    else
    {
        moveState({OFF, LEAVE, LEAVE, LEAVE});
    }
}
void Robot::matchLoad(bool load)
{
    helper.matchLoader.setState(load);
}
void Robot::descore(bool descoring)
{
    helper.descore.setState(!descoring);
}
void Robot::setPose(limelib::real_t x, limelib::real_t y, limelib::real_t theta)
{
    chassis.setPose(x, y, theta);
}
void Robot::waitUntilDone()
{
    chassis.waitUntilDone();
}

void Robot::disabled()
{
    lift(!liftState);
}

void Robot::moveToPoint(limelib::real_t x, limelib::real_t y, int timeout, limelib::moveToPointParams params)
{
    chassis.moveToPoint(x, y, timeout, params);
}
void Robot::turnToHeading(limelib::real_t heading, int timeout, limelib::turnToHeadingParams params)
{
    chassis.turnToHeading(heading, timeout, params);
}
void Robot::turnToPoint(limelib::real_t x, limelib::real_t y, int timeout, limelib::turnToHeadingParams params)
{
    chassis.turnToPoint(x, y, timeout, params);
}