#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

// #TODO: Add TokenGroup for better grouping like Literal, Operator, Whitespaces etc.
enum class TokenType
{
    Whitespace,
    Comment,
    MultiCharToken,
    SingleCharToken,
    StringLiteral,
    NumberLiteral,
    BooleanLiteral,
    Identifier,
    ReservedWord,
    Unexpected
};

class Token
{
   public:
    Token(TokenType          type,
          const std::string& lexeme,
          const std::string& literal,
          int                lineNumber,
          bool               error)
        : type(type), lexeme(lexeme), literal(literal), lineNumber(lineNumber), error(error)
    {
    }

    // Accessors
    TokenType          getType() const { return type; }
    const std::string& getLexeme() const { return lexeme; }
    const std::string& getLiteral() const { return literal; }
    int                getLineNumber() const { return lineNumber; }
    bool               hasError() const { return error; }

    // Check if the token's lexeme contains a newline character
    bool hasNewLine() const { return lexeme.find('\n') != std::string::npos; }

    size_t size() const { return lexeme.size(); }

   private:
    // State Variable
    TokenType   type;
    std::string lexeme;
    std::string literal;
    int         lineNumber;
    bool        error;
};
