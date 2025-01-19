#pragma once
#include <memory>
#include <vector>

#include "../Result/Result.h"

class Callable : public ResultBase
{
   public:
    virtual int arity() const = 0;

    virtual std::shared_ptr<ResultBase> call(
        std::vector<std::shared_ptr<ResultBase>> arguments) = 0;

    virtual ~Callable() = default;
};
