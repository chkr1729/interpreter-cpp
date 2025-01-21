#include "Printer.h"

void Printer::visitLiteralExpression(const LiteralExpression& expr, Environment* env)
{
    std::cout << expr.getValue();
}

void Printer::visitGroupingExpression(const GroupingExpression& expr, Environment* env)
{
    std::cout << "(group ";
    expr.getExpression()->accept(*this, env);
    std::cout << ")";
}

void Printer::visitUnaryExpression(const UnaryExpression& expr, Environment* env)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getRight()->accept(*this, env);
    std::cout << ")";
}

void Printer::visitBinaryExpression(const BinaryExpression& expr, Environment* env)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getLeft()->accept(*this, env);
    std::cout << " ";
    expr.getRight()->accept(*this, env);
    std::cout << ")";
}
