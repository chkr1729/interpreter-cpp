#pragma once

// Forward Declarations
class PrintStatement;
class ExpressionStatement;
class VariableStatement;
class BlockStatement;
class IfStatement;

class StatementVisitor
{
   public:
    virtual ~StatementVisitor() = default;

    // Visit methods for different kinds of statements
    virtual void visitPrintStatement(PrintStatement& statement)           = 0;
    virtual void visitExpressionStatement(ExpressionStatement& statement) = 0;
    virtual void visitVariableStatement(VariableStatement& statement)     = 0;
    virtual void visitBlockStatement(BlockStatement& statement)           = 0;
    virtual void visitIfStatement(IfStatement& statement)                 = 0;
};
