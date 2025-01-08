#include <iostream>

#include "CommandLineArgs/CommandLineProcessor.h"
#include "Parser/Parser.h"
#include "Parser/PrintVisitor.h"
#include "Scanner/Scanner.h"

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

    Scanner scanner(argument);
    scanner.process();

    if (command == "tokenize")
    {
        scanner.print();
        return scanner.getRetVal();
    }
    else if (command == "parse")
    {
        Parser parser(scanner.getTokens());
        auto   expression = parser.parse();
        if (expression)
        {
            PrintVisitor printer;
            expression->accept(printer);
            return 0;
            // return parser.getRetVal();
        }
        std::cerr << "Parsing failed due to errors." << std::endl;
        return 65;
    }

    return 0;
}
