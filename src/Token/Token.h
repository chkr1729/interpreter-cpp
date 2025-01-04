#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <unordered_set>

enum class TokenType
{
    Whitespace,
    Comment,
    MultiCharToken,
    SingleCharToken,
    StringLiteral,
    NumberLiteral,
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
    int                getLineNumber() const { return lineNumber; }
    const std::string& getLiteral() const { return literal; }
    bool               hasError() const { return error; }

    // Check if the token's lexeme contains a newline character
    bool hasNewLine() const { return lexeme.find('\n') != std::string::npos; }

    bool isBooleanLiteral() const { return booleanLiterals.find(lexeme) != booleanLiterals.end(); }
    bool isUnaryOperator() const { return unaryOperators.find(lexeme) != unaryOperators.end(); }

    size_t size() const { return lexeme.size(); }

   private:
    // State Variable
    TokenType   type;
    std::string lexeme;
    std::string literal;
    int         lineNumber;
    bool        error;

    inline static const std::unordered_set<std::string> booleanLiterals = {"true", "false", "nil"};
    inline static const std::unordered_set<std::string> unaryOperators  = {"!", "-"};
};

#endif  // TOKEN_H
