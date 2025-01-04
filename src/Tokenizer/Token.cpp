#include "Token.h"

// Overload << operator for printing Token
std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch (token.getType())
    {
        case TokenType::Whitespace:
        case TokenType::Comment:
            break;
        case TokenType::MultiCharToken:
            os << Token::multiTokenMap.at(token.getLexeme()) << std::endl;
            break;
        case TokenType::SingleCharToken:
            os << Token::tokenMap.at(token.getLexeme().front()) << std::endl;
            break;
        case TokenType::StringLiteral:
            if (token.hasError())
            {
                os << "[line " << token.getLineNumber() << "] Error: Unterminated string."
                   << std::endl;
            }
            else
            {
                os << "STRING " << token.getLexeme() << " " << token.getLiteral() << std::endl;
            }
            break;
        case TokenType::NumberLiteral:
            os << "NUMBER " << token.getLexeme() << " " << token.getLiteral() << std::endl;
            break;
        case TokenType::Identifier:
            os << "IDENTIFIER " << token.getLexeme() << " " << token.getLiteral() << std::endl;
            break;
        case TokenType::ReservedWord:
            os << Token::reservedWords.at(token.getLexeme()) << " " << token.getLexeme() << " "
               << token.getLiteral() << std::endl;
            break;
        case TokenType::Unexpected:
            os << "[line " << token.getLineNumber()
               << "] Error: Unexpected character: " << token.getLexeme() << std::endl;
            break;
        default:
            os << "Unknown" << std::endl;
            break;
    }
    return os;
}
