#include <memory>

#include "../Evaluator/Evaluator.h"
#include "../Statement/Statement.h"
#include "Callable.h"
#include "ReturnException.h"

class LoxFunction : public Callable
{
   public:
    explicit LoxFunction(std::shared_ptr<FunctionDefinitionStatement> def) : definition(def) {}

    std::shared_ptr<ResultBase> call(
        Evaluator&                               evaluator,
        Environment*                             env,
        std::vector<std::shared_ptr<ResultBase>> arguments) const override;

    int arity() const override { return definition->getParameters().size(); }  // No parameters

    bool isTruthy() const override { return false; }

    void print() const override { std::cout << "<fn " + definition->getName() + ">" << std::endl; }

   private:
    std::shared_ptr<FunctionDefinitionStatement> definition;
};
