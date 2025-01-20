#include <memory>

#include "../Evaluator/Evaluator.h"
#include "../Statement/Statement.h"
#include "Callable.h"

class LoxFunction : public Callable
{
   public:
    explicit LoxFunction(std::shared_ptr<FunctionDefinitionStatement> def) : definition(def) {}

    std::shared_ptr<ResultBase> call(
        Evaluator&                               evaluator,
        Environment*                             env,
        std::vector<std::shared_ptr<ResultBase>> arguments) const override
    {
        std::shared_ptr<Environment> localEnv =
            env ? std::make_shared<Environment>(env->getSharedPtr())
                : std::make_shared<Environment>();

        // Bind parameters to arguments
        const auto& params = definition->getParameters();
        if (params.size() != arguments.size())
        {
            std::cout << "Params size: " << params.size() << " Arguments Size: " << arguments.size()
                      << std::endl;

            throw std::runtime_error("Function called with incorrect number of arguments.");
        }

        for (size_t i = 0; i < params.size(); ++i)
        {
            localEnv->define(params[i], arguments[i]);
        }

        definition->getBody()->accept(evaluator, localEnv.get());
        // return evaluator.getResult();  // Functions return nothing (nil) by default
        return std::make_shared<Result<nullptr_t>>();
    }

    int arity() const override { return 0; }  // No parameters

    bool isTruthy() const override { return false; }

    void print() const override { std::cout << "<fn " + definition->getName() + ">" << std::endl; }

   private:
    std::shared_ptr<FunctionDefinitionStatement> definition;
};
