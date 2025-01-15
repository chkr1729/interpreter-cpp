#include "CommandLineProcessor.h"

#include <iostream>
#include <unordered_set>

// Allowed commands
const std::unordered_set<std::string> validCommands = {"tokenize", "parse", "evaluate", "run"};

// Constructor
CommandLineProcessor::CommandLineProcessor(int argc, char* argv[]) : argc(argc), argv(argv) {}

// Validate the command-line arguments
bool CommandLineProcessor::validateArgs() const
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

// Get the command
std::string CommandLineProcessor::getCommand() const
{
    return argv[1];
}

// Get the argument
std::string CommandLineProcessor::getArgument() const
{
    return argv[2];
}
