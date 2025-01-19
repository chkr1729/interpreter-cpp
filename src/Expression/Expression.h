#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../Environment/Environment.h"
#include "../Token/Token.h"
#include "ExpressionVisitor.h"

// Abstract base class for expressions
class Expression
{
   public:
    virtual ~Expression() = default;

    virtual void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const = 0;
};

// Enum to represent the type of a literal
enum class LiteralType
{
    Number,
    String,
    Boolean,
    Nil
};

class LiteralExpression : public Expression
{
   public:
    LiteralExpression(const std::string& value, LiteralType type) : value(value), type(type) {}

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitLiteralExpression(*this, env);
    }

    const std::string& getValue() const { return value; }
    LiteralType        getType() const { return type; }

   private:
    const std::string value;
    const LiteralType type;
};

// Concrete subclass for grouping expressions
class GroupingExpression : public Expression
{
   public:
    explicit GroupingExpression(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitGroupingExpression(*this, env);
    }

    const Expression* getExpression() const { return expression.get(); }

   private:
    std::unique_ptr<Expression> expression;
};

// Concrete class for unary expressions
class UnaryExpression : public Expression
{
   public:
    UnaryExpression(const std::string& op, std::unique_ptr<Expression> right)
        : op(op), right(std::move(right))
    {
    }

    const std::string& getOperator() const { return op; }
    const Expression*  getRight() const { return right.get(); }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitUnaryExpression(*this, env);
    }

   private:
    const std::string                 op;
    const std::unique_ptr<Expression> right;
};

// Concrete subclass for binary expressions
class BinaryExpression : public Expression
{
   public:
    BinaryExpression(std::unique_ptr<Expression> left,
                     const std::string&          op,
                     std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right))
    {
    }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitBinaryExpression(*this, env);
    }

    const Expression*  getLeft() const { return left.get(); }
    const std::string& getOperator() const { return op; }
    const Expression*  getRight() const { return right.get(); }

   private:
    const std::unique_ptr<Expression> left;
    const std::string                 op;
    const std::unique_ptr<Expression> right;
};

class VariableExpression : public Expression
{
   public:
    explicit VariableExpression(const std::string& name) : name(name) {}

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitVariableExpression(*this, env);
    }

    const std::string& getName() const { return name; }

   private:
    const std::string name;
};

class AssignmentExpression : public Expression
{
   public:
    AssignmentExpression(std::string name, std::unique_ptr<Expression> value)
        : name(std::move(name)), value(std::move(value))
    {
    }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitAssignmentExpression(*this, env);
    }

    const std::string& getName() const { return name; }
    const Expression*  getValue() const { return value.get(); }

   private:
    const std::string                 name;
    const std::unique_ptr<Expression> value;
};

class LogicalExpression : public Expression
{
   public:
    LogicalExpression(std::unique_ptr<Expression> left,
                      const std::string&          operatorLexeme,
                      std::unique_ptr<Expression> right)
        : left(std::move(left)), operatorLexeme(operatorLexeme), right(std::move(right))
    {
    }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitLogicalExpression(*this, env);
    }

    const Expression*  getLeft() const { return left.get(); }
    const std::string& getOperator() const { return operatorLexeme; }
    const Expression*  getRight() const { return right.get(); }

   private:
    const std::unique_ptr<Expression> left;
    const std::string                 operatorLexeme;
    const std::unique_ptr<Expression> right;
};

class CallExpression : public Expression
{
   public:
    CallExpression(std::unique_ptr<Expression>              callee,
                   std::vector<std::unique_ptr<Expression>> arguments)
        : callee(std::move(callee)), arguments(std::move(arguments))
    {
    }

    void accept(ExpressionVisitor& visitor, Environment* env = nullptr) const override
    {
        visitor.visitCallExpression(*this, env);
    }

    const Expression* getCallee() const { return callee.get(); }

    const std::vector<std::unique_ptr<Expression>>& getArguments() const { return arguments; }

   private:
    const std::unique_ptr<Expression>              callee;
    const std::vector<std::unique_ptr<Expression>> arguments;
};
