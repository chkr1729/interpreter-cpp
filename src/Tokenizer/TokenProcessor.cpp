#include "TokenProcessor.h"

#include <cassert>
#include <cctype>
#include <cstdlib>

#include "../Utils/FileUtils.h"
#include "../Utils/StringUtils.h"

// Constructor
TokenProcessor::TokenProcessor(const std::string& fileName)
    : fileContents(readFile(fileName))  // Initialize fileContents
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

TokenProcessor::DecimalParts TokenProcessor::getBeforeAfterDecimalStrings() const
{
    std::string                beforeDecimal;
    std::optional<std::string> afterDecimal;
    size_t                     localIndex = index;  // Use a local index variable

    // Lambda to collect digits into the given string
    auto collectDigits = [&](std::string& target) {
        while (localIndex < fileContents.size() && std::isdigit(fileContents[localIndex]))
        {
            target += fileContents[localIndex++];
        }
    };

    collectDigits(beforeDecimal);  // Collect digits before the decimal point

    if (localIndex < fileContents.size() && fileContents[localIndex] == '.')
    {
        localIndex++;  // Skip the decimal point
        std::string afterDecimalStr;
        collectDigits(afterDecimalStr);  // Collect digits after the decimal point

        if (!afterDecimalStr.empty())
        {
            afterDecimal = afterDecimalStr;  // Assign only if non-empty
        }
    }

    return {beforeDecimal, afterDecimal};
}

Token TokenProcessor::getNumberLiteralToken() const
{
    std::string lexeme;

    auto [beforeDecimalStr, afterDecimal] = getBeforeAfterDecimalStrings();

    lexeme += beforeDecimalStr;

    if (afterDecimal)
    {
        lexeme += ("." + *afterDecimal);
    }

    return Token(TokenType::NumberLiteral, lexeme, formatNumberLiteral(lexeme), lineNum, false);
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
