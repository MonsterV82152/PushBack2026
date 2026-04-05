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

    inline pros::Mutex mutex;
    inline std::vector<Command *> scheduledCommands;
    inline std::vector<Command *> pendingCommands;                 // staged by external tasks, drained each tick
    inline std::vector<std::pair<Command *, int>> defaultCommands; // pair of command and its subsystem
}
#endif