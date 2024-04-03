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
        void define(const SyntaxToken& token, const std::any& value);
        std::any get(const SyntaxToken& token);
        void assign(const SyntaxToken& token, const std::any& value);

        Environment() = default;
};