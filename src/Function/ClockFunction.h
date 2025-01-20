#pragma once
#include <chrono>

#include "Callable.h"

class ClockFunction : public Callable
{
   public:
    int arity() const override { return 0; }  // No parameters

    std::shared_ptr<ResultBase> call(
        Evaluator& evaluator, std::vector<std::shared_ptr<ResultBase>> arguments) const override
    {
        using namespace std::chrono;
        auto secondsSinceEpoch =
            duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        return std::make_shared<Result<double>>(static_cast<double>(secondsSinceEpoch));
    }

    bool isTruthy() const override { return false; }

    void print() const override { std::cout << std::endl; }
};
