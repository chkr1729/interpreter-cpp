#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

#include "../Environment/Environment.h"
#include "../Expression/ExpressionVisitor.h"
#include "../Statement/StatementVisitor.h"

class Evaluator : public ExpressionVisitor, public StatementVisitor  // Inherit both visitors
{
   public:
    Evaluator() : environment(std::make_shared<Environment>()) {}

    const std::shared_ptr<ResultBase>& getResult() const { return result; }

    // Expression visitor methods
    void visitLiteral(const Literal& literal) override;
    void visitUnary(const Unary& unary) override;
    void visitBinary(const Binary& binary) override;
    void visitGrouping(const Grouping& grp) override;
    void visitVariableExpression(const Variable& expression) override;
    void visitAssignmentExpression(const AssignmentExpression& expr) override;

    // Statement visitor methods
    void visitPrintStatement(PrintStatement& statement) override;
    void visitExpressionStatement(ExpressionStatement& statement) override;
    void visitVariableStatement(VariableStatement& statement) override;
    void visitBlockStatement(BlockStatement& statement) override;
    void visitIfStatement(IfStatement& statement) override;

    void printResult() const;

   private:
    std::shared_ptr<ResultBase> result;

    std::shared_ptr<Environment> environment;

    // Handle arithmetic operators
    inline static const std::unordered_map<std::string, std::function<double(double, double)>>
        arithmeticOps = {{"+", [](double lhs, double rhs) { return lhs + rhs; }},
                         {"-", [](double lhs, double rhs) { return lhs - rhs; }},
                         {"*", [](double lhs, double rhs) { return lhs * rhs; }},
                         {"/", [](double lhs, double rhs) {
                              if (rhs == 0.0) throw std::runtime_error("Division by zero");
                              return lhs / rhs;
                          }}};

    // Handle equality operators
    template <typename T>
    inline static const std::unordered_map<std::string, std::function<bool(const T&, const T&)>>
        equalityOps = {
            {"==", [](const T& lhs, const T& rhs) { return lhs == rhs; }},
            {"!=", [](const T& lhs, const T& rhs) { return lhs != rhs; }},
        };

    inline static const std::unordered_map<std::string, std::function<bool(double, double)>>
        relationalOps = {
            {">", [](double lhs, double rhs) { return lhs > rhs; }},
            {"<", [](double lhs, double rhs) { return lhs < rhs; }},
            {">=", [](double lhs, double rhs) { return lhs >= rhs; }},
            {"<=", [](double lhs, double rhs) { return lhs <= rhs; }},
        };

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

    void handleIncompatibleTypes(const std::string& op);

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
