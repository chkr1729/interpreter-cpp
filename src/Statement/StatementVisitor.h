#pragma once

// Forward Declarations

class PrintStatement;
class ExpressionStatement;

class StatementVisitor
{
   public:
    virtual ~StatementVisitor() = default;

    // Visit methods for different kinds of statements
    virtual void visitPrintStatement(PrintStatement& statement)           = 0;
    virtual void visitExpressionStatement(ExpressionStatement& statement) = 0;
};
