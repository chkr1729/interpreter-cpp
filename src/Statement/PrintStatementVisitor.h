#pragma once
#include <iostream>

#include "../Expression/Expression.h"
#include "../Expression/ExpressionVisitor.h"
#include "StatementVisitor.h"

// Forward declare classes
class Printer : public StatementVisitor, public ExpressionVisitor
{
   public:
    void visitPrintStatement(PrintStatement& statement) override;
    void visitExpressionStatement(ExpressionStatement& statement) override;

    // Expression visitor methods
    void visitLiteral(const Literal& expr) override;
    void visitGrouping(const Grouping& expr) override;
    void visitUnary(const Unary& expr) override;
    void visitBinary(const Binary& expr) override;
};
