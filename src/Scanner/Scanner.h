#pragma once
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Token/Token.h"

class Scanner
{
   public:
    // Type alias for decimal parts (before and after decimal point)
    using DecimalParts = std::pair<std::string, std::optional<std::string>>;

    // Constructor
    Scanner(const std::string& fileName);

    // Main processing function
    void process();

    void print();
    void print(Token);

    // Get the result
    int getRetVal() const { return retVal; }

    // Move the tokens instead of copying
    std::vector<Token> getTokens() { return std::move(tokens); }

   private:
    // State variables
    int index   = 0;
    int lineNum = 1;
    int retVal  = 0;

    const std::string  fileContents;
    std::vector<Token> tokens;

    bool isWhiteSpaceToken() const;
    bool isCommentToken() const;
    bool isMultiCharToken(size_t size) const;
    bool isSingleCharToken() const;
    bool isStringLiteralToken() const { return fileContents[index] == '"'; }
    bool isNumberLiteralToken() const { return std::isdigit(fileContents[index]); }
    bool isWordToken() const;

    // TODO: Try to move these functions to StringUtils
    std::string  extractWordToken() const;
    DecimalParts getBeforeAfterDecimalStrings() const;

    Token getCommentToken() const;
    Token getStringLiteralToken() const;
    Token getNumberLiteralToken() const;
    Token getIdentifierAndReservedWordToken() const;
    Token getToken() const;

    // Token maps
    // TODO: Combine these maps

    inline static const std::unordered_set<std::string> booleanLiterals = {"true", "false", "nil"};

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
};
