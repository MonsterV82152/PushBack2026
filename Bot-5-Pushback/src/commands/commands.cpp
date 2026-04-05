#include "commands/commands.hpp"

Command *Commands::run(std::function<void()> execute, std::initializer_list<int> requirements)
{
    class RunCommand : public Command
    {
    public:
        RunCommand(std::function<void()> execute, std::initializer_list<int> requirements)
            : executeFunc(execute)
        {
            addRequirements(requirements);
        }
        void initialize() override {}
        void execute() override { executeFunc(); }
        void end(bool interrupted) override {}
        bool isFinished() const override { return false; }

    private:
        std::function<void()> executeFunc;
    };
    return new RunCommand(execute, requirements);
}

Command *Commands::runOnce(std::function<void()> execute, std::initializer_list<int> requirements)
{
    class RunOnceCommand : public Command
    {
    public:
        RunOnceCommand(std::function<void()> execute, std::initializer_list<int> requirements)
            : executeFunc(execute)
        {
            addRequirements(requirements);
        }
        void initialize() override { executeFunc(); }
        void execute() override {}
        void end(bool interrupted) override {}
        bool isFinished() const override { return true; }

    private:
        std::function<void()> executeFunc;
    };
    return new RunOnceCommand(execute, requirements);
}

Command *Commands::sequential(std::initializer_list<Command *> commands)
{
    return new SequentialCommandGroup(commands);
}

Command *Commands::parallel(std::initializer_list<Command *> commands)
{
    return new ParallelCommandGroup(commands);
}

Command *Commands::race(std::initializer_list<Command *> commands)
{
    return new RaceCommandGroup(commands);
}

Command *Commands::wait(int milliseconds)
{
    class WaitCommand : public Command
    {
    public:
        WaitCommand(int milliseconds)
        {
            timeout = milliseconds;
        }
        void initialize() override {}
        void execute() override {}
        void end(bool interrupted) override {}
        bool isFinished() const override { return false; }
    };
    return new WaitCommand(milliseconds);
}

Command *Commands::empty()
{
    return new EmptyCommand();
}

Command *Commands::conditional(std::function<bool()> condition, Command *onTrue, Command *onFalse)
{
    return new ConditionalCommand(condition, onTrue, onFalse);
}