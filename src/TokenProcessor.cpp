#include "TokenProcessor.h"

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

// Handle whitespace and newlines
void TokenProcessor::handleWhitespaceAndNewlines()
{
    char c = fileContents[index];
    if (c == '\n')
    {
        lineNum++;
    }
    index++;
}

// Handle single-line comments (//)
bool TokenProcessor::handleComment()
{
    if (index + 1 >= fileContents.size() || fileContents[index] != '/' ||
        fileContents[index + 1] != '/')
    {
        return false;
    }

    while (index < fileContents.size() && fileContents[index] != '\n')
    {
        index++;
    }
    return true;
}

// Handle multi-character tokens
bool TokenProcessor::handleMultiCharToken()
{
    if (index + 1 >= fileContents.size())
    {
        return false;
    }
    std::string multi_token = std::string(1, fileContents[index]) + fileContents[index + 1];
    auto        it          = multi_token_map.find(multi_token);
    if (it != multi_token_map.end())
    {
        std::cout << it->second << std::endl;
        index += 2;
        return true;
    }
    return false;
}

// Handle single-character tokens
bool TokenProcessor::handleSingleCharToken()
{
    char c  = fileContents[index];
    auto it = token_map.find(c);
    if (it != token_map.end())
    {
        std::cout << it->second << std::endl;
        index++;
        return true;
    }
    return false;
}

bool TokenProcessor::handleStringLiteral()
{
    if (fileContents[index] != '"')
    {
        return false;
    }
    int endIndex = index + 1;
    while (endIndex < fileContents.size() && fileContents[endIndex] != '"' &&
           fileContents[endIndex] != '\n')
    {
        endIndex++;
    }
    if (fileContents[endIndex] == '"')
    {
        const std::string literal = fileContents.substr(index + 1, endIndex - index - 1);
        std::cout << "STRING " << "\"" << literal << "\" " << literal << std::endl;
    }
    else
    {
        std::cerr << "[line " << lineNum << "] Error: Unterminated string." << std::endl;
        retVal = 65;
    }
    if (fileContents[endIndex] == '\n')
    {
        lineNum++;
    }
    index = endIndex + 1;
    return true;
}

// Handle unexpected characters
void TokenProcessor::handleUnexpectedChar()
{
    const char c = fileContents[index];
    std::cerr << "[line " << lineNum << "] Error: Unexpected character: " << c << std::endl;
    retVal = 65;
    index++;
}

// Main processing function
void TokenProcessor::process()
{
    while (index < fileContents.size())
    {
        char c = fileContents[index];

        if (c == ' ' || c == '\t' || c == '\n')
        {
            handleWhitespaceAndNewlines();
            continue;
        }

        if (handleComment())
        {
            continue;
        }
        if (handleMultiCharToken())
        {
            continue;
        }
        if (handleSingleCharToken())
        {
            continue;
        }
        if (handleStringLiteral())
        {
            continue;
        }

        handleUnexpectedChar();
    }
    std::cout << "EOF  null" << std::endl;
}
