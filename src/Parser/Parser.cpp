#include "Parser.h"

#include <memory>

#include "ParserError.h"

Parser::Parser(std::vector<Token>&& tokens) : tokens(std::move(tokens)) {}

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

std::unique_ptr<Statement> Parser::parseStatement()
{
    try
    {
        if (match({"var"}))
        {
            return parseVariableStatement();
        }
        if (match({"print"}))
        {
            return parsePrintStatement();
        }
        if (match({"{"}))
        {
            return parseBlockStatement();
        }
        if (match({"if"}))
        {
            return parseIfStatement();
        }
        if (match({"while"}))
        {
            return parseWhileStatement();
        }
        if (match({"for"}))
        {
            return parseForStatement();
        }
        if (match({"fun"}))
        {
            return parseFunctionDefinitionStatement();
        }
        if (match({"return"}))
        {
            return parseReturnStatement();
        }
        return parseExpressionStatement();
    }
    catch (const ParserError& e)
    {
        std::cerr << "[line " << peek().getLineNumber() << "] Syntax Error: " << e.what()
                  << std::endl;
        std::exit(65);
    }
}

std::unique_ptr<PrintStatement> Parser::parsePrintStatement()
{
    auto expression = parseExpression();
    if (!match({";"}))
    {
        throw ParserError("Missing ';' after print statement.");
    }
    return std::make_unique<PrintStatement>(std::move(expression));
}

std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement()
{
    auto expression = parseExpression();
    if (!expression)
    {
        return nullptr;
    }
    if (isAtEnd())
    {
        return std::make_unique<ExpressionStatement>(std::move(expression), true);
    }
    if (!match({";"}))
    {
        throw ParserError("Missing ';' after expression statement.");
    }
    return std::make_unique<ExpressionStatement>(std::move(expression), false);
}

