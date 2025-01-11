#include <iostream>

#include "CommandLineArgs/CommandLineProcessor.h"
#include "Evaluator/Evaluator.h"
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

    Parser parser(scanner.getTokens());

    auto expression = parser.parse();
    if (!expression)
    {
        std::cerr << "Parsing failed due to errors." << std::endl;
        return 65;
    }

    if (command == "parse")
    {
        PrintVisitor printer;
        expression->accept(printer);
        return 0;
    }

    if (command == "evaluate")
    {
        Evaluator evaluator;
        expression->accept(evaluator);
        evaluator.printResult();
    }

    return 0;
}
