#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../Result/Result.h"

class Environment
{
   public:
    void define(const std::string& name, std::shared_ptr<ResultBase> value)
    {
        variables[name] = std::move(value);
    }

    std::shared_ptr<ResultBase> get(const std::string& name) const
    {
        auto it = variables.find(name);
        if (it != variables.end())
        {
            return it->second;
        }
        std::cerr << "Error: Undefined variable '" << name << "'." << std::endl;
        std::exit(70);
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<ResultBase>> variables;
};
