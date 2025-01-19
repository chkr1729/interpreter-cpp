#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

#include "../Environment/Environment.h"
#include "../Expression/ExpressionVisitor.h"
#include "../Statement/Statement.h"
#include "../Statement/StatementVisitor.h"

class Evaluator : public ExpressionVisitor, public StatementVisitor  // Inherit both visitors
{
   public:
    // Expression visitor methods
    void visitLiteralExpression(const LiteralExpression& expr, Environment* env) override;
    void visitUnaryExpression(const UnaryExpression& expr, Environment* env) override;
    void visitBinaryExpression(const BinaryExpression& expr, Environment* env) override;
    void visitGroupingExpression(const GroupingExpression& expr, Environment* env) override;
    void visitVariableExpression(const VariableExpression& expr, Environment* env) override;
    void visitAssignmentExpression(const AssignmentExpression& expr, Environment* env) override;
    void visitLogicalExpression(const LogicalExpression& expr, Environment* env) override;
    void visitCallExpression(const CallExpression& expr, Environment* env) override;

    // Statement visitor methods
    void visitPrintStatement(const PrintStatement& statement, Environment* env) override;
    void visitExpressionStatement(const ExpressionStatement& statement, Environment* env) override;
    void visitVariableStatement(const VariableStatement& statement, Environment* env) override;
    void visitBlockStatement(const BlockStatement& statement, Environment* env) override;
    void visitIfStatement(const IfStatement& statement, Environment* env) override;
    void visitWhileStatement(const WhileStatement& statement, Environment* env) override;
    void visitForStatement(const ForStatement& statement, Environment* env) override;

   private:
    std::shared_ptr<ResultBase> result;

    void handleIncompatibleTypes(const std::string& op);

    void handleLogicalOr(const LogicalExpression& expr, Environment* env);
    void handleLogicalAnd(const LogicalExpression& expr, Environment* env);

    void handleBangOperator();
    void handleMinusOperator();

    void handleNumberOperator(const std::shared_ptr<ResultBase>& leftResult,
                              const std::shared_ptr<ResultBase>& rightResult,
                              const std::string&                 op);

    void handleStringOperator(const std::shared_ptr<ResultBase>& leftResult,
                              const std::shared_ptr<ResultBase>& rightResult,
                              const std::string&                 op);

    void handleBoolOperator(const std::shared_ptr<ResultBase>& leftResult,
                            const std::shared_ptr<ResultBase>& rightResult,
                            const std::string&                 op);

    template <typename OperandT, typename ReturnT = OperandT, typename Op>
    void handleBinaryOperation(const std::shared_ptr<ResultBase>& leftResult,
                               const std::shared_ptr<ResultBase>& rightResult,
                               const std::string&                 errorMsg,
                               Op                                 operation)
    {
        auto left  = dynamic_cast<Result<OperandT>*>(leftResult.get());
        auto right = dynamic_cast<Result<OperandT>*>(rightResult.get());

        if (left && right)
        {
            result =
                std::make_unique<Result<ReturnT>>(operation(left->getValue(), right->getValue()));
        }
        else
        {
            std::cerr << errorMsg << std::endl;
            std::exit(70);
        }
    }
};
