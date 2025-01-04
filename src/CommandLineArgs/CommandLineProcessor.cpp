
#include "CommandLineProcessor.h"

CommandLineStatus validateCommandLineArgs(int argc, char* argv[])
{
    if (argc < 3)
    {
        return CommandLineStatus::INVALID_USAGE;
    }

    const std::string command = argv[1];
    if (command != "tokenize")
    {
        return CommandLineStatus::UNKNOWN_COMMAND;
    }

    const std::string filename = argv[2];
    std::ifstream     file(filename);
    if (!file.is_open())
    {
        return CommandLineStatus::FILE_NOT_FOUND;
    }

    return CommandLineStatus::SUCCESS;
}

void logCommandLineError(CommandLineStatus status, char* argv[])
{
    switch (status)
    {
        case CommandLineStatus::INVALID_USAGE:
            std::cerr << "Usage: " << argv[0] << " tokenize <filename>" << std::endl;
            break;
        case CommandLineStatus::UNKNOWN_COMMAND:
            std::cerr << "Unknown command: " << argv[1] << std::endl;
            break;
        case CommandLineStatus::FILE_NOT_FOUND:
            std::cerr << "Error: File does not exist: " << argv[2] << std::endl;
            break;
        case CommandLineStatus::SUCCESS:
            // No error, nothing to log
            break;
    }
}
