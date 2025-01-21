#include "Scanner.h"

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <vector>

#include "../Parser/ParserError.h"
#include "../Token/TokenData.h"
#include "../Utils/FileUtils.h"
#include "../Utils/StringUtils.h"

// Constructor
Scanner::Scanner(const std::string& fileName)
    : fileContents(readFile(fileName))  // Initialize fileContents
{
}

Token Scanner::getCommentToken() const
{
    std::string lexeme;
    size_t      endIndex = fileContents.find_first_of('\n', index + 1);
    if (endIndex == std::string::npos)
    {
        endIndex = fileContents.size();
    }
    return {TokenCategory::Space,
            TokenType::Comment,
            fileContents.substr(index, endIndex + 1 - index),
            "null",
            lineNum,
            false};
}

Token Scanner::getStringLiteralToken() const
{
    size_t startIndex = index + 1;
    size_t endIndex   = fileContents.find_first_of("\"", startIndex);

    if (endIndex == std::string::npos)
    {
        // throw ParserError("Unterminated string.", lineNum);
        endIndex = fileContents.size();
        return Token(TokenCategory::Literal,
                     TokenType::StringLiteral,
                     fileContents.substr(index, endIndex - index),
                     fileContents.substr(index + 1, endIndex - index - 1),
                     lineNum,
                     true);
    }

    return Token(TokenCategory::Literal,
                 TokenType::StringLiteral,
                 fileContents.substr(index, endIndex - index + 1),
                 fileContents.substr(index + 1, endIndex - index - 1),
                 lineNum,
                 false);
}

Scanner::DecimalParts Scanner::getBeforeAfterDecimalStrings() const
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

Token Scanner::getNumberLiteralToken() const
{
    std::string lexeme;

    auto [beforeDecimalStr, afterDecimal] = getBeforeAfterDecimalStrings();

    lexeme += beforeDecimalStr;

    if (afterDecimal)
    {
        lexeme += ("." + *afterDecimal);
    }

    return Token(TokenCategory::Literal,
                 TokenType::NumberLiteral,
                 lexeme,
                 formatNumberLiteral(lexeme),
                 lineNum,
                 false);
}

std::string Scanner::extractWordToken() const
{
    size_t endIndex = index;

    while (endIndex < fileContents.size() &&
           (std::isalnum(fileContents[endIndex]) || fileContents[endIndex] == '_'))
    {
        endIndex++;
    }

    return fileContents.substr(index, endIndex - index);
}

Token Scanner::getIdentifierAndReservedWordToken() const
{
    std::string word = extractWordToken();
    if (TokenData::booleanLiterals.find(word) != TokenData::booleanLiterals.end())
    {
        return Token(
            TokenCategory::Literal, TokenType::BooleanLiteral, word, "null", lineNum, false);
    }
    if (word == "nil")
    {
        return Token(TokenCategory::Literal, TokenType::NilLiteral, word, "null", lineNum, false);
    }
    if (TokenData::reservedWords.find(word) != TokenData::reservedWords.end())
    {
        return Token(TokenCategory::Word, TokenType::ReservedWord, word, "null", lineNum, false);
    }
    return Token(TokenCategory::Word, TokenType::Identifier, word, "null", lineNum, false);
}

bool Scanner::isWhiteSpaceToken() const
{
    char c = fileContents[index];
    return (c == ' ' || c == '\t' || c == '\n');
}

bool Scanner::isCommentToken() const
{
    return (fileContents[index] == '/' && index + 1 < fileContents.size() &&
            fileContents[index + 1] == '/');
}

bool Scanner::isMultiCharToken(size_t size) const
{
    if (index + 1 < fileContents.size())
    {
        if (TokenData::multiTokenMap.find(fileContents.substr(index, size)) !=
            TokenData::multiTokenMap.end())
        {
            return true;
        }
    }
    return false;
}

bool Scanner::isSingleCharToken() const
{
    return TokenData::tokenMap.find(fileContents[index]) != TokenData::tokenMap.end();
}

bool Scanner::isWordToken() const
{
    const char c = fileContents[index];
    return std::isalpha(c) || c == '_';
}

Token Scanner::getToken() const
{
    if (isWhiteSpaceToken())
    {
        return {TokenCategory::Space,
                TokenType::Whitespace,
                std::string(1, fileContents[index]),
                "null",
                lineNum,
                false};
    }

    if (isCommentToken())
    {
        return getCommentToken();
    }

    const size_t multiCharSize = 2;
    if (isMultiCharToken(multiCharSize))
    {
        return {TokenCategory::Operator,
                TokenType::MultiCharOperator,
                fileContents.substr(index, multiCharSize),
                "null",
                lineNum,
                false};
    }

    if (isSingleCharToken())
    {
        return {TokenCategory::Operator,
                TokenType::SingleCharOperator,
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

    // word = identifier + reserved word(including boolean literals)
    if (isWordToken())
    {
        return getIdentifierAndReservedWordToken();
    }

    // If none of the above, it's an unexpected character
    return Token(TokenCategory::Unexpected,
                 TokenType::Unexpected,
                 std::string(1, fileContents[index]),
                 "null",
                 lineNum,
                 true);
}

std::vector<Token> Scanner::scan()
{
    std::vector<Token> tokens;
    while (index < fileContents.size())
    {
        auto token = getToken();

        assert(token.size() > 0);

        index += token.size();
        lineNum += token.countNewLines();

        if (token.hasError())
        {
            retVal = 65;
        }
        // We will not add whitespace or comment tokens
        if (token.getCategory() == TokenCategory::Space)
        {
            continue;
        }
        tokens.emplace_back(token);
    }
    return std::move(tokens);
}
