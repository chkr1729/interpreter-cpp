#pragma once
#include <memory>
#include <vector>

#include "../Environment/Environment.h"
#include "../Expression/Expression.h"
#include "StatementVisitor.h"

// Base class for statements
class Statement
{
   public:
    virtual ~Statement() = default;

    // Accept method for visitor pattern
    virtual void accept(StatementVisitor& visitor, Environment* env = nullptr) const = 0;
};

class ExpressionStatement : public Statement
{
   public:
    ExpressionStatement(std::unique_ptr<Expression> expression, bool print)
        : expression(std::move(expression)), print(print)
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        return visitor.visitExpressionStatement(*this, env);
    }

    const Expression* getExpression() const { return expression.get(); }

    bool toPrint() const { return print; }

   private:
    const std::unique_ptr<Expression> expression;

    bool print;
};

class PrintStatement : public Statement
{
   public:
    explicit PrintStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitPrintStatement(*this, env);
    }

    const Expression* getExpression() const { return expression.get(); }

   private:
    const std::unique_ptr<Expression> expression;
};

class VariableStatement : public Statement
{
   public:
    VariableStatement(const std::string& name, std::unique_ptr<Expression> initializer)
        : name(name), initializer(std::move(initializer))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitVariableStatement(*this, env);
    }

    const std::string& getName() const { return name; }
    const Expression*  getInitializer() const { return initializer.get(); }

   private:
    const std::string name;

    const std::unique_ptr<Expression> initializer;
};

class BlockStatement : public Statement
{
   public:
    explicit BlockStatement(std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitBlockStatement(*this, env);
    }

    const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements; }

   private:
    const std::vector<std::unique_ptr<Statement>> statements;
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

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitIfStatement(*this, env);
    }

    const Expression* getCondition() const { return condition.get(); }
    const Statement*  getThenBranch() const { return thenBranch.get(); }
    const Statement*  getElseBranch() const { return elseBranch.get(); }

   private:
    const std::unique_ptr<Expression> condition;
    const std::unique_ptr<Statement>  thenBranch;
    const std::unique_ptr<Statement>  elseBranch;
};

class WhileStatement : public Statement
{
   public:
    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
        : condition(std::move(condition)), body(std::move(body))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitWhileStatement(*this, env);
    }

    const Expression* getCondition() const { return condition.get(); }
    const Statement*  getBody() const { return body.get(); }

   private:
    const std::unique_ptr<Expression> condition;
    const std::unique_ptr<Statement>  body;
};

class ForStatement : public Statement
{
   public:
    ForStatement(std::unique_ptr<Statement>  initializer,
                 std::unique_ptr<Expression> condition,
                 std::unique_ptr<Expression> increment,
                 std::unique_ptr<Statement>  body)
        : initializer(std::move(initializer)),
          condition(std::move(condition)),
          increment(std::move(increment)),
          body(std::move(body))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitForStatement(*this, env);
    }

    const Statement*  getInitializer() const { return initializer.get(); }
    const Expression* getCondition() const { return condition.get(); }
    const Expression* getIncrement() const { return increment.get(); }
    const Statement*  getBody() const { return body.get(); }

   private:
    const std::unique_ptr<Statement>  initializer;
    const std::unique_ptr<Expression> condition;
    const std::unique_ptr<Expression> increment;
    const std::unique_ptr<Statement>  body;
};

class FunctionDefinitionStatement : public Statement
{
   public:
    FunctionDefinitionStatement(const std::string&              name,
                                std::vector<std::string>        parameters,
                                std::shared_ptr<BlockStatement> body)
        : name(name), parameters(std::move(parameters)), body(std::move(body))
    {
    }

    void accept(StatementVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitFunctionDefinitionStatement(*this, env);
    }

    const std::string& getName() const { return name; }

    const std::vector<std::string>& getParameters() const { return parameters; }

    const std::shared_ptr<BlockStatement> getBody() const { return body; }

   private:
    const std::string name;

    const std::vector<std::string> parameters;

    const std::shared_ptr<BlockStatement> body;
};
