
#include <unordered_map>
#include <unordered_set>

namespace TokenData
{

inline const std::unordered_set<std::string> booleanLiterals = {"true", "false"};

// TODO: Combine these maps
inline const std::unordered_map<char, std::string> tokenMap = {{'(', "LEFT_PAREN ( null"},
                                                               {')', "RIGHT_PAREN ) null"},
                                                               {'{', "LEFT_BRACE { null"},
                                                               {'}', "RIGHT_BRACE } null"},
                                                               {'*', "STAR * null"},
                                                               {'.', "DOT . null"},
                                                               {',', "COMMA , null"},
                                                               {'+', "PLUS + null"},
                                                               {'-', "MINUS - null"},
                                                               {';', "SEMICOLON ; null"},
                                                               {'=', "EQUAL = null"},
                                                               {'!', "BANG ! null"},
                                                               {'<', "LESS < null"},
                                                               {'>', "GREATER > null"},
                                                               {'/', "SLASH / null"}};

inline const std::unordered_map<std::string, std::string> multiTokenMap = {
    {"==", "EQUAL_EQUAL == null"},
    {"!=", "BANG_EQUAL != null"},
    {"<=", "LESS_EQUAL <= null"},
    {">=", "GREATER_EQUAL >= null"}};

inline const std::unordered_map<std::string, std::string> reservedWords = {{"and", "AND"},
                                                                           {"class", "CLASS"},
                                                                           {"else", "ELSE"},
                                                                           {"false", "FALSE"},
                                                                           {"for", "FOR"},
                                                                           {"fun", "FUN"},
                                                                           {"if", "IF"},
                                                                           {"nil", "NIL"},
                                                                           {"or", "OR"},
                                                                           {"print", "PRINT"},
                                                                           {"return", "RETURN"},
                                                                           {"super", "SUPER"},
                                                                           {"this", "THIS"},
                                                                           {"true", "TRUE"},
                                                                           {"var", "VAR"},
                                                                           {"while", "WHILE"}};
}  // namespace TokenData
