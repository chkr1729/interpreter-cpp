#include "TokenProcessor.h"

#include <cassert>
#include <cctype>
#include <cstdlib>
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

Token TokenProcessor::getCommentToken() const
{
    std::string lexeme;
    size_t      endIndex = fileContents.find_first_of('\n', index + 1);
    if (endIndex == std::string::npos)
    {
        endIndex = fileContents.size();
    }
    return {TokenType::Comment,
            fileContents.substr(index, endIndex + 1 - index),
            "null",
            lineNum,
            false};
}

Token TokenProcessor::getStringLiteralToken() const
{
    size_t startIndex = index + 1;
    size_t endIndex   = fileContents.find_first_of("\"\n", startIndex);

    if (endIndex == std::string::npos || fileContents[endIndex] == '\n')
    {
        return Token(TokenType::StringLiteral,
                     fileContents.substr(index, fileContents.size() - index),
                     "null",
                     lineNum,
                     true);
    }

    return Token(TokenType::StringLiteral,
                 fileContents.substr(index, endIndex - index + 1),
                 fileContents.substr(index + 1, endIndex - index - 1),
                 lineNum,
                 false);
}

void TokenProcessor::removeTrailingZeros(std::string& str)
{
    str = std::regex_replace(str, std::regex("\\.?0+$"), "");
}

Token TokenProcessor::getNumberLiteralToken() const
{
    std::string lexeme;
    size_t      tempIndex = index;

    auto collectDigits = [&](std::string& target) {
        while (std::isdigit(fileContents[tempIndex]))
        {
            target += fileContents[tempIndex++];
        }
    };

    std::string beforeDecimalStr, afterDecimalStr;

    // Collect digits before the decimal point
    collectDigits(beforeDecimalStr);
    lexeme += beforeDecimalStr;

    // Handle the decimal point and digits after it
    if (fileContents[tempIndex] == '.')
    {
        lexeme += fileContents[tempIndex++];  // Append the decimal point
        collectDigits(afterDecimalStr);       // Collect digits after the decimal point
        lexeme += afterDecimalStr;
    }
    removeTrailingZeros(afterDecimalStr);
    if (afterDecimalStr.empty())
    {
        afterDecimalStr = "0";
    }

    return Token(
        TokenType::NumberLiteral, lexeme, beforeDecimalStr + "." + afterDecimalStr, lineNum, false);
}

std::string TokenProcessor::extractWordToken() const
{
    size_t endIndex = index;

    while (endIndex < fileContents.size() &&
           (std::isalnum(fileContents[endIndex]) || fileContents[endIndex] == '_'))
    {
        endIndex++;
    }

    return fileContents.substr(index, endIndex - index);
}

Token TokenProcessor::getIdentifierAndReservedWordToken() const
{
    std::string word = extractWordToken();
    if (Token::reservedWords.find(word) != Token::reservedWords.end())
    {
        return Token(TokenType::ReservedWord, word, "null", lineNum, false);
    }
    return Token(TokenType::Identifier, word, "null", lineNum, false);
}

bool TokenProcessor::isWhiteSpaceToken() const
{
    char c = fileContents[index];
    return (c == ' ' || c == '\t' || c == '\n');
}

bool TokenProcessor::isCommentToken() const
{
    return (fileContents[index] == '/' && index + 1 < fileContents.size() &&
            fileContents[index + 1] == '/');
}

bool TokenProcessor::isMultiCharToken(size_t size) const
{
    if (index + 1 < fileContents.size())
    {
        if (Token::multiTokenMap.find(fileContents.substr(index, size)) !=
            Token::multiTokenMap.end())
        {
            return true;
        }
    }
    return false;
}

bool TokenProcessor::isSingleCharToken() const
{
    return Token::tokenMap.find(fileContents[index]) != Token::tokenMap.end();
}

bool TokenProcessor::isWordToken() const
{
    const char c = fileContents[index];
    return std::isalpha(c) || c == '_';
}

Token TokenProcessor::getToken() const
{
    if (isWhiteSpaceToken())
    {
        return {TokenType::Whitespace, std::string(1, fileContents[index]), "null", lineNum, false};
    }

    if (isCommentToken())
    {
        return getCommentToken();
    }

    const size_t multiCharSize = 2;
    if (isMultiCharToken(multiCharSize))
    {
        return {TokenType::MultiCharToken,
                fileContents.substr(index, multiCharSize),
                "null",
                lineNum,
                false};
    }

    if (isSingleCharToken())
    {
        return {TokenType::SingleCharToken,
                std::string(1, fileContents[index]),
                "null",
                lineNum,
                false};
    }

    if (isStringLiteralToken())
    {
        return getStringLiteralToken();
    }

    if (isNumberLiteralToken())
    {
        return getNumberLiteralToken();
    }

    // word = identifier + reserved word
    if (isWordToken())
    {
        return getIdentifierAndReservedWordToken();
    }

    // If none of the above, it's an unexpected character
    return Token(TokenType::Unexpected, std::string(1, fileContents[index]), "null", lineNum, true);
}

// Main processing function
void TokenProcessor::process()
{
    while (index < fileContents.size())
    {
        auto token = getToken();
        tokens.emplace_back(token);
        assert(token.size() > 0);
        index += token.size();
        if (token.hasNewLine())
        {
            lineNum++;
        }
        if (token.hasError())
        {
            retVal = 65;
        }
    }
}

void TokenProcessor::print()
{
    for (auto token : tokens)
    {
        if (token.hasError())
        {
            std::cerr << token;
        }
        else
        {
            std::cout << token;
        }
    }
    std::cout << "EOF  null" << std::endl;
}
