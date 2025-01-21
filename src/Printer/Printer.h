#pragma once
#include <iostream>

#include "../Environment/Environment.h"
#include "../Expression/Expression.h"
#include "../Expression/ExpressionVisitor.h"

class Printer : public ExpressionVisitor
{
   public:
    // clang-format off
    // Expression visitor methods
    void visitLiteralExpression(const LiteralExpression& expr, Environment* env) override;
    void visitGroupingExpression(const GroupingExpression& expr, Environment* env) override;
    void visitUnaryExpression(const UnaryExpression& expr, Environment* env) override;
    void visitBinaryExpression(const BinaryExpression& expr, Environment* env) override;

    void visitVariableExpression(const VariableExpression& expr, Environment* env) override {};
    void visitAssignmentExpression(const AssignmentExpression& expr, Environment* env) override {};
    void visitLogicalExpression(const LogicalExpression& expr, Environment* env) override {};
    void visitCallExpression(const CallExpression& expr, Environment* env) override {};
    // clang-format on
};
