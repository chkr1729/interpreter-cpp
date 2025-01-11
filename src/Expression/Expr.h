#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <string>

#include "../Token/Token.h"

// Forward declaration of Visitor
class Visitor;

// Abstract base class for expressions
class Expr
{
   public:
    virtual ~Expr() = default;

    // Accept a visitor
    virtual void accept(Visitor& visitor) const = 0;
};

// Concrete subclass for literal expressions
class Literal : public Expr
{
   public:
    explicit Literal(const std::string& value) : value(value) {}

    void accept(Visitor& visitor) const override;

    const std::string& getValue() const { return value; }

   private:
    std::string value;
};

// Concrete subclass for grouping expressions
class Grouping : public Expr
{
   public:
    explicit Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    void accept(Visitor& visitor) const override;

    const Expr* getExpression() const { return expression.get(); }

   private:
    std::unique_ptr<Expr> expression;
};

// Concrete class for unary expressions
class Unary : public Expr
{
   public:
    Unary(Token op, std::unique_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {}

    const Token& getOperator() const { return op; }
    const Expr*  getRight() const { return right.get(); }

    void accept(Visitor& visitor) const override;

   private:
    Token                 op;
    std::unique_ptr<Expr> right;
};

// Concrete subclass for binary expressions
class Binary : public Expr
{
   public:
    Binary(std::unique_ptr<Expr> left, const std::string& op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right))
    {
    }

    void accept(Visitor& visitor) const override;

    const Expr*        getLeft() const { return left.get(); }
    const std::string& getOperator() const { return op; }
    const Expr*        getRight() const { return right.get(); }

   private:
    std::unique_ptr<Expr> left;
    std::string           op;
    std::unique_ptr<Expr> right;
};

#endif  // EXPR_H
