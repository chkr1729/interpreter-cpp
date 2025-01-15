#include "PrintStatementVisitor.h"

#include "Statement.h"

// Visit a print statement
void PrintStatementVisitor::visitPrintStatement(PrintStatement& statement)
{
    statement.getExpression().accept(*this);
    std::cout << std::endl;
}

// Visit an expression statement
void PrintStatementVisitor::visitExpressionStatement(ExpressionStatement& statement)
{
    statement.getExpression().accept(*this);
    std::cout << std::endl;
}

// Visit a literal expression
void PrintStatementVisitor::visitLiteral(const Literal& expr)
{
    std::cout << expr.getValue();
}

// Visit a grouping expression
void PrintStatementVisitor::visitGrouping(const Grouping& expr)
{
    std::cout << "(group ";
    expr.getExpression()->accept(*this);
    std::cout << ")";
}

// Visit a unary expression
void PrintStatementVisitor::visitUnary(const Unary& expr)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getRight()->accept(*this);
    std::cout << ")";
}

// Visit a binary expression
void PrintStatementVisitor::visitBinary(const Binary& expr)
{
    std::cout << "(" << expr.getOperator() << " ";
    expr.getLeft()->accept(*this);
    std::cout << " ";
    expr.getRight()->accept(*this);
    std::cout << ")";
}
