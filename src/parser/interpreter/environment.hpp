#pragma once

#include <iostream>
#include <map>
#include <any>

#include "../../lexer/token.hpp"
#include "../../alerts.hpp"

class Environment {
    private:
        std::map<std::string, std::any> mValues;

    public:
        void define(const std::string& name, const std::any& value);
        std::any get(const SyntaxToken& token);

        Environment() = default;
};