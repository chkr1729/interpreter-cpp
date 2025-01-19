#include "Printer.h"

#include "../Statement/Statement.h"

void Printer::visitPrintStatement(const PrintStatement& statement, Environment* env)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this, env);
        std::cout << std::endl;
    }
}

void Printer::visitExpressionStatement(const ExpressionStatement& statement, Environment* env)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this, env);
        std::cout << std::endl;
    }
}

void Printer::visitVariableStatement(const VariableStatement& statement, Environment* env)
{
    auto expr = statement.getInitializer();
    if (expr)
    {
        expr->accept(*this, env);
        std::cout << std::endl;
    }
}

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
