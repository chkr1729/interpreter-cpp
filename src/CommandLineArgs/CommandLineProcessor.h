
#ifndef COMMANDLINEPROCESSOR_H
#define COMMANDLINEPROCESSOR_H

#include <fstream>
#include <iostream>

enum class CommandLineStatus
{
    SUCCESS,
    INVALID_USAGE,
    UNKNOWN_COMMAND,
    FILE_NOT_FOUND
};

CommandLineStatus validateCommandLineArgs(int argc, char* argv[]);
void              logCommandLineError(CommandLineStatus status, char* argv[]);

#endif  // COMMANDLINEPROCESSOR_H