std::unique_ptr<VariableStatement> Parser::parseVariableStatement()
{
    Token var = advance();

    if (var.getType() != TokenType::Identifier)
    {
        throw ParserError(" Expected variable name after 'var'.");
    }

    std::string name = var.getLexeme();

    std::unique_ptr<Expression> initializer = nullptr;
    if (match({"="}))
    {
        initializer = parseExpression();
    }

    if (!match({";"}))
    {
        throw ParserError("Missing ';' after variable declaration.");
    }

    return std::make_unique<VariableStatement>(name, std::move(initializer));
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement()
{
    std::vector<std::unique_ptr<Statement>> statements;

    while (!check("}") && !isAtEnd())
    {
        statements.push_back(parseStatement());
    }

    if (!match({"}"}))
    {
        throw ParserError("Expected '}' to close block.");
    }

    return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<IfStatement> Parser::parseIfStatement()
{
    if (!match({"("}))
    {
        throw ParserError("Expected '(' after if.");
    }

    auto condition = parseExpression();

    if (!match({")"}))
    {
        throw ParserError("Expected ')' after if condition.");
    }

    auto thenBranch = parseStatement();

    std::unique_ptr<Statement> elseBranch = nullptr;
    if (match({"else"}))
    {
        elseBranch = parseStatement();
    }

    return std::make_unique<IfStatement>(
        std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement()
{
    if (!match({"("}))
    {
        throw ParserError("Expected '(' after while.");
    }

    auto condition = parseExpression();

    if (!match({")"}))
    {
        throw ParserError("Expected ')' after while condition.");
    }

    auto body = parseStatement();

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<ForStatement> Parser::parseForStatement()
{
    if (!match({"("}))
    {
        throw ParserError("Expected '(' after 'for'.");
    }

    // Parse initializer
    std::unique_ptr<Statement> initializer;
    if (!match({";"}))
    {
        initializer = parseStatement();
        if (!initializer)
        {
            throw ParserError("Expected expression in 'for' initializer.");
        }
    }

    // Parse condition
    std::unique_ptr<Expression> condition;
    if (!match({";"}))
    {
        condition = parseExpression();
        if (!condition)
        {
            throw ParserError("Expected condition in 'for' loop.");
        }
        if (!match({";"}))
        {
            throw ParserError("Expected ';' after condition.");
        }
    }

    // Parse increment
    std::unique_ptr<Expression> increment;
    if (!match({")"}))
    {
        increment = parseExpression();
        if (!increment)
        {
            throw ParserError("Expected expression in 'for' increment.");
        }
        if (!match({")"}))
        {
            throw ParserError("Expected ')' after 'for' increment.");
        }
    }

    // Parse body
    auto body = parseStatement();
    if (!body)
    {
        throw ParserError("Expected statement after 'for' loop.");
    }

    if (dynamic_cast<VariableStatement*>(body.get()))
    {
        throw ParserError("Variable declaration not allowed inside 'for' loop.");
    }

    return std::make_unique<ForStatement>(
        std::move(initializer), std::move(condition), std::move(increment), std::move(body));
}

std::unique_ptr<FunctionDefinitionStatement> Parser::parseFunctionDefinitionStatement()
{
    Token name = advance();
    if (!match({"("}))
    {
        throw ParserError("Expect '(' after function name.");
    }

    std::vector<std::string> parameters;
    if (!check({")"}))
    {
        do
        {
            auto token = advance();
            if (token.getType() != TokenType::Identifier)
            {
                throw ParserError("Expect a parameter name.");
            }
            parameters.push_back(token.getLexeme());
        } while (match({","}));
    }
    if (!match({")"}))
    {
        throw ParserError("Expect ')' after parameter.");
    }
    if (!match({"{"}))
    {
        throw ParserError("Expect '{' before function body.");
    }

    std::unique_ptr<BlockStatement> body = parseBlockStatement();

    return std::make_unique<FunctionDefinitionStatement>(
        std::move(name.getLexeme()), std::move(parameters), std::move(body));
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement()
{
    std::unique_ptr<Expression> returnExpr = nullptr;

    if (!check(";"))
    {
        returnExpr = parseExpression();
    }

    if (!match({";"}))
    {
        throw ParserError("Expect ';' after return statement.");
    }

    return std::make_unique<ReturnStatement>(std::move(returnExpr));
}

std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseOr();
}

std::unique_ptr<Expression> Parser::parseOr()
{
    auto left = parseAnd();  // OR has lower precedence than AND

    while (match({"or"}))
    {
        Token operatorToken = tokens[current - 1];  // The matched 'or' operator
        auto  right         = parseAnd();
        left                = std::make_unique<LogicalExpression>(
            std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseAnd()
{
    auto left = parseAssignment();

    while (match({"and"}))
    {
        Token operatorToken = tokens[current - 1];
        auto  right         = parseComparison();
        left                = std::make_unique<LogicalExpression>(
            std::move(left), operatorToken.getLexeme(), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseAssignment()
{
    auto left = parseEquality();
    if (match({"="}))
    {
        Token equals = tokens[current - 1];
        auto  right  = parseAssignment();

        // Ensure that left is a valid variable (identifier)
        if (auto variable = dynamic_cast<VariableExpression*>(left.get()))
        {
            return std::make_unique<AssignmentExpression>(variable->getName(), std::move(right));
        }

        throw ParserError("Invalid assignment target.");
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseEquality()
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
        return std::make_unique<UnaryExpression>(operatorToken.getLexeme(), std::move(right));
    }
    return parseCall(parsePrimary());
}

std::unique_ptr<GroupingExpression> Parser::parseGrouping()
{
    auto expression = parseExpression();
    if (!match({")"}))
    {
        throw ParserError("Missing closing parenthesis");
    }
    return std::make_unique<GroupingExpression>(std::move(expression));
}

std::unique_ptr<LiteralExpression> Parser::parseLiteral()
{
    Token literalToken = advance();

    auto literalLexeme = literalToken.getLexeme();
    auto literalValue  = literalToken.getLiteral();
    auto tokenType     = literalToken.getType();

    std::unique_ptr<LiteralExpression> literalExp;
    switch (tokenType)
    {
        case TokenType::NumberLiteral:
            literalExp = std::make_unique<LiteralExpression>(literalValue, LiteralType::Number);
            break;
        case TokenType::BooleanLiteral:
            literalExp = std::make_unique<LiteralExpression>(literalLexeme, LiteralType::Boolean);
            break;
        case TokenType::NilLiteral:
            literalExp = std::make_unique<LiteralExpression>(literalLexeme, LiteralType::Nil);
            break;
        case TokenType::StringLiteral:
            if (literalToken.hasError())
            {
                throw ParserError("Unterminated String Literal.");
            }
            literalExp = std::make_unique<LiteralExpression>(literalValue, LiteralType::String);
            break;
        default:
            throw ParserError(literalLexeme + "is not a Literal Token ");
    }
    return literalExp;
}

// Parse a primary expression (numbers, grouped expressions, and unary operators)
std::unique_ptr<Expression> Parser::parsePrimary()
{
    Token token = peek();

    if (match({"("}))
    {
        return parseGrouping();
    }

    if (token.getCategory() == TokenCategory::Literal)
    {
        return parseLiteral();
    }

    if (token.getType() == TokenType::Identifier)
    {
        advance();
        std::unique_ptr<Expression> expr = std::make_unique<VariableExpression>(token.getLexeme());

        // If the next token is '(', it means we're parsing a function call
        if (check({"("}))
        {
            expr = parseCall(std::move(expr));
        }
        return expr;
    }
    throw ParserError("Unexpected Token: " + peek().getLexeme());
}

std::unique_ptr<Expression> Parser::parseCall(std::unique_ptr<Expression> callee)
{
    while (match({"("}))
    {
        std::vector<std::unique_ptr<Expression>> arguments;

        if (!check(")"))
        {
            do
            {
                arguments.push_back(parseExpression());
            } while (match({","}));
        }

        if (!match({")"}))
        {
            throw ParserError("Expected ')' after function arguments.");
        }

        callee = std::make_unique<CallExpression>(std::move(callee), std::move(arguments));
    }
    return callee;
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
        left                = std::make_unique<BinaryExpression>(
            std::move(left), operatorToken.getLexeme(), std::move(right));
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
    return isAtEnd() ? tokens[tokens.size() - 1] : tokens[current];
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
