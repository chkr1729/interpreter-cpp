#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <unordered_set>

class Parser
{
   public:
    // Constructor accepting a file name
    Parser(const std::string& fileName);

    // Parse the file contents and print the result
    void parse();

   private:
    const std::string fileContents;

    // Helper methods for parsing different literal types
    bool isBooleanLiteral(const std::string& word) const;
    void handleBooleanLiteral(const std::string& word) const;
};

#endif  // PARSER_H
