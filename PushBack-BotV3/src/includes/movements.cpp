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
    Piston &hookPTO) : left1(left1),
                       left2(left2),
                       right1(right1),
                       right2(right2),
                       leftIntakeMotor(leftIntake),
                       rightIntakeMotor(rightIntake),
                       leftHookMotor(leftHook),
                       rightHookMotor(rightHook),
                       intakePTO(intakePTO),
                       scoringRotation(scoringRotation),
                       leftDT4({left1, left2, leftIntake, leftHook}),
                       rightDT4({right1, right2, rightIntake, rightHook}),
                       leftDT3Hook({left1, left2, leftHook}),
                       leftDT3Intake({left1, left2, leftIntake}),
                       rightDT3Hook({right1, right2, rightHook}),
                       rightDT3Intake({right1, right2, rightIntake}),
                       leftDT2({left1, left2}),
                       rightDT2({right1, right2}),
                       hookPTO(hookPTO),
                       scoreLift(scoreLift)
{
}

Robot::Robot(Helper &helper, limelib::MCL &mcl, pros::Controller &controller)
    : helper(helper), mcl(mcl), master(controller),
      intakePTOState(false),
      hookPTOState(false),
      intakePTOTask([&]() {}),
      hookPTOTask([&]() {}),
      scoringTask([&]() {}),
      hookPID(0.4, 0.0005, 0.6, 0.05, true),
      angularPID4(0, 0, 0),
      lateralPID4(0, 0, 0),
      velocityPID4(0, 0, 0),
      motionProfile4(0.5, 1.0),
      angularPID6(0, 0, 0),
      lateralPID6(0, 0, 0),
      velocityPID6(0, 0, 0),
      motionProfile6(0.5, 1.0),
      angularPID8(0, 0, 0),
      lateralPID8(0, 0, 0),
      velocityPID8(0, 0, 0),
      motionProfile8(0.5, 1.0),
      chassis4(mcl, helper.leftDT4, helper.rightDT4,
               lateralPID4,
               velocityPID4,
               angularPID4,
               motionProfile4),
      chassis6Intake(mcl, helper.leftDT3Intake, helper.rightDT3Intake,
                     lateralPID6,
                     velocityPID6,
                     angularPID6,
                     motionProfile6),
      chassis6Hook(mcl, helper.leftDT3Hook, helper.rightDT3Hook,
                   lateralPID6,
                   velocityPID6,
                   angularPID6,
                   motionProfile6),
      chassis8(mcl, helper.leftDT4, helper.rightDT4,
               lateralPID8,
               velocityPID8,
               angularPID8,
               motionProfile8)
{
}

void Robot::init()
{
    pros::lcd::initialize();
    helper.scoringRotation.set_position(0);
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
        if (hookPTOTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING)
        {
            return;
        }
        hookPTOTask.create([this, state]()
                           {
                pros::delay(50);
                hookPTOState.store(true);
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
        if (intakePTOTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING)
        {
            return;
        }
        intakePTOTask.create([this, state]()
                             {
                pros::delay(50);
                intakePTOState.store(true);
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
    helper.leftDT4.move(forward + turn);
    helper.rightDT4.move(forward - turn);

    if (!intakePTOState.load())
    {
        helper.leftIntakeMotor.move(forward + turn);
        helper.rightIntakeMotor.move(forward - turn);
    }
    if (!hookPTOState.load())
    {
        helper.leftHookMotor.move(forward + turn);
        helper.rightHookMotor.move(forward - turn);
    }
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
        startScoring();
    }
    if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1))
    {
        stopScoring();
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
            // Hold position - stop motors but keep PTO engaged
            double speed = hookPID.update(SCORING_POSITION - currentAngle);
            if (speed > 127)
                speed = 127;
            else if (speed < -127)
                speed = -127;
            moveState({LEAVE, ON, LEAVE, (short)speed});
            break;
        }

        pros::delay(10); // Small delay to prevent CPU hogging
    }
}

void Robot::score()
{
    setScoringAction(ScoringAction::SCOREANDRESET);

    if (!scoringTaskRunning.load())
    {
        scoringTask.create([this]()
                           { this->scoringLoop(); });
    }
}

void Robot::startScoring()
{
    setScoringAction(ScoringAction::SCOREANDHOLD);

    if (!scoringTaskRunning.load())
    {
        scoringTask.create([this]()
                           { this->scoringLoop(); });
    }
}

void Robot::stopScoring()
{
    setScoringAction(ScoringAction::RESET);

    if (!scoringTaskRunning.load())
    {
        scoringTask.create([this]()
                           { this->scoringLoop(); });
    }
}
