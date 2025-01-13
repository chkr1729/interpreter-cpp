#include "Evaluator.h"

#include <cstddef>
#include <functional>
#include <unordered_map>

#include "../Expression/Expression.h"

// Visit a literal expression
void Evaluator::visitLiteral(const Literal& literal)
{
    const std::string& value = literal.getValue();
    if (literal.getType() == LiteralType::Boolean && value != "nil")
    {
        result = std::make_unique<Result<bool>>(value == "true" ? true : false);
    }
    else if (literal.getType() == LiteralType::String)
    {
        result = std::make_unique<Result<std::string>>(value);
    }
    else if (literal.getType() == LiteralType::Number)
    {
        result = std::make_unique<Result<double>>(std::stod(value));
    }
    else if (value == "nil")
        result = std::make_unique<Result<std::nullptr_t>>();
    else
    {
        try
        {
            double number = std::stod(value);  // Convert string to double
            result        = std::make_unique<Result<double>>(number);
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "Error: Invalid number literal: " << value << std::endl;
            result = std::make_unique<Result<std::nullptr_t>>();  // Use NilResult for errors
        }  // Assume it's a number
    }
}

void Evaluator::visitUnary(const Unary& unary)
{
    unary.getRight()->accept(*this);  // Evaluate the operand
    auto op = unary.getOperator();

    if (op == "!")
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
    else if (op == "-")
    {
        auto numberResult = dynamic_cast<Result<double>*>(result.get());
        if (numberResult)
        {
            result = std::make_unique<Result<double>>(-numberResult->getValue());
        }
        else
        {
            std::cerr << "Error: Operand of '-' must be a number" << std::endl;
            result = std::make_unique<Result<std::nullptr_t>>();
        }
    }
}

template <typename T, typename Op>
void Evaluator::handleBinaryOperation(std::unique_ptr<ResultBase>& leftResult,
                                      std::unique_ptr<ResultBase>& rightResult,
                                      const std::string&           errorMsg,
                                      Op                           operation)
{
    auto left  = dynamic_cast<Result<T>*>(leftResult.get());
    auto right = dynamic_cast<Result<T>*>(rightResult.get());

    if (left && right)
    {
        result = std::make_unique<Result<T>>(operation(left->getValue(), right->getValue()));
    }
    else
    {
        std::cerr << "Error: " << errorMsg << std::endl;
        result = std::make_unique<Result<std::nullptr_t>>();
    }
}

void Evaluator::visitBinary(const Binary& binary)
{
    binary.getLeft()->accept(*this);      // Evaluate the left operand
    auto leftResult = std::move(result);  // Store the result of the left operand

    binary.getRight()->accept(*this);      // Evaluate the right operand
    auto rightResult = std::move(result);  // Store the result of the right operand

    auto op = binary.getOperator();

    // Handle arithmetic operators
    static const std::unordered_map<std::string, std::function<double(double, double)>>
        arithmeticOps = {{"+", [](double lhs, double rhs) { return lhs + rhs; }},
                         {"-", [](double lhs, double rhs) { return lhs - rhs; }},
                         {"*", [](double lhs, double rhs) { return lhs * rhs; }},
                         {"/", [](double lhs, double rhs) {
                              if (rhs == 0.0) throw std::runtime_error("Division by zero");
                              return lhs / rhs;
                          }}};

    auto it = arithmeticOps.find(op);
    if (it != arithmeticOps.end())
    {
        handleBinaryOperation<double>(
            leftResult, rightResult, "Operands must be numbers", it->second);
        return;
    }

    // Handle equality operators
    static const std::unordered_map<std::string, std::function<bool(double, double)>> equalityOps =
        {{"==", [](double lhs, double rhs) { return lhs == rhs; }},
         {"!=", [](double lhs, double rhs) { return lhs != rhs; }}};

    auto eqIt = equalityOps.find(op);
    if (eqIt != equalityOps.end())
    {
        handleBinaryOperation<double>(
            leftResult, rightResult, "Operands must be numbers", eqIt->second);
        return;
    }

    std::cerr << "Error: Unknown operator '" << op << "'" << std::endl;
    result = std::make_unique<Result<std::nullptr_t>>();
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
