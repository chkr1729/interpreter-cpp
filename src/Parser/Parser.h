#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../Token/Token.h"

class Parser
{
   public:
    Parser(std::vector<Token>&& tokens);  // Rvalue reference;

    // Parse the file contents and print the result
    void parse();

   private:
    std::vector<Token> tokens;  // Stored by value, but moved in

    size_t index = 0;
};

#endif  // PARSER_H
