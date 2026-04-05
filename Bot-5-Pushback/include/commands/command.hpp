#pragma once
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <initializer_list>
#include <vector>

class Command
{

public:
    virtual ~Command() = default;
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual void end(bool interrupted) = 0;
    virtual bool isFinished() const = 0;
    Command *withTimeout(int timeout);
    void setCurrentTime(int currentTime);
    int getTimeout() const { return timeout; }
    int getEndTime() const { return endTime; }
    std::vector<int> getRequirements() const { return requirements; }

protected:
    int timeout = 0;
    int endTime = 0;
    std::vector<int> requirements;
    void addRequirements(std::initializer_list<int> subsystems);
};

#endif