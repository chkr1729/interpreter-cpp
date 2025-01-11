// Evaluator.h
// TODO: Use pragma once in all header files
#pragma once
#include <memory>
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

    template <typename T, typename Op>
    void handleBinaryOperation(std::unique_ptr<ResultBase>& leftResult,
                               std::unique_ptr<ResultBase>& rightResult,
                               const std::string&           errorMsg,
                               Op                           operation);
};
