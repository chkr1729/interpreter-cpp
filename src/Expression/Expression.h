#pragma once
#include <memory>
#include <string>

#include "../Token/Token.h"
#include "ExpressionVisitor.h"

// Abstract base class for expressions
class Expression
{
   public:
    virtual ~Expression() = default;

    // Accept a visitor
    virtual void accept(ExpressionVisitor& visitor) const = 0;
};

// Enum to represent the type of a literal
enum class LiteralType
{
    Number,
    String,
    Boolean,
    Nil
};

// Concrete subclass for literal expressions
class Literal : public Expression
{
   public:
    Literal(const std::string& value, LiteralType type) : value(value), type(type) {}

    void accept(ExpressionVisitor& visitor) const override { visitor.visitLiteral(*this); }

    const std::string& getValue() const { return value; }
    LiteralType        getType() const { return type; }

   private:
    std::string value;
    LiteralType type;
};

// Concrete subclass for grouping expressions
class Grouping : public Expression
{
   public:
    explicit Grouping(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

    void accept(ExpressionVisitor& visitor) const override { visitor.visitGrouping(*this); }

    const Expression* getExpression() const { return expression.get(); }

   private:
    std::unique_ptr<Expression> expression;
};

// Concrete class for unary expressions
class Unary : public Expression
{
   public:
    Unary(const std::string& op, std::unique_ptr<Expression> right)
        : op(op), right(std::move(right))
    {
    }

    const std::string& getOperator() const { return op; }
    const Expression*  getRight() const { return right.get(); }

    void accept(ExpressionVisitor& visitor) const override { visitor.visitUnary(*this); }

   private:
    std::string                 op;
    std::unique_ptr<Expression> right;
};

// Concrete subclass for binary expressions
class Binary : public Expression
{
   public:
    Binary(std::unique_ptr<Expression> left,
           const std::string&          op,
           std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right))
    {
    }

    void accept(ExpressionVisitor& visitor) const override { visitor.visitBinary(*this); }

    const Expression*  getLeft() const { return left.get(); }
    const std::string& getOperator() const { return op; }
    const Expression*  getRight() const { return right.get(); }

   private:
    std::unique_ptr<Expression> left;
    std::string                 op;
    std::unique_ptr<Expression> right;
};

class Variable : public Expression
{
   public:
    explicit Variable(const std::string& name) : name(name) {}

    void accept(ExpressionVisitor& visitor) const override
    {
        visitor.visitVariableExpression(*this);
    }

    const std::string& getName() const { return name; }

   private:
    std::string name;
};

class AssignmentExpression : public Expression
{
   public:
    AssignmentExpression(std::string name, std::unique_ptr<Expression> value)
        : name(std::move(name)), value(std::move(value))
    {
    }

    void accept(ExpressionVisitor& visitor) const override
    {
        visitor.visitAssignmentExpression(*this);
    }

    const std::string& getName() const { return name; }
    const Expression&  getValue() const { return *value; }

   private:
    std::string                 name;
    std::unique_ptr<Expression> value;
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

    void accept(ExpressionVisitor& visitor) const override
    {
        visitor.visitLogicalExpression(*this);
    }

    const Expression&  getLeft() const { return *left; }
    const std::string& getOperator() const { return operatorLexeme; }
    const Expression&  getRight() const { return *right; }

   private:
    std::unique_ptr<Expression> left;
    std::string                 operatorLexeme;
    std::unique_ptr<Expression> right;
};
