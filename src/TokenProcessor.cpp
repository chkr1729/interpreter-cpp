#include "TokenProcessor.h"

#include <cctype>
#include <regex>

// Helper function to read file contents
std::string TokenProcessor::readFileContents(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Error reading file: " << fileName << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}
// Constructor
TokenProcessor::TokenProcessor(const std::string& fileName)
    : fileContents(readFileContents(fileName))  // Initialize fileContents
{
}

TokenType TokenProcessor::identifyTokenType() const
{
    char c = fileContents[index];

    // Check for whitespace or newline
    if (c == ' ' || c == '\t' || c == '\n')
    {
        return TokenType::Whitespace;
    }

    // Check for single-line comments
    if (c == '/' && index + 1 < fileContents.size() && fileContents[index + 1] == '/')
    {
        return TokenType::Comment;
    }

    // Check for multi-character tokens
    if (index + 1 < fileContents.size())
    {
        std::string multiToken = std::string(1, c) + fileContents[index + 1];
        if (multiTokenMap.find(multiToken) != multiTokenMap.end())
        {
            return TokenType::MultiCharToken;
        }
    }

    // Check for single-character tokens
    if (tokenMap.find(c) != tokenMap.end())
    {
        return TokenType::SingleCharToken;
    }

    // Check for string literals
    if (c == '"')
    {
        return TokenType::StringLiteral;
    }

    // Check for number literals
    if (std::isdigit(c))
    {
        return TokenType::NumberLiteral;
    }

    // Check for identifiers
    if (std::isalpha(c) || c == '_')
    {
        return TokenType::Identifier;
    }

    // If none of the above, it's an unexpected character
    return TokenType::Unexpected;
}

// Handle whitespace and newlines
void TokenProcessor::processWhitespaceAndNewlines()
{
    char c    = fileContents[index];
    curLexeme = c;
    index++;
    if (c == '\n')
    {
        lineNum++;
    }
}

// Handle single-line comments (//)
void TokenProcessor::processComment()
{
    while (index < fileContents.size() && fileContents[index] != '\n')
    {
        curLexeme += fileContents[index++];
    }
}

// Handle multi-character tokens
void TokenProcessor::processMultiCharToken(int tokenLen)
{
    std::string multiToken = fileContents.substr(index, tokenLen);
    index += tokenLen;
    curLexeme += multiToken;
    std::cout << multiTokenMap.at(multiToken) << std::endl;
}

// Handle single-character tokens
void TokenProcessor::processSingleCharToken()
{
    const char c = fileContents[index++];
    curLexeme += c;
    std::cout << tokenMap.at(c) << std::endl;
}

void TokenProcessor::processStringLiteral()
{
    curLexeme += fileContents[index];
    int endIndex = index + 1;
    while (endIndex < fileContents.size() && fileContents[endIndex] != '"' &&
           fileContents[endIndex] != '\n')
    {
        curLexeme += fileContents[endIndex++];
    }
    const char c = fileContents[endIndex];
    curLexeme += c;
    index = endIndex + 1;
    if (c == '"')
    {
        std::cout << "STRING " << curLexeme << " " << curLexeme.substr(1, curLexeme.size() - 2)
                  << std::endl;
    }
    else
    {
        std::cerr << "[line " << lineNum << "] Error: Unterminated string." << std::endl;
        retVal = 65;
    }
    if (c == '\n')
    {
        lineNum++;
    }
}

void TokenProcessor::removeTrailingZeros(std::string& str)
{
    str = std::regex_replace(str, std::regex("\\.?0+$"), "");
}

void TokenProcessor::processNumberLiteral()
{
    // Lambda function to collect digits
    auto collectDigits = [&](std::string& target) {
        while (std::isdigit(fileContents[index]))
        {
            target += fileContents[index++];
        }
    };

    std::string beforeDecimalStr, afterDecimalStr;

    // Collect digits before the decimal point
    collectDigits(beforeDecimalStr);
    curLexeme = beforeDecimalStr;

    // Handle the decimal point and digits after it
    if (fileContents[index] == '.')
    {
        curLexeme += fileContents[index++];  // Append the decimal point
        collectDigits(afterDecimalStr);      // Collect digits after the decimal point
        curLexeme += afterDecimalStr;
    }
    removeTrailingZeros(afterDecimalStr);
    if (afterDecimalStr.empty())
    {
        afterDecimalStr = "0";
    }
    std::cout << "NUMBER " << curLexeme << " " << beforeDecimalStr << "." << afterDecimalStr
              << std::endl;
}

void TokenProcessor::processIdentifier()
{
    char c = fileContents[index];
    while (std::isalnum(c) || c == '_')
    {
        curLexeme += c;
        index++;
        c = fileContents[index];
    }
    std::cout << "IDENTIFIER " << curLexeme << " null" << std::endl;
}

// Handle unexpected characters
void TokenProcessor::processUnexpectedChar()
{
    curLexeme = fileContents[index++];
    std::cerr << "[line " << lineNum << "] Error: Unexpected character: " << curLexeme << std::endl;
    retVal = 65;
}

void TokenProcessor::processToken()
{
    TokenType type = identifyTokenType();

    switch (type)
    {
        case TokenType::Whitespace:
            processWhitespaceAndNewlines();
            break;
        case TokenType::Comment:
            processComment();
            break;
        case TokenType::MultiCharToken:
            processMultiCharToken();
            break;
        case TokenType::SingleCharToken:
            processSingleCharToken();
            break;
        case TokenType::StringLiteral:
            processStringLiteral();
            break;
        case TokenType::NumberLiteral:
            processNumberLiteral();
            break;
        case TokenType::Identifier:
            processIdentifier();
            break;
        case TokenType::Unexpected:
            processUnexpectedChar();
            break;
    }
}

// Main processing function
void TokenProcessor::process()
{
    while (index < fileContents.size())
    {
        curLexeme.clear();
        processToken();
    }
    std::cout << "EOF  null" << std::endl;
}
