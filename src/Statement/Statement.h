#pragma once
#include <memory>

#include "../Expression/Expression.h"
#include "StatementVisitor.h"

// Base class for statements
class Statement
{
   public:
    virtual ~Statement() = default;

    // Accept method for visitor pattern
    virtual void accept(StatementVisitor& visitor) = 0;
};

// Expression statement class
class ExpressionStatement : public Statement
{
   public:
    explicit ExpressionStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitExpressionStatement(*this); }

    Expression& getExpression() { return *expression; }

   private:
    std::unique_ptr<Expression> expression;
};

// Print statement class
class PrintStatement : public Statement
{
   public:
    explicit PrintStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitPrintStatement(*this); }

    Expression& getExpression() { return *expression; }

   private:
    std::unique_ptr<Expression> expression;
};
