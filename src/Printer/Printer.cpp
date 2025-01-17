#include "Printer.h"

#include "../Statement/Statement.h"

// Visit a print statement
void Printer::visitPrintStatement(PrintStatement& statement)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this);
        std::cout << std::endl;
    }
}

// Visit an expression statement
void Printer::visitExpressionStatement(ExpressionStatement& statement)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this);
        std::cout << std::endl;
    }
}

void Printer::visitVariableStatement(VariableStatement& statement)
{
    auto expr = statement.getInitializer();
    if (expr)
    {
        expr->accept(*this);
        std::cout << std::endl;
    }
}

void Printer::visitBlockStatement(BlockStatement& statement)
{
    std::cout << std::endl;
}

void Printer::visitIfStatement(IfStatement& statement)
{
    std::cout << std::endl;
}

void Printer::visitWhileStatement(WhileStatement& statement)
{
    std::cout << std::endl;
}

void Printer::visitForStatement(ForStatement& statement)
{
    std::cout << std::endl;
}

// Visit a literal expression
void Printer::visitLiteral(const Literal& expr)
{
    std::cout << expr.getValue();
}

// Visit a grouping expression
void Printer::visitGrouping(const Grouping& expr)
{
    std::cout << "(group ";
    expr.getExpression()->accept(*this);
    std::cout << ")";
}

// Visit a unary expression
void Printer::visitUnary(const Unary& expr)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getRight()->accept(*this);
    std::cout << ")";
}

// Visit a binary expression
void Printer::visitBinary(const Binary& expr)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getLeft()->accept(*this);
    std::cout << " ";
    expr.getRight()->accept(*this);
    std::cout << ")";
}
