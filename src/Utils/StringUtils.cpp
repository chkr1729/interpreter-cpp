
#include "StringUtils.h"

#include <regex>

void removeTrailingZeros(std::string& str)
{
    str = std::regex_replace(str, std::regex("\\.?0+$"), "");
}
