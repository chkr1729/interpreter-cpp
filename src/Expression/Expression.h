#pragma once
#include <memory>
#include <string>

#include "../Token/Token.h"
#include "Visitor.h"

// Abstract base class for expressions
class Expression
{
   public:
    virtual ~Expression() = default;

    // Accept a visitor
    virtual void accept(Visitor& visitor) const = 0;
};

// Concrete subclass for literal expressions
class Literal : public Expression
{
   public:
    explicit Literal(const std::string& value) : value(value) {}

    void accept(Visitor& visitor) const override { visitor.visitLiteral(*this); }

    const std::string& getValue() const { return value; }

   private:
    std::string value;
};

// Concrete subclass for grouping expressions
class Grouping : public Expression
{
   public:
    explicit Grouping(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

    void accept(Visitor& visitor) const override { visitor.visitGrouping(*this); }

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

    void accept(Visitor& visitor) const override { visitor.visitUnary(*this); }

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

    void accept(Visitor& visitor) const override { visitor.visitBinary(*this); }

    const Expression*  getLeft() const { return left.get(); }
    const std::string& getOperator() const { return op; }
    const Expression*  getRight() const { return right.get(); }

   private:
    std::unique_ptr<Expression> left;
    std::string                 op;
    std::unique_ptr<Expression> right;
};
