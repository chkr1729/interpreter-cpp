#include "Parser.h"

#include <memory>

// Constructor: Move tokens into the parser
Parser::Parser(std::vector<Token>&& tokens) : tokens(std::move(tokens)) {}

// Main parse method: returns a list of statements
std::vector<std::unique_ptr<Statement>> Parser::parse()
{
    std::vector<std::unique_ptr<Statement>> statements;
    while (!isAtEnd())
    {
        auto statement = parseStatement();
        if (statement)
        {
            statements.push_back(std::move(statement));
        }
    }
    return statements;
}

// Parse a statement
std::unique_ptr<Statement> Parser::parseStatement()
{
    if (match({"print"}))
    {
        return parsePrintStatement();
    }
    return parseExpressionStatement();
}

// Parse a print statement
std::unique_ptr<PrintStatement> Parser::parsePrintStatement()
{
    auto expression = parseExpression();
    if (!match({";"}))
    {
        std::cerr << "Error: Missing ';' after print statement." << std::endl;
    }
    return std::make_unique<PrintStatement>(std::move(expression));
}

// Parse an expression statement
std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement()
{
    auto expression = parseExpression();
    if (isAtEnd())
    {
        return std::make_unique<ExpressionStatement>(std::move(expression), true);
    }
    if (!match({";"}))
    {
        std::cerr << "Error: Missing ';' after expression statement." << std::endl;
    }
    return std::make_unique<ExpressionStatement>(std::move(expression), false);
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

std::unique_ptr<Grouping> Parser::parseGrouping()
{
    auto expression = parseExpression();
    if (!match({")"}))
    {
        std::cerr << "Error: Missing closing parenthesis" << std::endl;
        retVal = 65;
        advance();
        return nullptr;
    }
    return std::make_unique<Grouping>(std::move(expression));
}

std::unique_ptr<Literal> Parser::parseNumberLiteral()
{
    Token numberToken = advance();  // Consume the number token
    return std::make_unique<Literal>(numberToken.getLiteral(), LiteralType::Number);
}

std::unique_ptr<Literal> Parser::parseStringLiteral(bool error)
{
    if (error)
    {
        std::cerr << "Unterminated string literal" << std::endl;
        retVal = 65;
    }

    Token stringToken = advance();  // Consume the string token
    return std::make_unique<Literal>(stringToken.getLiteral(), LiteralType::String);
}

std::unique_ptr<Literal> Parser::parseBooleanLiteral()
{
    Token booleanToken = advance();  // Consume the literal token
    return std::make_unique<Literal>(booleanToken.getLexeme(), LiteralType::Boolean);
}

std::unique_ptr<Literal> Parser::parseNilLiteral()
{
    Token nilToken = advance();  // Consume the literal token
    return std::make_unique<Literal>(nilToken.getLexeme(), LiteralType::Nil);
}

// Parse a primary expression (numbers, grouped expressions, and unary operators)
std::unique_ptr<Expression> Parser::parsePrimary()
{
    Token token     = peek();
    auto  tokenType = token.getType();

    // Handle grouped expressions
    if (match({"("}))
    {
        return parseGrouping();
    }

    // Handle literals
    if (tokenType == TokenType::NumberLiteral)
    {
        return parseNumberLiteral();
    }

    if (tokenType == TokenType::StringLiteral)
    {
        return parseStringLiteral(token.hasError());
    }

    if (token.getType() == TokenType::BooleanLiteral)
    {
        return parseBooleanLiteral();
    }

    if (token.getType() == TokenType::NilLiteral)
    {
        return parseNilLiteral();
    }

    std::cerr << "Error: Unexpected token: " << peek().getLexeme() << std::endl;
    retVal = 65;
    advance();
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
