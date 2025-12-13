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
    Piston &intakePTO,
    Piston &hookPTO
    ) : left1(left1), 
                            left2(left2),
                            right1(right1), 
                            right2(right2),
                            leftIntakeMotor(leftIntake), 
                            rightIntakeMotor(rightIntake),
                            leftHookMotor(leftHook), 
                            rightHookMotor(rightHook),
                            intakePTO(intakePTO), 
                            leftDT4({left1, left2, leftIntake, leftHook}),
                            rightDT4({right1, right2, rightIntake, rightHook}),
                            leftDT3Hook({left1, left2, leftHook}),
                            leftDT3Intake({left1, left2, leftIntake}),
                            rightDT3Hook({right1, right2, rightHook}),
                            rightDT3Intake({right1, right2, rightIntake}),
                            leftDT2({left1, left2}),
                            rightDT2({right1, right2}),
                            hookPTO(hookPTO)
{
}

Robot::Robot(Helper &helper, limelib::MCL &mcl, pros::Controller &controller)
    : helper(helper), mcl(mcl), master(controller), 
    intakePTOState(false),
    hookPTOState(false),
    intakePTOTask([&](){}),
    hookPTOTask([&](){}),
    angularPID4(0,0,0), 
    lateralPID4(0,0,0), 
    velocityPID4(0,0,0), 
    motionProfile4(0.5, 1.0), 
    angularPID6(0,0,0), 
    lateralPID6(0,0,0), 
    velocityPID6(0,0,0), 
    motionProfile6(0.5, 1.0), 
    angularPID8(0,0,0), 
    lateralPID8(0,0,0), 
    velocityPID8(0,0,0), 
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

void Robot::teleopControl() {
    int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    helper.leftDT4.move(forward + turn);
    helper.rightDT4.move(forward - turn);
    if (!intakePTOState) {
        helper.leftIntakeMotor.move(forward + turn);
        helper.rightIntakeMotor.move(forward - turn);
    }
    if (!hookPTOState) {
        helper.leftHookMotor.move(forward + turn);
        helper.rightHookMotor.move(forward - turn);
    }
    
}
