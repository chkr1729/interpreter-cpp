#ifndef VISITOR_H
#define VISITOR_H

#include "Expr.h"

class Visitor
{
   public:
    virtual void visitLiteral(const Literal& expr)   = 0;
    virtual void visitGrouping(const Grouping& expr) = 0;
    virtual void visitBinary(const Binary& expr)     = 0;
    virtual void visitUnary(const Unary& expr)       = 0;
};

#endif  // VISITOR_H
