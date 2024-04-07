#pragma once

#include <iostream>
#include <map>
#include <any>
#include <memory>

#include "../../lexer/token.hpp"
#include "../../alerts.hpp"

class Environment {
    private:
        std::map<std::string, std::any> mValues;
        std::shared_ptr<Environment> mEnclosing = nullptr;

    public:
        void define(const SyntaxToken& token, const std::any& value);
        std::any get(const SyntaxToken& token);
        void assign(const SyntaxToken& token, const std::any& value);

        Environment() = default;
        Environment(std::shared_ptr<Environment> enclosing) : mEnclosing(enclosing) {}

};