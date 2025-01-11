#include <iostream>

class ResultBase
{
   public:
    virtual ~ResultBase() = default;

    // Polymorphic method to print the result
    virtual void print() const = 0;
};

template <typename T>
class Result : public ResultBase
{
   protected:
    T value;

   public:
    explicit Result(const T& value) : value(value) {}

    const T& getValue() const { return value; }

    void print() const override { std::cout << value << std::endl; }
};

template <>
class Result<bool> : public ResultBase
{
   private:
    bool value;

   public:
    explicit Result(bool value) : value(value) {}

    void print() const override { std::cout << (value ? "true" : "false") << std::endl; }

    bool getValue() const { return value; }
};

template <>
class Result<std::nullptr_t> : public ResultBase
{
   public:
    Result() {}

    void print() const override { std::cout << "nil" << std::endl; }

    std::nullptr_t getValue() const { return nullptr; }
};
