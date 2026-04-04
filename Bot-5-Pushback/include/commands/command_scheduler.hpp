#pragma once

#ifndef COMMAND_SCHEDULER_HPP
#define COMMAND_SCHEDULER_HPP

#include "pros/rtos.hpp"
#include "commands/command.hpp"
#include <vector>
#include <algorithm>

namespace CommandScheduler
{
    int getCurrentTime();
    void scheduleCommand(Command *command);
    void setDefaultCommand(Command *command, int subsystem);
    void start();
    void run();
    std::vector<Command *> scheduledCommands;
    std::vector<std::pair<Command *, int>> defaultCommands; // pair of command and its requirements
}
#endif