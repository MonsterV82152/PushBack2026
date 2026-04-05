#include "robot_commands.hpp"

Command *switchPTOState(bool state)
{
    return Commands::conditional(
        [state]()
        {
            return pto.getState() != state;
        },
        // Commands::sequential({
        Commands::runOnce([state]()
                          { pto.setState(state); }, {MANIPULATOR_ID}),
        // Commands::wait(500),
        // }),
        Commands::empty());
}

Command *score(std::function<double(double)> &scoreFunc)
{
    return Commands::sequential({
        switchPTOState(SCORING_ENGAGED),
        new TransitionToScore(scoreFunc),
        new HoldScoring(scoreFunc),
    });
}

Command *intake()
{
    return Commands::sequential({
        lowerScoring(),
        switchPTOState(INTAKE_ENGAGED),
        new StartIntake(),
    });
}

Command *reverseIntake()
{
    return Commands::sequential({
        lowerScoring(),
        switchPTOState(INTAKE_ENGAGED),
        new ReverseIntake(),
    });
}

Command *lowerScoring()
{
    return Commands::conditional(
        []()
        {
            return RobotStates::stickState == RobotState::INTAKING;
        },
        Commands::empty(), Commands::sequential({switchPTOState(SCORING_ENGAGED), new LowerScoring()}));
}