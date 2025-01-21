#pragma once
#include <stdexcept>
#include <string>

class ParserError : public std::runtime_error
{
   public:
    ParserError(const std::string& message, size_t lineNum)
        : std::runtime_error(message), lineNum(lineNum)
    {
    }

    size_t getLineNum() const { return lineNum; }

   private:
    size_t lineNum;
};
