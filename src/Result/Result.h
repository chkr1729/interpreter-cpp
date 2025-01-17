#include <iostream>
#include <optional>

class ResultBase
{
   public:
    virtual ~ResultBase()         = default;
    virtual bool isTruthy() const = 0;
    virtual void print() const    = 0;
};

template <typename T>
class Result : public ResultBase
{
   private:
    T value;

   public:
    explicit Result(const T& value) : value(value) {}

    const T& getValue() const { return value; }

    bool isTruthy() const override { return false; }
    void print() const override { std::cout << value << std::endl; }
};

template <>
class Result<bool> : public ResultBase
{
   private:
    bool value;

   public:
    explicit Result(bool value) : value(value) {}
    bool getValue() const { return value; }
    bool isTruthy() const override { return value; }
    void print() const override { std::cout << (value ? "true" : "false") << std::endl; }
};

template <>
class Result<std::nullptr_t> : public ResultBase
{
   public:
    Result() {}

    std::nullptr_t getValue() const { return nullptr; }

    void print() const override { std::cout << "nil" << std::endl; }
    bool isTruthy() const override { return false; }
};

template <>
class Result<std::string> : public ResultBase
{
   public:
    explicit Result(std::string value) : value(std::move(value)) {}

    const std::string& getValue() const { return value; }

    bool isTruthy() const override { return true; }  // Empty string is false

    void print() const override { std::cout << value << std::endl; }

   private:
    std::string value;
};

template <>
class Result<double> : public ResultBase
{
   public:
    explicit Result(double value) : value(value) {}

    double getValue() const { return value; }

    bool isTruthy() const override { return value != 0; }  // Zero is false, nonzero is true

    void print() const override { std::cout << value << std::endl; }

   private:
    double value;
};
