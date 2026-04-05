#pragma once
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "commands/command.hpp"
#include "commands/command_variations.hpp"
#include <functional>

namespace Commands
{
    Command *run(std::function<void()> execute, std::initializer_list<int> requirements = {});
    Command *runOnce(std::function<void()> execute, std::initializer_list<int> requirements = {});
    Command *sequential(std::initializer_list<Command *> commands);
    Command *parallel(std::initializer_list<Command *> commands);
    Command *race(std::initializer_list<Command *> commands);
    Command *wait(int milliseconds);
    Command *empty();
    Command *conditional(std::function<bool()> condition, Command *onTrue, Command *onFalse);
}

#endif
