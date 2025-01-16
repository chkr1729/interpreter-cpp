#include <iostream>

#include "CommandLineArgs/CommandLineProcessor.h"
#include "Evaluator/Evaluator.h"
#include "Parser/Parser.h"
#include "Printer/Printer.h"
#include "Scanner/Scanner.h"
#include "Statement/Statement.h"

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

    // Step 1: Tokenize the input
    Scanner scanner(argument);
    scanner.process();

    if (command == "tokenize")
    {
        scanner.print();
        return scanner.getRetVal();
    }

    // Step 2: Parse the tokens into statements
    Parser parser(scanner.getTokens());

    auto statements = parser.parse();

    // If parsing failed, we exit
    int parserRetVal = parser.getRetVal();
    if (parserRetVal)
    {
        return parserRetVal;
    }

    // Step 3: Handle commands
    if (command == "parse")
    {
        Printer printer;
        for (const auto& statement : statements)
        {
            statement->accept(printer);
        }
        return 0;
    }

    if (command == "evaluate" || command == "run")
    {
        Evaluator evaluator;
        for (const auto& statement : statements)
        {
            statement->accept(evaluator);
        }
    }

    return 0;
}
