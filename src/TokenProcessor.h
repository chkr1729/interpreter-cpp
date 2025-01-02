#ifndef TOKEN_PROCESSOR_H
#define TOKEN_PROCESSOR_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
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

class TokenProcessor
{
   private:
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

    // State variables
    int         index     = 0;
    int         lineNum   = 1;
    int         retVal    = 0;
    std::string curLexeme = "";

    const std::string fileContents;

    // Helper functions

    static std::string readFileContents(const std::string& fileName);
    static void        removeTrailingZeros(std::string& str);

    TokenType   identifyTokenType() const;
    std::string extractWordToken() const;

    void processToken();
    void processWhitespaceAndNewlines();
    void processComment();
    void processMultiCharToken(int tokeLen = 2);
    void processSingleCharToken();
    void processUnexpectedChar();
    void processStringLiteral();
    void processNumberLiteral();
    void processWordToken();
    void processIdentifier();
    void processReservedWord();

   public:
    // Constructor
    TokenProcessor() = default;
    TokenProcessor(const std::string& fileName);

    // Main processing function
    void process();

    // Get the result
    int getRetVal() const { return retVal; }
};

#endif  // TOKEN_PROCESSOR_H
