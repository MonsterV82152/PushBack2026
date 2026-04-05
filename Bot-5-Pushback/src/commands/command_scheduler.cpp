#include "commands/command_scheduler.hpp"

int CommandScheduler::getCurrentTime()
{
    return pros::millis();
}

// Internal helper — only called from within run(), no locking needed.
// Handles conflict cancellation, initialization, and insertion into scheduledCommands.
static void scheduleInternal(Command *command)
{
    command->setCurrentTime(CommandScheduler::getCurrentTime());

    // Cancel and erase any currently running commands that share a subsystem
    for (auto it = CommandScheduler::scheduledCommands.begin(); it != CommandScheduler::scheduledCommands.end();)
    {
        Command *existing = *it;
        bool conflicts = false;
        for (int req : existing->getRequirements())
        {
            const auto &incoming = command->getRequirements();
            if (std::find(incoming.begin(), incoming.end(), req) != incoming.end())
            {
                conflicts = true;
                break;
            }
        }
        if (conflicts)
        {
            existing->end(true);
            // Don't delete default commands — they are not owned by the scheduler
            bool isDefault = false;
            for (const auto &[defaultCmd, subsystem] : CommandScheduler::defaultCommands)
                if (existing == defaultCmd)
                {
                    isDefault = true;
                    break;
                }
            if (!isDefault)
                delete existing;
            it = CommandScheduler::scheduledCommands.erase(it);
        }
        else
        {
            ++it;
        }
    }

    CommandScheduler::scheduledCommands.push_back(command);
    command->initialize();
}

// Safe to call from any task. Pushes to the pending queue under a mutex;
// the run() loop will drain and process it at the start of the next tick.
void CommandScheduler::scheduleCommand(Command *command)
{
    mutex.take(TIMEOUT_MAX);
    pendingCommands.push_back(command);
    mutex.give();
}

void CommandScheduler::setDefaultCommand(Command *command, int subsystem)
{
    for (auto &[cmd, sub] : defaultCommands)
    {
        if (sub == subsystem)
        {
            // Remove old default from running commands if present (don't delete it yet)
            scheduledCommands.erase(
                std::remove(scheduledCommands.begin(), scheduledCommands.end(), cmd),
                scheduledCommands.end());
            delete cmd;
            cmd = command;
            scheduleCommand(command);
            return;
        }
    }
    defaultCommands.emplace_back(command, subsystem);
    scheduleCommand(command);
}

void CommandScheduler::run()
{
    // Drain the pending queue under the mutex — brief critical section
    std::vector<Command *> toSchedule;
    mutex.take(TIMEOUT_MAX);
    toSchedule.swap(pendingCommands);
    mutex.give();

    // Initialize and insert each pending command (conflict detection runs here)
    for (Command *cmd : toSchedule)
        scheduleInternal(cmd);

    // Execute all running commands
    int currentTime = getCurrentTime();
    for (auto it = scheduledCommands.begin(); it != scheduledCommands.end();)
    {
        Command *command = *it;
        command->execute();
        bool timedOut = command->getTimeout() > 0 && currentTime >= command->getEndTime();
        if (command->isFinished() || timedOut)
        {
            // Save requirements BEFORE deleting the command
            std::vector<int> reqs = command->getRequirements();
            command->end(timedOut);

            // Don't delete default commands — the scheduler doesn't own them
            bool isDefault = false;
            for (const auto &[defaultCmd, subsystem] : defaultCommands)
                if (command == defaultCmd)
                {
                    isDefault = true;
                    break;
                }
            if (!isDefault)
                delete command;

            it = scheduledCommands.erase(it);

            // Reschedule any default command whose subsystem was just freed
            for (const auto &[defaultCmd, subsystem] : defaultCommands)
            {
                if (std::find(reqs.begin(), reqs.end(), subsystem) != reqs.end())
                {
                    bool alreadyRunning = std::find(scheduledCommands.begin(),
                                                    scheduledCommands.end(),
                                                    defaultCmd) != scheduledCommands.end();
                    if (!alreadyRunning)
                        scheduleInternal(defaultCmd);
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
    pros::Task([]()
               {
                         while (true)
                         {
                             CommandScheduler::run();
                             pros::delay(20); // Run the scheduler every 20ms
                         } });
}