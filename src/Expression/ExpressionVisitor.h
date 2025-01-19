#pragma once

#include "../Environment/Environment.h"

// Forward Declarations
class LiteralExpression;
class GroupingExpression;
class BinaryExpression;
class UnaryExpression;
class VariableExpression;
class AssignmentExpression;
class LogicalExpression;
class CallExpression;

class ExpressionVisitor
{
   public:
    virtual void visitLiteralExpression(const LiteralExpression& expr,
                                        Environment*             env = nullptr)                            = 0;
    virtual void visitGroupingExpression(const GroupingExpression& expr,
                                         Environment*              env = nullptr)                           = 0;
    virtual void visitBinaryExpression(const BinaryExpression& expr,
                                       Environment*            env = nullptr)                             = 0;
    virtual void visitUnaryExpression(const UnaryExpression& expr, Environment* env = nullptr) = 0;
    virtual void visitVariableExpression(const VariableExpression& expr,
                                         Environment*              env = nullptr)                           = 0;
    virtual void visitAssignmentExpression(const AssignmentExpression& expr,
                                           Environment*                env = nullptr)                         = 0;
    virtual void visitLogicalExpression(const LogicalExpression& expr,
                                        Environment*             env = nullptr)                            = 0;
    virtual void visitCallExpression(const CallExpression& expr, Environment* env = nullptr)   = 0;
};
