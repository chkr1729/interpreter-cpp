#ifndef TOKEN_PROCESSOR_H
#define TOKEN_PROCESSOR_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class TokenProcessor
{
   private:
    // Token maps
    const std::unordered_map<char, std::string> token_map = {{'(', "LEFT_PAREN ( null"},
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

    const std::unordered_map<std::string, std::string> multi_token_map = {
        {"==", "EQUAL_EQUAL == null"},
        {"!=", "BANG_EQUAL != null"},
        {"<=", "LESS_EQUAL <= null"},
        {">=", "GREATER_EQUAL >= null"}};

    // State variables
    int         index     = 0;
    int         lineNum   = 1;
    int         retVal    = 0;
    std::string curLexeme = "";

    std::string fileContents;

    // Helper functions
    bool canProcess() const;

    void resetLexeme();

    void handleWhitespaceAndNewlines();
    void handleComment();
    void handleMultiCharToken();
    void handleSingleCharToken();
    void handleUnexpectedChar();
    void handleStringLiteral();
    void handleNumberLiteral();
    void handleIdentifier();

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
