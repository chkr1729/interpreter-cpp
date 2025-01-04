#include <iostream>

#include "CommandLineArgs/CommandLineProcessor.h"
#include "Parser/Parser.h"
#include "Tokenizer/TokenProcessor.h"

int main(int argc, char* argv[])
{
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    CommandLineProcessor cmdProcessor(argc, argv);

    if (!cmdProcessor.validateArgs())
    {
        return 1;  // Return error code if arguments are invalid
    }

    const std::string command  = cmdProcessor.getCommand();
    const std::string argument = cmdProcessor.getArgument();

    if (command == "tokenize")
    {
        TokenProcessor tokenProcessor(argument);
        tokenProcessor.process();
        tokenProcessor.print();
        return tokenProcessor.getRetVal();
    }
    else if (command == "parse")
    {
        Parser parser(argument);
        parser.parse();
    }

    return 0;
}
