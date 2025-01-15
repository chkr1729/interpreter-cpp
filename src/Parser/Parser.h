#pragma once
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "../Expression/Expression.h"
#include "../Statement/Statement.h"
#include "../Token/Token.h"

// The Parser class handles parsing tokens into an Abstract Syntax Tree (AST) consisting of
// Statements and Expressions
class Parser
{
   public:
    explicit Parser(std::vector<Token>&& tokens);

    // Main parse method: returns a list of parsed statements
    std::vector<std::unique_ptr<Statement>> parse();

   private:
    std::vector<Token> tokens;  // The list of tokens to parse
    size_t             current = 0;

    // Recursive descent parsing methods for statements
    std::unique_ptr<Statement>           parseStatement();
    std::unique_ptr<PrintStatement>      parsePrintStatement();
    std::unique_ptr<ExpressionStatement> parseExpressionStatement();

    // Recursive descent parsing methods for expressions
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Grouping>   parseGrouping();
    std::unique_ptr<Literal>    parseNumberLiteral();
    std::unique_ptr<Literal>    parseStringLiteral(bool error);
    std::unique_ptr<Literal>    parseBooleanLiteral();
    std::unique_ptr<Literal>    parseNilLiteral();

    // Helper function for parsing binary expressions
    std::unique_ptr<Expression> parseBinary(std::function<std::unique_ptr<Expression>()> subParser,
                                            const std::vector<std::string>&              operators);

    // Helper methods
    Token advance();        // Advances and returns the current token
    Token peek() const;     // Returns the current token without advancing
    bool  isAtEnd() const;  // Checks if we've reached the end of the tokens
    bool  match(const std::vector<std::string>& lexemes);  // Checks and advances if a token matches
    bool  check(const std::string& lexeme) const;  // Checks if the current token matches a lexeme
};
