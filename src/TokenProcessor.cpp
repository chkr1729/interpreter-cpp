#include "TokenProcessor.h"

#include <cctype>

TokenProcessor::TokenProcessor(const std::string& fileName)
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

    fileContents = buffer.str();
}

bool TokenProcessor::canProcess() const
{
    return curLexeme.empty();
}

// Handle whitespace and newlines
void TokenProcessor::handleWhitespaceAndNewlines()
{
    if (!canProcess())
    {
        return;
    }
    char c = fileContents[index];
    if (c != ' ' && c != '\t' && c != '\n')
    {
        return;
    }
    curLexeme = c;
    index++;
    if (c == '\n')
    {
        lineNum++;
    }
}

// Handle single-line comments (//)
void TokenProcessor::handleComment()
{
    if (!canProcess() || index + 1 >= fileContents.size() || fileContents[index] != '/' ||
        fileContents[index + 1] != '/')
    {
        return;
    }

    while (index < fileContents.size() && fileContents[index] != '\n')
    {
        curLexeme += fileContents[index++];
    }
}

// Handle multi-character tokens
void TokenProcessor::handleMultiCharToken()
{
    if (!canProcess() || index + 1 >= fileContents.size())
    {
        return;
    }
    std::string multiToken = std::string(1, fileContents[index]) + fileContents[index + 1];
    auto        it         = multiTokenMap.find(multiToken);
    if (it == multiTokenMap.end())
    {
        return;
    }
    index += 2;
    curLexeme += multiToken;
    std::cout << it->second << std::endl;
}

// Handle single-character tokens
void TokenProcessor::handleSingleCharToken()
{
    if (!canProcess())
    {
        return;
    }
    auto it = tokenMap.find(fileContents[index]);
    if (it == tokenMap.end())
    {
        return;
    }
    curLexeme += fileContents[index++];
    std::cout << it->second << std::endl;
}

void TokenProcessor::handleStringLiteral()
{
    if (!canProcess() || fileContents[index] != '"')
    {
        return;
    }
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

void TokenProcessor::handleNumberLiteral()
{
    if (!canProcess() || !std::isdigit(fileContents[index]))
    {
        return;
    }
    std::string beforeDecimalStr, afterDecimalStr;
    while (std::isdigit(fileContents[index]))
    {
        beforeDecimalStr += fileContents[index++];
    }
    curLexeme = beforeDecimalStr;
    if (fileContents[index] == '.')
    {
        curLexeme += fileContents[index++];
        while (std::isdigit(fileContents[index]))
        {
            afterDecimalStr += fileContents[index++];
        }
        curLexeme += afterDecimalStr;
    }
    if (afterDecimalStr.empty())
    {
        afterDecimalStr = "0";
    }
    std::cout << "NUMBER " << curLexeme << " " << beforeDecimalStr << "."
              << std::stof(afterDecimalStr) << std::endl;
}

void TokenProcessor::handleIdentifier()
{
    char c = fileContents[index];
    if (!canProcess() || !(std::isalpha(c) || c == '_'))
    {
        return;
    }
    while (std::isalnum(c) || c == '_')
    {
        curLexeme += c;
        index++;
        c = fileContents[index];
    }
    std::cout << "IDENTIFIER " << curLexeme << " null" << std::endl;
}

// Handle unexpected characters
void TokenProcessor::handleUnexpectedChar()
{
    if (!canProcess())
    {
        return;
    }
    curLexeme = fileContents[index++];
    std::cerr << "[line " << lineNum << "] Error: Unexpected character: " << curLexeme << std::endl;
    retVal = 65;
}

void TokenProcessor::resetLexeme()
{
    curLexeme.clear();
}

// Main processing function
void TokenProcessor::process()
{
    while (index < fileContents.size())
    {
        resetLexeme();
        // The order is important -  handleMultiCharToken should come before handleSingleCharToken
        handleWhitespaceAndNewlines();
        handleComment();
        handleMultiCharToken();
        handleSingleCharToken();
        handleStringLiteral();
        handleNumberLiteral();
        handleIdentifier();
        handleUnexpectedChar();
    }
    std::cout << "EOF  null" << std::endl;
}
