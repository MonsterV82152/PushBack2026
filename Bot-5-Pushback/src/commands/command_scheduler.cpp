#include "commands/command_scheduler.hpp"

int CommandScheduler::getCurrentTime()
{
    return pros::millis();
}

void CommandScheduler::scheduleCommand(Command *command)
{
    command->setCurrentTime(getCurrentTime());
    scheduledCommands.push_back(command);
    for (Command *cmd : scheduledCommands)
    {
        for (int req : cmd->getRequirements())
        {
            if (std::find(command->getRequirements().begin(), command->getRequirements().end(), req) != command->getRequirements().end())
            {
                cmd->end();
                break;
            }
        }
    }
    command->initialize();
}

void CommandScheduler::setDefaultCommand(Command *command, int subsystem)
{
    for (auto it = defaultCommands.begin(); it != defaultCommands.end(); ++it)
    {
        if (it->second == subsystem)
        {
            delete it->first;
            it->first = command;
            return;
        }
    }
    defaultCommands.emplace_back(command, subsystem);
    scheduleCommand(command);
}

void CommandScheduler::run()
{
    int currentTime = getCurrentTime();
    for (auto it = scheduledCommands.begin(); it != scheduledCommands.end();)
    {
        Command *command = *it;
        command->execute();
        if (command->isFinished() || (command->getTimeout() > 0 && currentTime >= command->getEndTime()))
        {
            command->end();
            delete command;
            it = scheduledCommands.erase(it);
            for (const auto &[defaultCmd, subsystem] : defaultCommands)
            {
                if (std::find(command->getRequirements().begin(), command->getRequirements().end(), subsystem) != command->getRequirements().end())
                {
                    scheduleCommand(defaultCmd);
                }
            }
        }
        else
        {
            ++it;
        }
    }
}

void CommandScheduler::start()
{
    scheduledCommands.clear();
    pros::Task runTask([](void *)
                     {
                         while (true)
                         {
                             CommandScheduler::run();
                             pros::delay(20); // Run the scheduler every 20ms
                         }
                     });
}