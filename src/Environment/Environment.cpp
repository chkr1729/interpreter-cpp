#include "Environment.h"

void Environment::define(const std::string& name, std::shared_ptr<ResultBase> value)
{
    variables[name] = std::move(value);
}

void Environment::assign(const std::string& name, std::shared_ptr<ResultBase> value)
{
    if (variables.find(name) != variables.end())
    {
        variables[name] = std::move(value);
        return;
    }

    // If not found in the current scope, check the enclosing scope
    if (enclosing)
    {
        enclosing->assign(name, std::move(value));
        return;
    }

    std::cerr << "Undefined variable '" + name + "'." << std::endl;
    std::exit(70);
}

std::shared_ptr<ResultBase> Environment::get(const std::string& name) const
{
    auto it = variables.find(name);
    if (it != variables.end())
    {
        return it->second;
    }

    if (enclosing)
    {
        return enclosing->get(name);
    }

    std::cerr << "Undefined variable '" + name + "'." << std::endl;
    std::exit(70);
}
