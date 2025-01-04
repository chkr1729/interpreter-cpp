#ifndef COMMAND_LINE_PROCESSOR_H
#define COMMAND_LINE_PROCESSOR_H

#include <string>

class CommandLineProcessor
{
   public:
    // Constructor
    CommandLineProcessor(int argc, char* argv[]);

    // Returns true if the arguments are valid, false otherwise
    bool validateArgs() const;

    // Get the command (e.g., "tokenize" or "parse")
    std::string getCommand() const;

    // Get the argument (e.g., filename for "tokenize" or literal for "parse")
    std::string getArgument() const;

   private:
    int    argc;
    char** argv;
};

#endif  // COMMAND_LINE_PROCESSOR_H
