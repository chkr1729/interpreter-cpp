#include "../Result/Result.h"

class ReturnException : public std::exception
{
   public:
    explicit ReturnException(std::shared_ptr<ResultBase> value) : returnValue(std::move(value)) {}

    std::shared_ptr<ResultBase> getValue() const { return returnValue; }

   private:
    std::shared_ptr<ResultBase> returnValue;
};
