#pragma once
#ifndef COMMAND_VARIATIONS_HPP
#define COMMAND_VARIATIONS_HPP

#include "commands/command.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <algorithm>

// Runs commands one after another. Finishes when the last command finishes.
class SequentialCommandGroup : public Command
{
public:
    SequentialCommandGroup(std::initializer_list<Command *> cmds)
        : commands(cmds), currentIndex(0)
    {
        for (Command *cmd : commands)
            for (int req : cmd->getRequirements())
                requirements.push_back(req);
    }

    ~SequentialCommandGroup()
    {
        for (Command *cmd : commands)
            delete cmd;
    }

    void initialize() override
    {
        currentIndex = 0;
        if (!commands.empty())
        {
            commands[0]->setCurrentTime(pros::millis());
            commands[0]->initialize();
        }
    }

    void execute() override
    {
        if (currentIndex >= (int)commands.size())
            return;
        Command *current = commands[currentIndex];
        current->execute();
        int now = pros::millis();
        bool timedOut = current->getTimeout() > 0 && now >= current->getEndTime();
        if (current->isFinished() || timedOut)
        {
            current->end(timedOut);
            ++currentIndex;
            if (currentIndex < (int)commands.size())
            {
                commands[currentIndex]->setCurrentTime(now);
                commands[currentIndex]->initialize();
            }
        }
    }

    void end(bool interrupted) override
    {
        if (currentIndex < (int)commands.size())
            commands[currentIndex]->end(interrupted);
    }

    bool isFinished() const override
    {
        return currentIndex >= (int)commands.size();
    }

private:
    std::vector<Command *> commands;
    int currentIndex;
};

// Runs all commands simultaneously. Finishes when ALL commands finish.
class ParallelCommandGroup : public Command
{
public:
    ParallelCommandGroup(std::initializer_list<Command *> cmds)
        : commands(cmds), finished(cmds.size(), false)
    {
        for (Command *cmd : commands)
            for (int req : cmd->getRequirements())
                requirements.push_back(req);
    }

    ~ParallelCommandGroup()
    {
        for (Command *cmd : commands)
            delete cmd;
    }

    void initialize() override
    {
        int now = pros::millis();
        std::fill(finished.begin(), finished.end(), false);
        for (Command *cmd : commands)
        {
            cmd->setCurrentTime(now);
            cmd->initialize();
        }
    }

    void execute() override
    {
        int now = pros::millis();
        for (int i = 0; i < (int)commands.size(); ++i)
        {
            if (finished[i])
                continue;
            commands[i]->execute();
            bool timedOut = commands[i]->getTimeout() > 0 && now >= commands[i]->getEndTime();
            if (commands[i]->isFinished() || timedOut)
            {
                commands[i]->end(timedOut);
                finished[i] = true;
            }
        }
    }

    void end(bool interrupted) override
    {
        for (int i = 0; i < (int)commands.size(); ++i)
        {
            if (!finished[i])
            {
                commands[i]->end(interrupted);
                finished[i] = true;
            }
        }
    }

    bool isFinished() const override
    {
        return std::all_of(finished.begin(), finished.end(), [](bool f)
                           { return f; });
    }

private:
    std::vector<Command *> commands;
    std::vector<bool> finished;
};

// Runs all commands simultaneously. Finishes as soon as ANY one command finishes,
// cancelling the rest.
class RaceCommandGroup : public Command
{
public:
    RaceCommandGroup(std::initializer_list<Command *> cmds)
        : commands(cmds), finished(cmds.size(), false), raceWon(false)
    {
        for (Command *cmd : commands)
            for (int req : cmd->getRequirements())
                requirements.push_back(req);
    }

    ~RaceCommandGroup()
    {
        for (Command *cmd : commands)
            delete cmd;
    }

    void initialize() override
    {
        int now = pros::millis();
        std::fill(finished.begin(), finished.end(), false);
        raceWon = false;
        for (Command *cmd : commands)
        {
            cmd->setCurrentTime(now);
            cmd->initialize();
        }
    }

    void execute() override
    {
        if (raceWon)
            return;
        int now = pros::millis();
        for (int i = 0; i < (int)commands.size(); ++i)
        {
            if (finished[i])
                continue;
            commands[i]->execute();
            bool timedOut = commands[i]->getTimeout() > 0 && now >= commands[i]->getEndTime();
            if (commands[i]->isFinished() || timedOut)
            {
                commands[i]->end(timedOut);
                finished[i] = true;
                raceWon = true;
                break; // end() on the rest is handled by isFinished() → scheduler calls end()
            }
        }
    }

    void end(bool interrupted) override
    {
        for (int i = 0; i < (int)commands.size(); ++i)
        {
            if (!finished[i])
            {
                commands[i]->end(interrupted);
                finished[i] = true;
            }
        }
    }

    bool isFinished() const override
    {
        return raceWon;
    }

private:
    std::vector<Command *> commands;
    std::vector<bool> finished;
    bool raceWon;
};

// Does nothing and finishes immediately. Useful as a no-op placeholder.
class EmptyCommand : public Command
{
public:
    void initialize() override {}
    void execute() override {}
    void end(bool interrupted) override {}
    bool isFinished() const override { return true; }
};

// Evaluates a condition at initialization time and runs one of two commands.
// Takes ownership of both branch commands.
class ConditionalCommand : public Command
{
public:
    ConditionalCommand(std::function<bool()> condition, Command *onTrue, Command *onFalse)
        : condition(condition), onTrue(onTrue), onFalse(onFalse), selected(nullptr)
    {
        for (int req : onTrue->getRequirements())
            requirements.push_back(req);
        for (int req : onFalse->getRequirements())
            requirements.push_back(req);
    }

    ~ConditionalCommand()
    {
        delete onTrue;
        delete onFalse;
    }

    void initialize() override
    {
        selected = condition() ? onTrue : onFalse;
        selected->setCurrentTime(pros::millis());
        selected->initialize();
    }

    void execute() override
    {
        if (selected)
            selected->execute();
    }

    void end(bool interrupted) override
    {
        if (selected)
            selected->end(interrupted);
    }

    bool isFinished() const override
    {
        if (!selected)
            return true;
        bool timedOut = selected->getTimeout() > 0 && pros::millis() >= selected->getEndTime();
        return selected->isFinished() || timedOut;
    }

private:
    std::function<bool()> condition;
    Command *onTrue;
    Command *onFalse;
    Command *selected;
};

#endif
