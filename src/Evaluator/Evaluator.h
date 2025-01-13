// Evaluator.h
// TODO: Use pragma once in all header files
#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <variant>

#include "../Expression/Visitor.h"
#include "Result.h"

class Evaluator : public Visitor
{
   public:
    const std::unique_ptr<ResultBase>& getResult() const { return result; }

    void visitLiteral(const Literal& literal) override;
    void visitUnary(const Unary& unary) override;
    void visitBinary(const Binary& binary) override;
    void visitGrouping(const Grouping& grp) override;

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
    inline static const std::unordered_map<std::string, std::function<bool(double, double)>>
        equalityOps = {{"==", [](double lhs, double rhs) { return lhs == rhs; }},
                       {"!=", [](double lhs, double rhs) { return lhs != rhs; }}};

    inline static const std::unordered_map<std::string, std::function<bool(double, double)>>
        relationalOps = {
            {">", [](double lhs, double rhs) { return lhs > rhs; }},
            {"<", [](double lhs, double rhs) { return lhs < rhs; }},
            {">=", [](double lhs, double rhs) { return lhs >= rhs; }},
            {"<=", [](double lhs, double rhs) { return lhs <= rhs; }},
        };

    void handleNumberOperator(const std::unique_ptr<ResultBase>& leftResult,
                              const std::unique_ptr<ResultBase>& rightResult,
                              const std::string&                 op);

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
            std::cerr << "Error: " << errorMsg << std::endl;
            result = std::make_unique<Result<std::nullptr_t>>();
        }
    }
};
