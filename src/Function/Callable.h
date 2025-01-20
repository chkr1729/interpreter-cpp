#pragma once
#include <memory>
#include <vector>

#include "../Evaluator/Evaluator.h"
#include "../Result/Result.h"

class Callable : public ResultBase
{
   public:
    virtual int arity() const = 0;

    virtual std::shared_ptr<ResultBase> call(
        Evaluator& evaluator, std::vector<std::shared_ptr<ResultBase>> arguments) const = 0;

    virtual ~Callable() = default;
};
