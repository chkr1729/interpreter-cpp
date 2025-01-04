#ifndef TOKEN_PROCESSOR_H
#define TOKEN_PROCESSOR_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

class TokenProcessor
{
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

    std::string extractWordToken() const;

    Token getCommentToken() const;
    Token getStringLiteralToken() const;
    Token getNumberLiteralToken() const;
    Token getIdentifierAndReservedWordToken() const;
    Token getToken() const;

   public:
    // Constructor
    TokenProcessor() = default;
    TokenProcessor(const std::string& fileName);

    // Main processing function
    void process();
    void print();

    // Get the result
    int getRetVal() const { return retVal; }
};

#endif  // TOKEN_PROCESSOR_H
