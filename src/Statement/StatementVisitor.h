#pragma once

#include "../Environment/Environment.h"

// Forward Declarations
class PrintStatement;
class ExpressionStatement;
class VariableStatement;
class BlockStatement;
class IfStatement;
class WhileStatement;
class ForStatement;

class StatementVisitor
{
   public:
    virtual ~StatementVisitor() = default;

    // Visit methods for different kinds of statements
    virtual void visitPrintStatement(const PrintStatement& statement,
                                     Environment*          env = nullptr)                              = 0;
    virtual void visitExpressionStatement(const ExpressionStatement& statement,
                                          Environment*               env = nullptr)                         = 0;
    virtual void visitVariableStatement(const VariableStatement& statement,
                                        Environment*             env = nullptr)                           = 0;
    virtual void visitBlockStatement(const BlockStatement& statement,
                                     Environment*          env = nullptr)                              = 0;
    virtual void visitIfStatement(const IfStatement& statement, Environment* env = nullptr)   = 0;
    virtual void visitWhileStatement(const WhileStatement& statement,
                                     Environment*          env = nullptr)                              = 0;
    virtual void visitForStatement(const ForStatement& statement, Environment* env = nullptr) = 0;
};
