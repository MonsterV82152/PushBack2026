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
    Piston &scoreLift,
    Piston &matchLoader,
    Piston &descore,
    Piston &intakePTO,
    Piston &hookPTO) : leftHookMotorPort(leftHook),
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
                       scoringRotation(scoringRotation),
                       leftDT({left1, left2, leftIntake, leftHook}),
                       rightDT({right1, right2, rightIntake, rightHook}),
                       hookPTO(hookPTO),
                       scoreLift(scoreLift),
                       matchLoader(matchLoader),
                       descore(descore)
{
}

Robot::Robot(Helper &helper, limelib::Locator &locator, pros::Controller &controller)
    : helper(helper), locator(locator), master(controller),
      intakePTOState(false),
      hookPTOState(false),
      intakePTOTask(nullptr),
      hookPTOTask(nullptr),
      scoringTask(nullptr),
      hookPID(0.4, 0.001, 0.5, 0.05, true),
      angularPID4(3, 0, 24.5),
      linearPID4(5, 0, 60),
      angularPID6(3, 0.0000001, 20.75, 0.59),
      linearPID6(5.75, 0, 44

                 ),
      angularPID8(3, 0.0000001, 20.75, 1.32),
      linearPID8(8, 0, 38),
      chassis(locator, helper.leftDT, helper.rightDT,
              linearPID4,
              angularPID4)
{
}

void Robot::init()
{
    pros::lcd::initialize();
    // hookPTOTask = std::make_unique<pros::Task>([this]()
    //                                            { this->scoringLoop(); });
    currentScoringAction.store(ScoringAction::RESET);
    locator.calibrate();
    hookPTOTask = std::make_unique<pros::Task>([&]() {});
    intakePTOTask = std::make_unique<pros::Task>([&]() {});
    scoringTask = std::make_unique<pros::Task>([&]() {});
    descoreTask = std::make_unique<pros::Task>([&]() {});
    helper.scoringRotation.set_position(0);
    helper.descore.setState(true);
    intakePTOTask->remove();
    scoringTask->remove();
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
                pros::delay(50);
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
                pros::delay(100);
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
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
    {
        moveState({OFF, OFF, LEAVE, LEAVE});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
    {
        moveState({ON, ON, 0, 0});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
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
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * 1.2;
    helper.leftDT.move(forward + turn);
    helper.rightDT.move(forward - turn);
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
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        moveState({ON, LEAVE, -127, LEAVE});
    }
    else if (intaking && (currentAction == ScoringAction::RESET || currentAction == ScoringAction::IDLE) && !liftState)
    {
        moveState({ON, LEAVE, 127, LEAVE});
    }
    else
    {
        moveState({OFF, LEAVE, LEAVE, LEAVE});
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
        descore();
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_DOWN))
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
            scoringPosition = 1850;
        else
            maxSpeed = 90;
        setScoringAction(ScoringAction::SCOREANDHOLD);
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1))
    {
        setScoringAction(ScoringAction::RESET);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        setScoringAction(ScoringAction::DESCOREANDHOLD);
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_X))
    {
        setScoringAction(ScoringAction::RESET);
    }
}

void Robot::setScoringAction(ScoringAction action)
{
    currentScoringAction.store(action);
    fullSpeed.store(false);
    if (!scoringTaskRunning.load())
    {
        scoringTask->create([this]()
                            { this->scoringLoop(); });
    }
}

void Robot::scoringLoop()
{
    scoringTaskRunning.store(true);

    while (scoringTaskRunning.load())
    {
        ScoringAction action = currentScoringAction.load();
        currentAngle = helper.scoringRotation.get_position() / 100.0;
        int velo = helper.scoringRotation.get_velocity() / 100;
        switch (action)
        {
        case ScoringAction::DESCOREANDHOLD:
            if (abs(descoringPosition - currentAngle) > 5)
            {
                double speed = hookPID.update(descoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::DEHOLD);
                hookPID.reset();
            }

            break;
        case ScoringAction::DESCOREANDRESET:
            if (abs(descoringPosition - currentAngle) > 5)
            {
                double speed = hookPID.update(descoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;

                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::RESET);
                hookPID.reset();
            }
            break;
        case ScoringAction::SCOREANDHOLD:
            if (abs(currentAngle - scoringPosition) > 5)
            {
                double speed = hookPID.update(scoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                if (velo > 180)
                    fullSpeed.store(true);
                if (fullSpeed.load() && velo < 10 && speed > 30)
                {
                    lastState = ScoringAction::SCOREANDRESET;
                    unjamAngle = currentAngle;
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
            if (scoringPosition - currentAngle > 5)
            {
                double speed = hookPID.update(scoringPosition - currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > maxSpeed)
                    speed = maxSpeed;
                else if (speed < -maxSpeed)
                    speed = -maxSpeed;
                if (velo > 180)
                    fullSpeed.store(true);
                if (fullSpeed.load() && velo < 10 && speed > 30)
                {
                    lastState = ScoringAction::SCOREANDRESET;
                    unjamAngle = currentAngle;
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
        case ScoringAction::RESET:
        {
            scoringPosition = DEFAULT_SCORING_POSITION;
            descoringPosition = DEFAULT_DESCORING_POSITION;
            maxSpeed = 127;
            if (abs(currentAngle) > 10)
            {
                double speed = hookPID.update(-currentAngle);
                speed += speed > 0 ? FEEDFORWARD : -FEEDFORWARD;
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                moveState({LEAVE, OFF, LEAVE, 0});
                currentScoringAction.store(ScoringAction::IDLE);
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
        case ScoringAction::DEHOLD:
        {
            // Hold position - stop motors but keep PTO engaged
            double speed = hookPID.update(descoringPosition - currentAngle);
            if (speed > 127)
                speed = 127;
            else if (speed < -127)
                speed = -127;
            moveState({LEAVE, ON, LEAVE, (short)speed});
            break;
        }
        case ScoringAction::IDLE:
            // Do nothing
            break;
        case ScoringAction::UNJAM:
            moveState({LEAVE, ON, LEAVE, -127});
            if (unjamAngle - 30 > currentAngle)
            {
                currentScoringAction.store(lastState);
                hookPID.reset();
            }
            else
            {
                currentScoringAction.store(lastState);
                hookPID.reset();
            }
            break;
        }

        pros::delay(20); // Small delay to prevent CPU hogging
    }
}

void Robot::score(int position, int maxSpeed)
{
    scoringPosition = position;
    this->maxSpeed = maxSpeed;
    setScoringAction(ScoringAction::SCOREANDRESET);
}
void Robot::lift(bool up)
{
    if (up)
    {
        moveState({OFF, LEAVE, LEAVE, LEAVE});
    }
    if (!up)
    {
        helper.scoreLift.setState(false);
        descoreTask->create([this]()
                            {
            pros::delay(500);
            this->helper.descore.setState(true); });
    }
    else
    {
        helper.scoreLift.setState(up);
    }
    liftState = up;
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
    if (!liftState && descoring)
    {
        liftState = true;
        helper.descore.setState(false);
        helper.scoreLift.setState(liftState);
        descoreTask->create([this, descoring]()
                            {
            pros::delay(500);
            this->helper.descore.setState(descoring); });
    }
    else if (liftState)
    {
        helper.descore.setState(descoring);
    }
}
void Robot::setPose(limelib::real_t x, limelib::real_t y, limelib::real_t theta)
{
    chassis.setPose(x, y, theta);
}
void Robot::waitUntilDone()
{
    chassis.waitUntilDone();
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