#include "Parser.h"

#include <iostream>
#include <sstream>
#include <unordered_set>

#include "../Utils/FileUtils.h"

// Allowed boolean literal values
const std::unordered_set<std::string> booleanLiterals = {"true", "false", "nil"};

// Constructor
Parser::Parser(const std::string& fileName) : fileContents(readFile(fileName)) {}

// Parse the file contents
void Parser::parse()
{
    std::istringstream stream(fileContents);
    std::string        word;

    // Process each word in the file
    while (stream >> word)
    {
        if (isBooleanLiteral(word))
        {
            handleBooleanLiteral(word);
        }
        else
        {
            std::cerr << "Error: Unsupported literal type: " << word << std::endl;
        }
    }
}

// Check if a word is a boolean literal
bool Parser::isBooleanLiteral(const std::string& word) const
{
    return booleanLiterals.find(word) != booleanLiterals.end();
}

// Handle and print boolean literals
void Parser::handleBooleanLiteral(const std::string& word) const
{
    std::cout << word << std::endl;
}
