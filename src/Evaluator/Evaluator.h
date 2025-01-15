#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

#include "../Expression/ExpressionVisitor.h"
#include "../Statement/StatementVisitor.h"  // Include StatementVisitor
#include "Result.h"

class Evaluator : public ExpressionVisitor, public StatementVisitor  // Inherit both visitors
{
   public:
    const std::unique_ptr<ResultBase>& getResult() const { return result; }

    // Expression visitor methods
    void visitLiteral(const Literal& literal) override;
    void visitUnary(const Unary& unary) override;
    void visitBinary(const Binary& binary) override;
    void visitGrouping(const Grouping& grp) override;

    // Statement visitor methods
    void visitPrintStatement(PrintStatement& statement) override;
    void visitExpressionStatement(ExpressionStatement& statement) override;

    void printResult() const;

   private:
    std::unique_ptr<ResultBase> result;

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

    void handleNumberOperator(const std::unique_ptr<ResultBase>& leftResult,
                              const std::unique_ptr<ResultBase>& rightResult,
                              const std::string&                 op);

    void handleStringOperator(const std::unique_ptr<ResultBase>& leftResult,
                              const std::unique_ptr<ResultBase>& rightResult,
                              const std::string&                 op);

    void handleBoolOperator(const std::unique_ptr<ResultBase>& leftResult,
                            const std::unique_ptr<ResultBase>& rightResult,
                            const std::string&                 op);

    void handleIncompatibleTypes(const std::string& op);

    template <typename OperandT, typename ReturnT = OperandT, typename Op>
    void handleBinaryOperation(const std::unique_ptr<ResultBase>& leftResult,
                               const std::unique_ptr<ResultBase>& rightResult,
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
