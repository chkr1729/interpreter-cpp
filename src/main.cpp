#include "CommandLineArgs/CommandLineProcessor.h"
#include "Tokenizer/TokenProcessor.h"

int main(int argc, char* argv[])
{
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    CommandLineStatus status = validateCommandLineArgs(argc, argv);
    if (status != CommandLineStatus::SUCCESS)
    {
        logCommandLineError(status, argv);
        return static_cast<int>(status);  // Use status code as return value
    }

    TokenProcessor tokenProcessor(argv[2]);
    tokenProcessor.process();
    tokenProcessor.print();

    return tokenProcessor.getRetVal();
}
