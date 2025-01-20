#pragma once
#include <iostream>

#include "../Environment/Environment.h"
#include "../Expression/Expression.h"
#include "../Expression/ExpressionVisitor.h"
#include "../Statement/StatementVisitor.h"

class Printer : public StatementVisitor, public ExpressionVisitor
{
   public:
    // clang-format off
    void visitPrintStatement(const PrintStatement& statement, Environment* env) override;
    void visitExpressionStatement(const ExpressionStatement& statement, Environment* env) override;
    void visitVariableStatement(const VariableStatement& statement, Environment* env) override;
    void visitBlockStatement(const BlockStatement& statement, Environment* env) override {};
    void visitIfStatement(const IfStatement& statement, Environment* env) override {};
    void visitWhileStatement(const WhileStatement& statement, Environment* env) override {};
    void visitForStatement(const ForStatement& statement, Environment* env) override {};
    void visitFunctionDefinitionStatement(const FunctionDefinitionStatement& statement, Environment* env) override {};
    void visitReturnStatement(const ReturnStatement& statement, Environment* env) override {};

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
