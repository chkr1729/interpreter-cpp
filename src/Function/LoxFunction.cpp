
#include "LoxFunction.h"

std::shared_ptr<ResultBase> LoxFunction::call(
    Evaluator&                               evaluator,
    Environment*                             env,
    std::vector<std::shared_ptr<ResultBase>> arguments) const
{
    std::shared_ptr<Environment> localEnv =
        env ? std::make_shared<Environment>(env->getSharedPtr()) : std::make_shared<Environment>();

    // Bind parameters to arguments
    const auto& params = definition->getParameters();
    if (params.size() != arguments.size())
    {
        throw std::runtime_error("Function called with incorrect number of arguments.");
    }

    for (size_t i = 0; i < params.size(); ++i)
    {
        localEnv->define(params[i], arguments[i]);
    }

    try
    {
        definition->getBody()->accept(evaluator, localEnv.get());
    }
    catch (const ReturnException& returnValue)
    {
        return returnValue.getValue();
    }
    return std::make_shared<Result<nullptr_t>>();
}
