#include "Parser.h"

#include <iostream>

// Constructor: Move the tokens into the internal storage
Parser::Parser(std::vector<Token>&& tokens) : tokens(std::move(tokens)) {}

// Parse the file contents
void Parser::parse()
{
    while (index < tokens.size())
    {
        Token token = tokens[index];

        switch (token.getType())
        {
            case TokenType::Whitespace:
            case TokenType::Comment:
            case TokenType::MultiCharToken:
                break;
            case TokenType::SingleCharToken:
                if (token.getLexeme() == "(")
                {
                    std::cout << "(group ";
                    index++;
                    parse();
                }
                else if (token.getLexeme() == ")")
                {
                    std::cout << ")";
                }
                break;
            case TokenType::StringLiteral:
                std::cout << tokens[index].getLiteral();
                break;
            case TokenType::NumberLiteral:
                std::cout << token.getLiteral();
                break;
            case TokenType::Identifier:
                break;
            case TokenType::ReservedWord: {
                const std::string lexeme = token.getLexeme();
                if (isBooleanLiteral(lexeme))
                {
                    std::cout << lexeme;
                }
                break;
            }
            case TokenType::Unexpected:
            default:
                std::cerr << "Error: Unexpected token: " << token.getLexeme() << std::endl;
                break;
        }
        index++;
    }
}
