
#include "StringUtils.h"

std::string formatNumberLiteral(const std::string& word)
{
    size_t decimalPos = word.find('.');

    if (decimalPos == std::string::npos)
    {
        // No decimal point, append ".0"
        return word + ".0";
    }

    // Find the position of the last non-zero digit after the decimal
    size_t endPos = word.size() - 1;
    while (endPos > decimalPos && word[endPos] == '0')
    {
        endPos--;
    }

    // If the last character is the decimal point, include one zero after it
    if (endPos == decimalPos)
    {
        return word.substr(0, decimalPos + 2);  // Keep "x.0"
    }

    // Return the substring up to the last non-zero digit
    return word.substr(0, endPos + 1);
}

bool isNumber(const std::string& word)
{
    if (word.empty())
    {
        return false;  // An empty string is not a valid number
    }

    bool   hasDecimal = false;
    size_t i          = 0;

    // Handle optional leading sign
    if (word[i] == '+' || word[i] == '-')
    {
        i++;
        if (i == word.size())
        {
            return false;  // String cannot be just a sign
        }
    }

    // Iterate through the string to check for valid digits and decimal point
    for (; i < word.size(); ++i)
    {
        if (std::isdigit(word[i]))
        {
            continue;
        }
        else if (word[i] == '.')
        {
            if (hasDecimal)
            {
                return false;  // More than one decimal point is invalid
            }
            hasDecimal = true;
        }
        else
        {
            return false;  // Any other character is invalid
        }
    }

    return true;  // If we reach here, it's a valid number
}
