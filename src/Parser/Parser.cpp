#include "Parser.h"

// Constructor: Move tokens into the parser
Parser::Parser(std::vector<Token>&& tokens) : tokens(std::move(tokens)) {}

std::unique_ptr<Expr> Parser::parse()
{
    return parseExpression();  // Start parsing from the top-level expression
}

// Parse an expression (handles comparison operators)
std::unique_ptr<Expr> Parser::parseExpression()
{
    auto left = parseTerm();

    while (match({">", "<", ">=", "<="}))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = parseTerm();
        left =
            std::make_unique<Binary>(std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

// Parse a term (handles + and -)
std::unique_ptr<Expr> Parser::parseTerm()
{
    auto left = parseFactor();

    while (match({"+", "-"}))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = parseFactor();
        left =
            std::make_unique<Binary>(std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

// Parse a factor (handles *, / and unary operators)
std::unique_ptr<Expr> Parser::parseFactor()
{
    auto left = parsePrimary();

    while (match({"*", "/"}))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = parsePrimary();
        left =
            std::make_unique<Binary>(std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

// Parse a primary expression (numbers, grouped expressions, and unary operators)
std::unique_ptr<Expr> Parser::parsePrimary()
{
    Token token = peek();

    // Handle unary operators: ! and -
    if (match({"!", "-"}))
    {
        Token operatorToken = tokens[current - 1];  // The matched operator
        auto  right         = parsePrimary();       // Recursively parse the operand
        return std::make_unique<Unary>(operatorToken, std::move(right));
    }

    // Handle grouped expressions
    if (match({"("}))
    {
        auto expression = parseExpression();
        if (!match({")"}))
        {
            std::cerr << "Error: Missing closing parenthesis" << std::endl;
        }
        return std::make_unique<Grouping>(std::move(expression));
    }

    // Handle literals
    if (token.getType() == TokenType::NumberLiteral)
    {
        Token numberToken = advance();  // Consume the number token
        return std::make_unique<Literal>(numberToken.getLiteral());
    }

    if (token.getType() == TokenType::StringLiteral)
    {
        Token stringToken = advance();  // Consume the string token
        return std::make_unique<Literal>(stringToken.getLiteral());
    }

    if (token.getType() == TokenType::ReservedWord)
    {
        if (token.getLexeme() == "true" || token.getLexeme() == "false" ||
            token.getLexeme() == "nil")
        {
            Token reservedToken = advance();  // Consume the literal token
            return std::make_unique<Literal>(reservedToken.getLexeme());
        }
    }

    std::cerr << "Error: Unexpected token: " << peek().getLexeme() << std::endl;
    advance();  // Consume the unexpected token to prevent infinite loop
    return nullptr;
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
