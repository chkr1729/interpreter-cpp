#include "Scanner.h"

#include <cassert>
#include <cctype>
#include <cstdlib>

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
    return {TokenType::Comment,
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
        endIndex = fileContents.size();
        return Token(TokenType::StringLiteral,
                     fileContents.substr(index, endIndex - index),
                     fileContents.substr(index + 1, endIndex - index - 1),
                     lineNum,
                     true);
    }

    return Token(TokenType::StringLiteral,
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

    return Token(TokenType::NumberLiteral, lexeme, formatNumberLiteral(lexeme), lineNum, false);
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
    if (reservedWords.find(word) != reservedWords.end())
    {
        if (booleanLiterals.find(word) != booleanLiterals.end())
        {
            return Token(TokenType::BooleanLiteral, word, "null", lineNum, false);
        }
        else if (word == "nil")
        {
            return Token(TokenType::NilLiteral, word, "null", lineNum, false);
        }
        return Token(TokenType::ReservedWord, word, "null", lineNum, false);
    }
    return Token(TokenType::Identifier, word, "null", lineNum, false);
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
        if (multiTokenMap.find(fileContents.substr(index, size)) != multiTokenMap.end())
        {
            return true;
        }
    }
    return false;
}

bool Scanner::isSingleCharToken() const
{
    return tokenMap.find(fileContents[index]) != tokenMap.end();
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

    // word = identifier + reserved word(including boolean literals)
    if (isWordToken())
    {
        return getIdentifierAndReservedWordToken();
    }

    // If none of the above, it's an unexpected character
    return Token(TokenType::Unexpected, std::string(1, fileContents[index]), "null", lineNum, true);
}

// Main processing function
void Scanner::process()
{
    while (index < fileContents.size())
    {
        auto token     = getToken();
        auto tokenType = token.getType();
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
        // We will not add whitespace or comment tokens
        if (tokenType == TokenType::Whitespace || tokenType == TokenType::Comment)
        {
            continue;
        }
        tokens.emplace_back(token);
    }
}

void Scanner::print()
{
    for (auto token : tokens)
    {
        print(token);
    }
    std::cout << "EOF  null" << std::endl;
}

void Scanner::print(Token token)
{
    switch (token.getType())
    {
        case TokenType::Whitespace:
        case TokenType::Comment:
            break;
        case TokenType::MultiCharToken:
            std::cout << multiTokenMap.at(token.getLexeme()) << std::endl;
            break;
        case TokenType::SingleCharToken:
            std::cout << tokenMap.at(token.getLexeme().front()) << std::endl;
            break;
        case TokenType::StringLiteral:
            if (token.hasError())
            {
                std::cerr << "[line " << token.getLineNumber() << "] Error: Unterminated string."
                          << std::endl;
            }
            else
            {
                std::cout << "STRING " << token.getLexeme() << " " << token.getLiteral()
                          << std::endl;
            }
            break;
        case TokenType::NumberLiteral:
            std::cout << "NUMBER " << token.getLexeme() << " " << token.getLiteral() << std::endl;
            break;
        case TokenType::Identifier:
            std::cout << "IDENTIFIER " << token.getLexeme() << " " << token.getLiteral()
                      << std::endl;
            break;
        case TokenType::BooleanLiteral:
        case TokenType::NilLiteral:
        case TokenType::ReservedWord:
            std::cout << reservedWords.at(token.getLexeme()) << " " << token.getLexeme() << " "
                      << token.getLiteral() << std::endl;
            break;
        case TokenType::Unexpected:
            std::cerr << "[line " << token.getLineNumber()
                      << "] Error: Unexpected character: " << token.getLexeme() << std::endl;
            break;
        default:
            std::cerr << "Unknown" << std::endl;
            break;
    }
}
