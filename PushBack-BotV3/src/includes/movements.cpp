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
                       scoreLift(scoreLift)
{
}

Robot::Robot(Helper &helper, limelib::MCL &mcl, pros::Controller &controller)
    : helper(helper), mcl(mcl), master(controller),
      intakePTOState(false),
      hookPTOState(false),
      intakePTOTask(nullptr),
      hookPTOTask(nullptr),
      scoringTask(nullptr),
      hookPID(0.4, 0.001, 0.5, 0.05, true),
      angularPID4(0, 0, 0),
      lateralPID4(0, 0, 0),
      angularPID6(0, 0, 0),
      lateralPID6(0, 0, 0),
      angularPID8(0, 0, 0),
      lateralPID8(0, 0, 0),
      chassis(mcl, helper.leftDT, helper.rightDT,
              lateralPID4,
              angularPID4)
{
}

void Robot::init()
{
    intakePTOTask = std::make_unique<pros::Task>([&]() {});
    hookPTOTask = std::make_unique<pros::Task>([&]() {});
    scoringTask = std::make_unique<pros::Task>([&]() {});
    helper.scoringRotation.set_position(0);
    intakePTOTask->remove();
    hookPTOTask->remove();
    scoringTask->remove();
}

void Robot::moveState(PTOState state)
{
    if (state.hookPTOState != LEAVE)
        helper.hookPTO.setState(state.hookPTOState == ON);
    if (state.intakePTOState != LEAVE)
        helper.intakePTO.setState(state.intakePTOState == ON);
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
        helper.leftDT.append(helper.leftHookMotor);
        helper.rightDT.append(helper.rightHookMotor);
        hookPTOState.store(false);
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

void Robot::teleopControl()
{
    int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    helper.leftDT.move(forward + turn);
    helper.rightDT.move(forward - turn);
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
    {
        intaking = !intaking;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        moveState({ON, LEAVE, -127, LEAVE});
    }
    else if (intaking && !scoringTaskRunning.load() && !liftState)
    {
        moveState({ON, LEAVE, 127, LEAVE});
    }
    else
    {
        moveState({OFF, LEAVE, LEAVE, LEAVE});
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
    {
        liftState = !liftState;
        helper.scoreLift.setState(liftState);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    {
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
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
        hookPID.kP += 0.5;
        pros::lcd::print(2, "Hook PID kP: %.2f", hookPID.kP);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
    {
        hookPID.kP -= 0.5;
        pros::lcd::print(2, "Hook PID kP: %.2f", hookPID.kP);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
    {
        hookPID.kD += 1;
        pros::lcd::print(1, "Hook PID kD: %.2f", hookPID.kD);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
    {
        hookPID.kD -= 1;
        pros::lcd::print(1, "Hook PID kD: %.2f", hookPID.kD);
    }
}

void Robot::setScoringAction(ScoringAction action)
{
    currentScoringAction.store(action);

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

        switch (action)
        {
        case ScoringAction::DESCOREANDHOLD:
            if (abs(DESCORING_POSITION - currentAngle) > 5)
            {
                double speed = hookPID.update(DESCORING_POSITION - currentAngle);
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                helper.leftHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                helper.rightHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                currentScoringAction.store(ScoringAction::DEHOLD);
                hookPID.reset();
            }

            break;
        case ScoringAction::DESCOREANDRESET:
            if (abs(DESCORING_POSITION - currentAngle) > 5)
            {
                double speed = hookPID.update(DESCORING_POSITION - currentAngle);
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::RESET);
                hookPID.reset();
            }
            break;
        case ScoringAction::SCOREANDHOLD:
            if (abs(currentAngle - SCORING_POSITION) > 5)
            {
                double speed = hookPID.update(SCORING_POSITION - currentAngle);
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                helper.leftHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                helper.rightHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                currentScoringAction.store(ScoringAction::HOLD);
                hookPID.reset();
            }

            break;
        case ScoringAction::SCOREANDRESET:
            if (abs(currentAngle - SCORING_POSITION) > 5)
            {
                double speed = hookPID.update(SCORING_POSITION - currentAngle);
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                currentScoringAction.store(ScoringAction::RESET);
                hookPID.reset();
            }
            break;
        case ScoringAction::RESET:
            if (abs(currentAngle - 0) > 10)
            {
                double speed = hookPID.update(-currentAngle);
                if (speed > 127)
                    speed = 127;
                else if (speed < -127)
                    speed = -127;
                moveState({LEAVE, ON, LEAVE, (short)speed});
            }
            else
            {
                helper.leftHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                helper.rightHookMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                moveState({LEAVE, OFF, LEAVE, LEAVE});
                scoringTaskRunning.store(false);
                hookPID.reset();
            }
            break;

        case ScoringAction::HOLD:
        {
            // Hold position - stop motors but keep PTO engaged
            double speed = hookPID.update(SCORING_POSITION - currentAngle);
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
            double speed = hookPID.update(DESCORING_POSITION - currentAngle);
            if (speed > 127)
                speed = 127;
            else if (speed < -127)
                speed = -127;
            moveState({LEAVE, ON, LEAVE, (short)speed});
            break;
        }
        }

        pros::delay(10); // Small delay to prevent CPU hogging
    }
}

void Robot::score()
{
    setScoringAction(ScoringAction::SCOREANDRESET);
}