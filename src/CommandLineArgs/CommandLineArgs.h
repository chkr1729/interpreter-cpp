#pragma once
#include <string>

class CommandLineArgs
{
   public:
    CommandLineArgs(int argc, char* argv[]);

    bool validateArgs() const;

    std::string getCommand() const;

    std::string getArgument() const;

   private:
    int    argc;
    char** argv;
};
