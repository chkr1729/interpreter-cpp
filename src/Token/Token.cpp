
#include "Token.h"

#include "TokenData.h"

void Token::print() const
{
    switch (type)
    {
        case TokenType::Whitespace:
        case TokenType::Comment:
            break;
        case TokenType::MultiCharOperator:
            std::cout << TokenData::multiTokenMap.at(lexeme) << std::endl;
            break;
        case TokenType::SingleCharOperator:
            std::cout << TokenData::tokenMap.at(lexeme.front()) << std::endl;
            break;
        case TokenType::StringLiteral:
            if (error)
            {
                std::cerr << "[line " << lineNumber << "] Error: Unterminated string." << std::endl;
            }
            else
            {
                std::cout << "STRING " << lexeme << " " << literal << std::endl;
            }
            break;
        case TokenType::NumberLiteral:
            std::cout << "NUMBER " << lexeme << " " << literal << std::endl;
            break;
        case TokenType::BooleanLiteral:
        case TokenType::NilLiteral:
        case TokenType::ReservedWord:
            std::cout << TokenData::reservedWords.at(lexeme) << " " << lexeme << " " << literal
                      << std::endl;
            break;
        case TokenType::Identifier:
            std::cout << "IDENTIFIER " << lexeme << " " << literal << std::endl;
            break;
        case TokenType::Unexpected:
            std::cerr << "[line " << lineNumber << "] Error: Unexpected character: " << lexeme
                      << std::endl;
            break;
        default:
            std::cerr << "Unknown" << std::endl;
            break;
    }
}
