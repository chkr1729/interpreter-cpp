#include "Evaluator.h"

#include <cstddef>
#include <memory>

#include "../Expression/Expression.h"
#include "../Function/Callable.h"
#include "../Function/LoxFunction.h"
#include "../Operators/Operators.h"
#include "../Statement/Statement.h"

void Evaluator::visitPrintStatement(const PrintStatement& statement, Environment* env)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this, env);
        result->print();
    }
}

void Evaluator::visitExpressionStatement(const ExpressionStatement& statement, Environment* env)
{
    auto expr = statement.getExpression();
    if (expr)
    {
        expr->accept(*this, env);
    }
    if (statement.toPrint())
    {
        result->print();
    }
}

void Evaluator::visitVariableStatement(const VariableStatement& statement, Environment* env)
{
    result.reset();
    auto initializer = statement.getInitializer();
    if (initializer)
    {
        initializer->accept(*this, env);
    }
    if (env)
    {
        env->define(statement.getName(), result);
    }
}

void Evaluator::visitBlockStatement(const BlockStatement& statement, Environment* env)
{
    auto blockEnv =
        env ? std::make_shared<Environment>(env->getSharedPtr()) : std::make_shared<Environment>();
    for (const auto& stmnt : statement.getStatements())
    {
        stmnt->accept(*this, blockEnv.get());
    }
}

void Evaluator::visitIfStatement(const IfStatement& statement, Environment* env)
{
    statement.getCondition()->accept(*this, env);
    if (result->isTruthy())
    {
        statement.getThenBranch()->accept(*this, env);
    }
    else if (statement.getElseBranch())
    {
        statement.getElseBranch()->accept(*this, env);
    }
}

void Evaluator::visitWhileStatement(const WhileStatement& statement, Environment* env)
{
    while (true)
    {
        statement.getCondition()->accept(*this, env);
        if (!result->isTruthy())
        {
            break;
        }
        statement.getBody()->accept(*this, env);
    }
}

void Evaluator::visitForStatement(const ForStatement& statement, Environment* env)
{
    if (statement.getInitializer())
    {
        statement.getInitializer()->accept(*this, env);
    }

    while (true)
    {
        bool conditionTruthy = true;
        if (statement.getCondition())
        {
            statement.getCondition()->accept(*this, env);
            conditionTruthy = result->isTruthy();
        }

        if (!conditionTruthy)
        {
            break;
        }

        statement.getBody()->accept(*this, env);

        if (statement.getIncrement())
        {
            statement.getIncrement()->accept(*this, env);
        }
    }
}

void Evaluator::visitFunctionDefinitionStatement(const FunctionDefinitionStatement& statement,
                                                 Environment*                       env)
{
    auto functionDef = std::make_shared<FunctionDefinitionStatement>(
        statement.getName(), statement.getParameters(), statement.getBody());
    env->define(statement.getName(), std::make_shared<LoxFunction>(functionDef));
}

void Evaluator::visitVariableExpression(const VariableExpression& expression, Environment* env)
{
    result.reset();
    auto value = (env) ? env->get(expression.getName()) : nullptr;
    if (value)
    {
        result = value;
    }
    else
    {
        result = std::make_unique<Result<std::nullptr_t>>();
    }
}

void Evaluator::visitAssignmentExpression(const AssignmentExpression& expr, Environment* env)
{
    result.reset();
    expr.getValue()->accept(*this, env);
    if (env)
    {
        env->assign(expr.getName(), result);
    }
}

void Evaluator::visitLogicalExpression(const LogicalExpression& expr, Environment* env)
{
    const auto op = expr.getOperator();
    expr.getLeft()->accept(*this, env);
    if (op == "or" && result->isTruthy())
    {
        return;
    }
    if (op == "and" && !result->isTruthy())
    {
        return;
    }
    expr.getRight()->accept(*this, env);
}

void Evaluator::visitLiteralExpression(const LiteralExpression& literal, Environment* env)
{
    result.reset();
    const std::string& value = literal.getValue();
    switch (literal.getType())
    {
        case LiteralType::Boolean:
            result = std::make_shared<Result<bool>>(value == "true" ? true : false);
            break;
        case LiteralType::String:
            result = std::make_shared<Result<std::string>>(value);
            break;
        case LiteralType::Number:
            try
            {
                double number = std::stod(value);  // Convert string to double
                result        = std::make_shared<Result<double>>(number);
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "Error: Invalid number literal: " << value << std::endl;
                result = std::make_shared<Result<std::nullptr_t>>();  // Use NilResult for errors
            }
            break;
        case LiteralType::Nil:
            result = std::make_shared<Result<std::nullptr_t>>();
            break;
    }
}

