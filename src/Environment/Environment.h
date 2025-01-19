#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../Result/Result.h"

class Environment : public std::enable_shared_from_this<Environment>
{
   public:
    explicit Environment(std::shared_ptr<Environment> enclosing = nullptr)
        : enclosing(std::move(enclosing))
    {
    }

    std::shared_ptr<Environment> getSharedPtr() { return shared_from_this(); }

    void define(const std::string& name, std::shared_ptr<ResultBase> value);
    void assign(const std::string& name, std::shared_ptr<ResultBase> value);

    void initializeGlobalScope();

    const std::shared_ptr<ResultBase>& get(const std::string& name) const;

   private:
    std::unordered_map<std::string, std::shared_ptr<ResultBase>> variables;

    std::shared_ptr<Environment> enclosing;
};
