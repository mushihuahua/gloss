#pragma once

#include <iostream>
#include "ASTs/ExprAST.hpp"
#include "../lexer/token.hpp"
#include "../alerts.hpp"

class RuntimeError : public std::runtime_error {
    public:
        SyntaxToken mToken;
        RuntimeError(SyntaxToken token, const std::string& msg) : std::runtime_error(msg), mToken(token) {}
};

class Interpreter : public ExprVisitor {
    private:

        std::any visit(const BinaryExprAST* expr) override;
        std::any visit(const GroupingExprAST* expr) override;
        std::any visit(const UnaryExprAST* expr) override;

        inline std::any visit(const LiteralExprAST<double>* expr) override;
        inline std::any visit(const LiteralExprAST<std::string>* expr) override;
        inline std::any visit(const LiteralExprAST<bool>* expr) override;
        inline std::any visit(const LiteralExprAST<std::nullptr_t>* expr) override;

        void assertNumberOperand(const SyntaxToken& op, std::any left, std::any right);
        bool isTruthy(std::any value);

    public:    
        void interpret(ExprAST* expr, bool& error);
};