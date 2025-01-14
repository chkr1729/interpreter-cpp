#include "Evaluator.h"

#include <cstddef>
#include <memory>

#include "../Expression/Expression.h"

// Visit a literal expression
void Evaluator::visitLiteral(const Literal& literal)
{
    const std::string& value       = literal.getValue();
    const auto         literalType = literal.getType();
    switch (literalType)
    {
        case LiteralType::Boolean:
            result = std::make_unique<Result<bool>>(value == "true" ? true : false);
            break;

        case LiteralType::String:
            result = std::make_unique<Result<std::string>>(value);
            break;

        case LiteralType::Number:
            result = std::make_unique<Result<double>>(std::stod(value));
            break;

        case LiteralType::Nil:
            result = std::make_unique<Result<std::nullptr_t>>();
            break;

        default:
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
    }
}

void Evaluator::handleBangOperator()
{
    auto boolResult = dynamic_cast<Result<bool>*>(result.get());
    if (boolResult)
    {
        result = std::make_unique<Result<bool>>(!boolResult->getValue());
    }
    else if (dynamic_cast<Result<std::nullptr_t>*>(result.get()))
    {
        result = std::make_unique<Result<bool>>(true);
    }
    else
    {
        std::cerr << "Error: Operand of '!' must be a boolean or nil" << std::endl;
        result = std::make_unique<Result<std::nullptr_t>>();
    }
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
        std::cerr << "Operand must be a number" << std::endl;
        std::exit(70);
    }
}

void Evaluator::visitUnary(const Unary& unary)
{
    unary.getRight()->accept(*this);  // Evaluate the operand
    auto op = unary.getOperator();

    if (op == "!")
    {
        handleBangOperator();
    }
    else if (op == "-")
    {
        handleMinusOperator();
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

    // Check for relational operations
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
            leftResult, rightResult, "Operands must be numbers.", eqIt->second);
        return;
    }

    return;
}

void Evaluator::handleStringOperator(const std::unique_ptr<ResultBase>& leftResult,
                                     const std::unique_ptr<ResultBase>& rightResult,
                                     const std::string&                 op)
{
    auto eqIt = equalityOps<std::string>.find(op);
    if (eqIt != equalityOps<std::string>.end())
    {
        handleBinaryOperation<std::string, bool>(
            leftResult, rightResult, "Operands must be Strings", eqIt->second);
        return;
    }
    if (op == "+")
    {
        handleBinaryOperation<std::string>(
            leftResult,
            rightResult,
            "Operands must be strings",
            [](const std::string& left, const std::string& right) { return left + right; });
        return;
    }
    std::cerr << "Incompatible types for '" << op << "'" << std::endl;
    std::exit(70);
}

void Evaluator::handleIncompatibleTypes(const std::string& op)
{
    if (op == "==")
    {
        result = std::make_unique<Result<bool>>(false);
        return;
    }
    else if (op == "!=")
    {
        result = std::make_unique<Result<bool>>(true);
        return;
    }
    std::cerr << "Incompatible types for '" << op << "'" << std::endl;
    std::exit(70);
}

void Evaluator::visitBinary(const Binary& binary)
{
    binary.getLeft()->accept(*this);      // Evaluate the left operand
    auto leftResult = std::move(result);  // Store the result of the left operand

    binary.getRight()->accept(*this);      // Evaluate the right operand
    auto rightResult = std::move(result);  // Store the result of the right operand

    auto op = binary.getOperator();

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
