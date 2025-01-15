#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

// #TODO: Add TokenGroup for better grouping like Literal, Operator, Whitespaces etc.

enum class TokenCategory
{
    Space,
    Operator,
    Literal,
    Word,
    Unexpected
};

enum class TokenType
{
    // Space
    Whitespace,
    Comment,

    // Operator
    MultiCharOperator,
    SingleCharOperator,

    // Literal
    StringLiteral,
    NumberLiteral,
    BooleanLiteral,
    NilLiteral,

    // Word
    Identifier,
    ReservedWord,

    // Unexpected
    Unexpected
};

class Token
{
   public:
    Token(TokenCategory      category,
          TokenType          type,
          const std::string& lexeme,
          const std::string& literal,
          int                lineNumber,
          bool               error)
        : category(category),
          type(type),
          lexeme(lexeme),
          literal(literal),
          lineNumber(lineNumber),
          error(error)
    {
    }

    // Accessors
    TokenCategory      getCategory() const { return category; }
    TokenType          getType() const { return type; }
    const std::string& getLexeme() const { return lexeme; }
    const std::string& getLiteral() const { return literal; }
    int                getLineNumber() const { return lineNumber; }
    bool               hasError() const { return error; }

    // Count the number of newline characters in the token's lexeme
    int countNewLines() const { return std::count(lexeme.begin(), lexeme.end(), '\n'); }

    size_t size() const { return lexeme.size(); }

   private:
    // State Variable
    TokenCategory category;
    TokenType     type;
    std::string   lexeme;
    std::string   literal;
    int           lineNumber;
    bool          error;
};
