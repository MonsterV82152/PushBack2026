#pragma once

#ifndef ROBOT_STATES_HPP
#define ROBOT_STATES_HPP

enum class RobotState
{
    INTAKING,
    OUTTAKING,
    SCORING,
    TRANSITIONING,
    IDLE
};
namespace RobotStates
{
    inline bool shouldBeIntaking = false;
    inline RobotState currentState = RobotState::IDLE;
    inline RobotState targetState = RobotState::IDLE;
    inline RobotState stickState = RobotState::INTAKING;
}
inline std::function<double(double)> matchScoreFunction = [](double input){ return 127.0; };

#endif