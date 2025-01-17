#include "CommandLineArgs.h"

#include <iostream>
#include <unordered_set>

const std::unordered_set<std::string> validCommands = {"tokenize", "parse", "evaluate", "run"};

CommandLineArgs::CommandLineArgs(int argc, char* argv[]) : argc(argc), argv(argv) {}

bool CommandLineArgs::validateArgs() const
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./your_program <command> <argument>" << std::endl;
        return false;
    }

    const std::string command = argv[1];
    if (validCommands.find(command) == validCommands.end())
    {
        std::cerr << "Unknown command: " << command << std::endl;
        return false;
    }

    return true;
}

std::string CommandLineArgs::getCommand() const
{
    return argv[1];
}

std::string CommandLineArgs::getArgument() const
{
    return argv[2];
}
