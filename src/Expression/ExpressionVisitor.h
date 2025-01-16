#pragma once

// Forward Declarations
class Literal;
class Grouping;
class Binary;
class Unary;
class Variable;
class AssignmentExpression;

class ExpressionVisitor
{
   public:
    virtual void visitLiteral(const Literal& expr)                           = 0;
    virtual void visitGrouping(const Grouping& expr)                         = 0;
    virtual void visitBinary(const Binary& expr)                             = 0;
    virtual void visitUnary(const Unary& expr)                               = 0;
    virtual void visitVariableExpression(const Variable& expr)               = 0;
    virtual void visitAssignmentExpression(const AssignmentExpression& expr) = 0;
};
