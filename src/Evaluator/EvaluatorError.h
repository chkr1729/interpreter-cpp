#pragma once
#include <stdexcept>
#include <string>

class EvaluatorError : public std::runtime_error
{
   public:
    EvaluatorError(const std::string& message) : std::runtime_error(message) {}
};
