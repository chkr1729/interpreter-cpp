#include "Expr.h"

#include "Visitor.h"

void Literal::accept(Visitor& visitor) const
{
    visitor.visitLiteral(*this);
}

void Grouping::accept(Visitor& visitor) const
{
    visitor.visitGrouping(*this);
}

void Binary::accept(Visitor& visitor) const
{
    visitor.visitBinary(*this);
}

void Unary::accept(Visitor& visitor) const
{
    visitor.visitUnary(*this);
}