void Evaluator::handleBangOperator()
{
    auto boolResult = dynamic_cast<Result<bool>*>(result.get());
    if (boolResult)
    {
        result = std::make_shared<Result<bool>>(!boolResult->getValue());
        return;
    }

    auto doubleResult = dynamic_cast<Result<double>*>(result.get());
    if (doubleResult)
    {
        result = std::make_shared<Result<bool>>(!doubleResult->getValue());
        return;
    }

    if (dynamic_cast<Result<std::nullptr_t>*>(result.get()))
    {
        result = std::make_shared<Result<bool>>(true);
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
        result = std::make_shared<Result<double>>(-numberResult->getValue());
    }
    else
    {
        std::cerr << "Error: Operand must be a number" << std::endl;
        std::exit(70);
    }
}

void Evaluator::visitUnaryExpression(const UnaryExpression& unary, Environment* env)
{
    result.reset();
    unary.getRight()->accept(*this, env);
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

void Evaluator::handleNumbersOperation(const std::shared_ptr<Result<double>>& leftResult,
                                       const std::shared_ptr<Result<double>>& rightResult,
                                       const std::string&                     op)
{
    auto it = Operators::arithmeticOps.find(op);
    if (it != Operators::arithmeticOps.end())
    {
        handleBinaryOperation<double>(
            leftResult, rightResult, "Operands must be numbers", it->second);
        return;
    }

    auto relIt = Operators::relationalOps.find(op);
    if (relIt != Operators::relationalOps.end())
    {
        handleBinaryOperation<double, bool>(
            leftResult, rightResult, "Operands must be numbers", relIt->second);
        return;
    }

    auto eqIt = Operators::equalityOps<double>.find(op);
    if (eqIt != Operators::equalityOps<double>.end())
    {
        handleBinaryOperation<double, bool>(
            leftResult, rightResult, "Operands must be numbers", eqIt->second);
        return;
    }

    std::cerr << "Error: Unsupported operator '" << op << "' for numbers" << std::endl;
    std::exit(70);
}

void Evaluator::handleStringsOperation(const std::shared_ptr<Result<std::string>>& leftResult,
                                       const std::shared_ptr<Result<std::string>>& rightResult,
                                       const std::string&                          op)
{
    auto eqIt = Operators::equalityOps<std::string>.find(op);
    if (eqIt != Operators::equalityOps<std::string>.end())
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

void Evaluator::handleBoolsOperation(const std::shared_ptr<Result<bool>>& leftResult,
                                     const std::shared_ptr<Result<bool>>& rightResult,
                                     const std::string&                   op)
{
    auto eqIt = Operators::equalityOps<bool>.find(op);
    if (eqIt != Operators::equalityOps<bool>.end())
    {
        handleBinaryOperation<bool, bool>(
            leftResult, rightResult, "Operands must be booleans", eqIt->second);
        return;
    }

    std::cerr << "Error: Unsupported operator '" << op << "' for booleans" << std::endl;
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

void Evaluator::visitBinaryExpression(const BinaryExpression& binary, Environment* env)
{
    result.reset();

    binary.getLeft()->accept(*this, env);
    auto leftResult = std::move(result);

    binary.getRight()->accept(*this, env);
    auto rightResult = std::move(result);

    const auto& op = binary.getOperator();

    // Handle number operators
    auto leftNum  = std::dynamic_pointer_cast<Result<double>>(leftResult);
    auto rightNum = std::dynamic_pointer_cast<Result<double>>(rightResult);
    if (leftNum && rightNum)
    {
        handleNumbersOperation(leftNum, rightNum, op);
        return;
    }

    // Handle string operators
    auto leftString  = std::dynamic_pointer_cast<Result<std::string>>(leftResult);
    auto rightString = std::dynamic_pointer_cast<Result<std::string>>(rightResult);
    if (leftString && rightString)
    {
        handleStringsOperation(leftString, rightString, op);
        return;
    }

    // Handle bools
    auto leftBool  = std::dynamic_pointer_cast<Result<bool>>(leftResult);
    auto rightBool = std::dynamic_pointer_cast<Result<bool>>(rightResult);
    if (leftBool && rightBool)
    {
        handleBoolsOperation(leftBool, rightBool, op);
        return;
    }

    // Handle incompatible types
    handleIncompatibleTypes(op);
}

void Evaluator::visitGroupingExpression(const GroupingExpression& grp, Environment* env)
{
    grp.getExpression()->accept(*this, env);
}

void Evaluator::visitCallExpression(const CallExpression& expr, Environment* env)
{
    expr.getCallee()->accept(*this, env);
    auto callee = std::dynamic_pointer_cast<Callable>(result);

    if (!callee)
    {
        throw std::runtime_error("Attempt to call a non-function object.");
    }

    // Evaluate arguments
    std::vector<std::shared_ptr<ResultBase>> arguments;
    for (const auto& argument : expr.getArguments())
    {
        argument->accept(*this, env);
        arguments.push_back(result);
    }

    // Ensure correct number of arguments
    if (arguments.size() != callee->arity())
    {
        throw std::runtime_error("Incorrect number of arguments to function.");
    }

    // Call the function
    result = callee->call(*this, env, arguments);
}
