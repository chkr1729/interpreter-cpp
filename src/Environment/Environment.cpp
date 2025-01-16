#include "Environment.h"

void Environment::define(const std::string& name, std::shared_ptr<ResultBase> value)
{
    variables[name] = std::move(value);
}

void Environment::assign(const std::string& name, std::shared_ptr<ResultBase> value)
{
    if (variables.find(name) == variables.end())
    {
        std::cerr << "Error: Undefined variable '" << name << "'." << std::endl;
        return;
    }

    variables[name] = std::move(value);
}

std::shared_ptr<ResultBase> Environment::get(const std::string& name) const
{
    auto it = variables.find(name);
    if (it != variables.end())
    {
        return it->second;
    }
    std::cerr << "Error: Undefined variable '" << name << "'." << std::endl;
    std::exit(70);
}
