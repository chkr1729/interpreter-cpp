#pragma once
#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

#include "../Expression/Expression.h"
#include "../Token/Token.h"

class Parser
{
   public:
    explicit Parser(std::vector<Token>&& tokens);
    std::unique_ptr<Expression> parse();  // Returns the root of the expression tree

   private:
    std::vector<Token> tokens;
    size_t             current = 0;

    // Recursive descent parsing methods
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();

    std::unique_ptr<Expression> parseBinary(std::function<std::unique_ptr<Expression>()> subParser,
                                            const std::vector<std::string>&              operators);

    // Helper methods
    bool  match(const std::vector<std::string>& lexemes);
    bool  check(const std::string& lexeme) const;
    Token advance();
    Token peek() const;
    bool  isAtEnd() const;
};
