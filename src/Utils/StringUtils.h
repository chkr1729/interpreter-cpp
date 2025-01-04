
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

// Removes trailing zeros from a numeric string (after a decimal point)
std::string formatNumberLiteral(const std::string& word);
bool        isNumber(const std::string& word);

#endif  // STRING_UTILS_H
