#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../Result/Result.h"

class Environment
{
   public:
    void define(const std::string& name, std::shared_ptr<ResultBase> value);
    void assign(const std::string& name, std::shared_ptr<ResultBase> value);

    std::shared_ptr<ResultBase> get(const std::string& name) const;

   private:
    std::unordered_map<std::string, std::shared_ptr<ResultBase>> variables;
};
