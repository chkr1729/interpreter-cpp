#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include <iostream>

#include "../Expression/Visitor.h"

class PrintVisitor : public Visitor
{
   public:
    void visitLiteral(const Literal& expr) override { std::cout << expr.getValue(); }

    void visitGrouping(const Grouping& expr) override
    {
        std::cout << "(group ";
        expr.getExpression()->accept(*this);
        std::cout << ")";
    }

    void visitUnary(const Unary& expr) override
    {
        std::cout << "(" << expr.getOperator() << " ";
        expr.getRight()->accept(*this);
        std::cout << ")";
    }

    void visitBinary(const Binary& expr) override
    {
        std::cout << "(" << expr.getOperator() << " ";
        expr.getLeft()->accept(*this);
        std::cout << " ";
        expr.getRight()->accept(*this);
        std::cout << ")";
    }
};
#endif  // PRINT_VISITOR_H
