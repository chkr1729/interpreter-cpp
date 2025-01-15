#include "Evaluator.h"

#include <cstddef>
#include <memory>

#include "../Expression/Expression.h"
#include "../Statement/Statement.h"

void Evaluator::visitPrintStatement(PrintStatement& statement)
{
    statement.getExpression().accept(*this);
    result->print();
    std::cout << std::endl;
}

// Visit a literal expression
void Evaluator::visitLiteral(const Literal& literal)
{
    const std::string& value = literal.getValue();
    switch (literal.getType())
    {
        case LiteralType::Boolean:
            result = std::make_unique<Result<bool>>(value == "true" ? true : false);
            break;
        case LiteralType::String:
            result = std::make_unique<Result<std::string>>(value);
            break;
        case LiteralType::Number:
            try
            {
                double number = std::stod(value);  // Convert string to double
                result        = std::make_unique<Result<double>>(number);
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "Error: Invalid number literal: " << value << std::endl;
                result = std::make_unique<Result<std::nullptr_t>>();  // Use NilResult for errors
            }
            break;
        case LiteralType::Nil:
            result = std::make_unique<Result<std::nullptr_t>>();
            break;
    }
}
// Visit an expression statement
void Evaluator::visitExpressionStatement(ExpressionStatement& statement)
{
    statement.getExpression().accept(*this);
    result->print();
    std::cout << std::endl;
}

void Evaluator::handleBangOperator()
{
    auto boolResult = dynamic_cast<Result<bool>*>(result.get());
    if (boolResult)
    {
        result = std::make_unique<Result<bool>>(!boolResult->getValue());
        return;
    }

    auto doubleResult = dynamic_cast<Result<double>*>(result.get());
    if (doubleResult)
    {
        result = std::make_unique<Result<bool>>(!doubleResult->getValue());
        return;
    }

    if (dynamic_cast<Result<std::nullptr_t>*>(result.get()))
    {
        result = std::make_unique<Result<bool>>(true);
        return;
    }

    std::cerr << "Error: Operand of '!' must be a boolean or nil" << std::endl;
    std::exit(70);
}

void Evaluator::handleMinusOperator()
{
    auto numberResult = dynamic_cast<Result<double>*>(result.get());
    if (numberResult)
    {
        result = std::make_unique<Result<double>>(-numberResult->getValue());
    }
    else
    {
        std::cerr << "Error: Operand must be a number" << std::endl;
        std::exit(70);
    }
}

void Evaluator::visitUnary(const Unary& unary)
{
    unary.getRight()->accept(*this);  // Evaluate the operand
    const auto& op = unary.getOperator();

    if (op == "!")
    {
        handleBangOperator();
    }
    else if (op == "-")
    {
        handleMinusOperator();
    }
    else
    {
        std::cerr << "Error: Unknown unary operator '" << op << "'" << std::endl;
        std::exit(70);
    }
}

void Evaluator::handleNumberOperator(const std::unique_ptr<ResultBase>& leftResult,
                                     const std::unique_ptr<ResultBase>& rightResult,
                                     const std::string&                 op)
{
    auto it = arithmeticOps.find(op);
    if (it != arithmeticOps.end())
    {
        handleBinaryOperation<double>(
            leftResult, rightResult, "Operands must be numbers", it->second);
        return;
    }

    auto relIt = relationalOps.find(op);
    if (relIt != relationalOps.end())
    {
        handleBinaryOperation<double, bool>(
            leftResult, rightResult, "Operands must be numbers", relIt->second);
        return;
    }

    auto eqIt = equalityOps<double>.find(op);
    if (eqIt != equalityOps<double>.end())
    {
        handleBinaryOperation<double, bool>(
            leftResult, rightResult, "Operands must be numbers", eqIt->second);
        return;
    }

    std::cerr << "Error: Unsupported operator '" << op << "' for numbers" << std::endl;
    std::exit(70);
}

void Evaluator::handleStringOperator(const std::unique_ptr<ResultBase>& leftResult,
                                     const std::unique_ptr<ResultBase>& rightResult,
                                     const std::string&                 op)
{
    auto eqIt = equalityOps<std::string>.find(op);
    if (eqIt != equalityOps<std::string>.end())
    {
        handleBinaryOperation<std::string, bool>(
            leftResult, rightResult, "Operands must be strings", eqIt->second);
        return;
    }

    if (op == "+")
    {
        handleBinaryOperation<std::string>(
            leftResult,
            rightResult,
            "Operands must be strings",
            [](const std::string& lhs, const std::string& rhs) { return lhs + rhs; });
        return;
    }

    std::cerr << "Error: Unsupported operator '" << op << "' for strings" << std::endl;
    std::exit(70);
}

void Evaluator::handleIncompatibleTypes(const std::string& op)
{
    if (op == "==" || op == "!=")
    {
        result = std::make_unique<Result<bool>>(op == "!=");
        return;
    }

    std::cerr << "Error: Incompatible types for operator '" << op << "'" << std::endl;
    std::exit(70);
}

void Evaluator::visitBinary(const Binary& binary)
{
    binary.getLeft()->accept(*this);      // Evaluate the left operand
    auto leftResult = std::move(result);  // Store the result of the left operand

    binary.getRight()->accept(*this);      // Evaluate the right operand
    auto rightResult = std::move(result);  // Store the result of the right operand

    const auto& op = binary.getOperator();

    // Handle number operators
    if (dynamic_cast<Result<double>*>(leftResult.get()) &&
        dynamic_cast<Result<double>*>(rightResult.get()))
    {
        handleNumberOperator(leftResult, rightResult, op);
        return;
    }

    // Handle string operators
    if (dynamic_cast<Result<std::string>*>(leftResult.get()) &&
        dynamic_cast<Result<std::string>*>(rightResult.get()))
    {
        handleStringOperator(leftResult, rightResult, op);
        return;
    }

    // Handle incompatible types
    handleIncompatibleTypes(op);
}

void Evaluator::visitGrouping(const Grouping& grp)
{
    grp.getExpression()->accept(*this);
}

void Evaluator::printResult() const
{
    if (!result)
    {
        std::cout << "nil" << std::endl;
        return;
    }

    result->print();  // Call the appropriate print() method based on the specific Result type
}
