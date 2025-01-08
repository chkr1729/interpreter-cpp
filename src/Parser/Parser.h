#ifndef PARSER_H
#define PARSER_H

#include <cstddef>
#include <iostream>
#include <vector>

#include "../Token/Token.h"
#include "Expr.h"

class Parser
{
   public:
    explicit Parser(std::vector<Token>&& tokens);
    std::unique_ptr<Expr> parse();  // Returns the root of the expression tree

   private:
    std::vector<Token> tokens;
    size_t             current = 0;

    // Recursive descent parsing methods
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseComparison();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Expr> parsePrimary();

    // Helper methods
    bool  match(const std::vector<std::string>& lexemes);
    bool  check(const std::string& lexeme) const;
    Token advance();
    Token peek() const;
    bool  isAtEnd() const;
};

#endif  // PARSER_H
