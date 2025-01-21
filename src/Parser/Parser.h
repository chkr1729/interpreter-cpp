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
    std::vector<Token> tokens;

    size_t current = 0;

    // Recursive descent parsing methods for statements
    std::unique_ptr<Statement>                   parseStatement();
    std::unique_ptr<PrintStatement>              parsePrintStatement();
    std::unique_ptr<ExpressionStatement>         parseExpressionStatement();
    std::unique_ptr<VariableStatement>           parseVariableStatement();
    std::unique_ptr<BlockStatement>              parseBlockStatement();
    std::unique_ptr<IfStatement>                 parseIfStatement();
    std::unique_ptr<WhileStatement>              parseWhileStatement();
    std::unique_ptr<ForStatement>                parseForStatement();
    std::unique_ptr<FunctionDefinitionStatement> parseFunctionDefinitionStatement();
    std::unique_ptr<ReturnStatement>             parseReturnStatement();

    // Recursive descent parsing methods for expressions
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseOr();
    std::unique_ptr<Expression> parseAnd();
    std::unique_ptr<Expression> parseAssignment();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Expression> parseCall(std::unique_ptr<Expression> callee);

    std::unique_ptr<GroupingExpression> parseGrouping();
    std::unique_ptr<LiteralExpression>  parseLiteral();

    // Helper function for parsing binary expressions
    std::unique_ptr<Expression> parseBinary(std::function<std::unique_ptr<Expression>()> subParser,
                                            const std::vector<std::string>&              operators);

    // Helper methods
    Token advance();
    Token peek() const;

    bool isAtEnd() const;
    bool match(const std::vector<std::string>& lexemes);
    bool check(const std::string& lexeme) const;
};
