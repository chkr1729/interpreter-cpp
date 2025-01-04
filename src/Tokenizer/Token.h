#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <unordered_map>

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

    // Token maps
    inline static const std::unordered_map<char, std::string> tokenMap = {
        {'(', "LEFT_PAREN ( null"},
        {')', "RIGHT_PAREN ) null"},
        {'{', "LEFT_BRACE { null"},
        {'}', "RIGHT_BRACE } null"},
        {'*', "STAR * null"},
        {'.', "DOT . null"},
        {',', "COMMA , null"},
        {'+', "PLUS + null"},
        {'-', "MINUS - null"},
        {';', "SEMICOLON ; null"},
        {'=', "EQUAL = null"},
        {'!', "BANG ! null"},
        {'<', "LESS < null"},
        {'>', "GREATER > null"},
        {'/', "SLASH / null"}};

    inline static const std::unordered_map<std::string, std::string> multiTokenMap = {
        {"==", "EQUAL_EQUAL == null"},
        {"!=", "BANG_EQUAL != null"},
        {"<=", "LESS_EQUAL <= null"},
        {">=", "GREATER_EQUAL >= null"}};

    inline static const std::unordered_map<std::string, std::string> reservedWords = {
        {"and", "AND"},
        {"class", "CLASS"},
        {"else", "ELSE"},
        {"false", "FALSE"},
        {"for", "FOR"},
        {"fun", "FUN"},
        {"if", "IF"},
        {"nil", "NIL"},
        {"or", "OR"},
        {"print", "PRINT"},
        {"return", "RETURN"},
        {"super", "SUPER"},
        {"this", "THIS"},
        {"true", "TRUE"},
        {"var", "VAR"},
        {"while", "WHILE"}};

    // Accessors
    TokenType          getType() const { return type; }
    const std::string& getLexeme() const { return lexeme; }
    int                getLineNumber() const { return lineNumber; }
    const std::string& getLiteral() const { return literal; }
    bool               hasError() const { return error; }

    // Overload << for consistent printing
    friend std::ostream& operator<<(std::ostream& os, const Token& token);

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

#endif  // TOKEN_H
