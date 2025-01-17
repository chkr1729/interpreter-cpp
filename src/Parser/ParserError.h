#pragma once
#include <stdexcept>
#include <string>

class ParserError : public std::runtime_error
{
   public:
    explicit ParserError(const std::string& message) : std::runtime_error(message) {}
};
