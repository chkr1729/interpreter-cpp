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

    Scanner(const std::string& fileName);

    std::vector<Token> scan();

    int getRetVal() const { return retVal; }

   private:
    // State variables
    int index   = 0;
    int lineNum = 1;
    int retVal  = 0;

    const std::string fileContents;

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
};
