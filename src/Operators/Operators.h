#pragma once

#include <functional>
#include <stdexcept>
#include <unordered_map>

namespace Operators
{

// Handle arithmetic operators
inline const std::unordered_map<std::string, std::function<double(double, double)>> arithmeticOps =
    {{"+", [](double lhs, double rhs) { return lhs + rhs; }},
     {"-", [](double lhs, double rhs) { return lhs - rhs; }},
     {"*", [](double lhs, double rhs) { return lhs * rhs; }},
     {"/", [](double lhs, double rhs) {
          if (rhs == 0.0) throw std::runtime_error("Division by zero");
          return lhs / rhs;
      }}};

// Handle equality operators (templated)
template <typename T>
inline const std::unordered_map<std::string, std::function<bool(const T&, const T&)>> equalityOps =
    {
        {"==", [](const T& lhs, const T& rhs) { return lhs == rhs; }},
        {"!=", [](const T& lhs, const T& rhs) { return lhs != rhs; }},
};

// Handle relational operators
inline const std::unordered_map<std::string, std::function<bool(double, double)>> relationalOps = {
    {">", [](double lhs, double rhs) { return lhs > rhs; }},
    {"<", [](double lhs, double rhs) { return lhs < rhs; }},
    {">=", [](double lhs, double rhs) { return lhs >= rhs; }},
    {"<=", [](double lhs, double rhs) { return lhs <= rhs; }},
};

}  // namespace Operators
