#pragma once
#include <memory>
#include <vector>

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

class ExpressionStatement : public Statement
{
   public:
    ExpressionStatement(std::unique_ptr<Expression> expression, bool print)
        : expression(std::move(expression)), print(print)
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitExpressionStatement(*this); }

    const Expression* getExpression() { return expression.get(); }
    bool              toPrint() const { return print; }

   private:
    std::unique_ptr<Expression> expression;
    bool                        print;
};

class PrintStatement : public Statement
{
   public:
    explicit PrintStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitPrintStatement(*this); }

    const Expression* getExpression() { return expression.get(); }

   private:
    std::unique_ptr<Expression> expression;
};

class VariableStatement : public Statement
{
   public:
    VariableStatement(const std::string& name, std::unique_ptr<Expression> initializer)
        : name(name), initializer(std::move(initializer))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitVariableStatement(*this); }

    const std::string& getName() const { return name; }
    const Expression*  getInitializer() const { return initializer.get(); }

   private:
    std::string                 name;
    std::unique_ptr<Expression> initializer;
};

class BlockStatement : public Statement
{
   public:
    explicit BlockStatement(std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitBlockStatement(*this); }

    const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements; }

   private:
    std::vector<std::unique_ptr<Statement>> statements;
};

class IfStatement : public Statement
{
   public:
    IfStatement(std::unique_ptr<Expression> condition,
                std::unique_ptr<Statement>  thenBranch,
                std::unique_ptr<Statement>  elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch))
    {
    }

    void accept(StatementVisitor& visitor) override { visitor.visitIfStatement(*this); }

    const Expression& getCondition() const { return *condition; }
    Statement*        getThenBranch() const { return thenBranch.get(); }
    Statement*        getElseBranch() const { return elseBranch.get(); }

   private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement>  thenBranch;
    std::unique_ptr<Statement>  elseBranch;
};
