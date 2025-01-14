#include "Parser.h"

// Constructor: Move tokens into the parser
Parser::Parser(std::vector<Token>&& tokens) : tokens(std::move(tokens)) {}

std::unique_ptr<Expression> Parser::parse()
{
    return parseExpression();  // Start parsing from the top-level expression
}

// Parse an expression (handles equality operators)
std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseBinary([this]() { return parseComparison(); }, {"==", "!="});
}

// Parse a comparison (handles >, <, >=, <=)
std::unique_ptr<Expression> Parser::parseComparison()
{
    return parseBinary([this]() { return parseTerm(); }, {">", "<", ">=", "<="});
}

// Parse a term (handles + and -)
std::unique_ptr<Expression> Parser::parseTerm()
{
    return parseBinary([this]() { return parseFactor(); }, {"+", "-"});
}

// Parse a factor (handles * and /)
std::unique_ptr<Expression> Parser::parseFactor()
{
    return parseBinary([this]() { return parseUnary(); }, {"*", "/"});
}

// Parse unary operators
std::unique_ptr<Expression> Parser::parseUnary()
{
    if (match({"!", "-"}))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = parseUnary();         // Recursively parse the operand
        return std::make_unique<Unary>(operatorToken.getLexeme(), std::move(right));
    }
    return parsePrimary();
}

// Parse a primary expression (numbers, grouped expressions, and unary operators)
std::unique_ptr<Expression> Parser::parsePrimary()
{
    Token token = peek();

    // Handle grouped expressions
    if (match({"("}))
    {
        auto expression = parseExpression();
        if (!match({")"}))
        {
            std::cerr << "Error: Missing closing parenthesis" << std::endl;
            advance();
            return nullptr;
        }
        return std::make_unique<Grouping>(std::move(expression));
    }

    // Handle literals
    if (token.getType() == TokenType::NumberLiteral)
    {
        Token numberToken = advance();  // Consume the number token
        return std::make_unique<Literal>(numberToken.getLiteral(), LiteralType::Number);
    }

    if (token.getType() == TokenType::StringLiteral)
    {
        if (token.hasError())
        {
            std::cerr << "Unterminated string literal" << std::endl;
            advance();
            return nullptr;
        }

        Token stringToken = advance();  // Consume the string token
        return std::make_unique<Literal>(stringToken.getLiteral(), LiteralType::String);
    }

    if (token.getType() == TokenType::BooleanLiteral)
    {
        Token booleanToken = advance();  // Consume the literal token
        return std::make_unique<Literal>(booleanToken.getLexeme(), LiteralType::Boolean);
    }
    if (token.getType() == TokenType::NilLiteral)
    {
        Token nilToken = advance();  // Consume the literal token
        return std::make_unique<Literal>(nilToken.getLexeme(), LiteralType::Nil);
    }

    std::cerr << "Error: Unexpected token: " << peek().getLexeme() << std::endl;
    advance();  // Consume the unexpected token to prevent infinite loop
    return nullptr;
}

// Helper function for parsing binary expressions
std::unique_ptr<Expression> Parser::parseBinary(
    std::function<std::unique_ptr<Expression>()> subParser,
    const std::vector<std::string>&              operators)
{
    auto left = subParser();

    while (match(operators))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = subParser();
        left =
            std::make_unique<Binary>(std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

// Helper method: Advances and returns the current token
Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return tokens[current - 1];  // Return the previous token
}

// Helper method: Returns the current token without advancing
Token Parser::peek() const
{
    return tokens[current];
}

// Helper method: Checks if we've reached the end of the tokens
bool Parser::isAtEnd() const
{
    return current >= tokens.size();
}

// Helper method: Checks if the current token matches any of the given lexemes and advances if it
// does
bool Parser::match(const std::vector<std::string>& lexemes)
{
    if (isAtEnd()) return false;

    for (const auto& lexeme : lexemes)
    {
        if (peek().getLexeme() == lexeme)
        {
            advance();
            return true;
        }
    }

    return false;
}

// Helper method: Checks if the current token matches the given lexeme without advancing
bool Parser::check(const std::string& lexeme) const
{
    if (isAtEnd()) return false;
    return peek().getLexeme() == lexeme;
}
