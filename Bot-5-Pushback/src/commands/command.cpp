#include "commands/command.hpp"

Command *Command::withTimeout(int timeout)
{
    this->timeout = timeout;
    return this;
}

void Command::setCurrentTime(int currentTime)
{
    endTime = currentTime + timeout;
}

void Command::addRequirements(std::initializer_list<int> subsystems)
{
    for (int subsystem : subsystems)
    {
        requirements.push_back(subsystem);
    }
}
