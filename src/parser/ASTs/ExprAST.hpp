#pragma once

#include <iostream>
#include <memory>
#include "../../lexer/token.hpp"
#include <any>

class BinaryExprAST;
class GroupingExprAST;
template<typename T>
class LiteralExprAST;
class UnaryExprAST;

class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visit(const BinaryExprAST* expr)  = 0;
        virtual std::any visit(const GroupingExprAST* expr)  = 0;
        virtual std::any visit(const UnaryExprAST* expr)  = 0;

        virtual std::any visit(const LiteralExprAST<double>* expr) = 0;
        virtual std::any visit(const LiteralExprAST<std::string>* expr) = 0;
        virtual std::any visit(const LiteralExprAST<bool>* expr) = 0;
        virtual std::any visit(const LiteralExprAST<std::nullptr_t>* expr) = 0;
};

class ExprAST {
    public:
        virtual ~ExprAST() = default;
        virtual std::any accept(Visitor& visitor) const = 0;
};

class BinaryExprAST : public ExprAST {
    public:
        SyntaxToken mOperator; // operator
        std::unique_ptr<ExprAST> mLeft, mRight;

    BinaryExprAST(SyntaxToken Op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right) 
                : mOperator(Op), mLeft(std::move(left)), mRight(std::move(right)) {}

    std::any accept(Visitor& visitor) const override {
        return visitor.visit(this);
    }
};

class GroupingExprAST : public ExprAST {
    public:
        std::unique_ptr<ExprAST> mExpr;

    GroupingExprAST(std::unique_ptr<ExprAST>  Expr) : mExpr(std::move(Expr)) {}

    std::any accept(Visitor& visitor) const override {
        return visitor.visit(this);
    }
};

template<typename T>
class LiteralExprAST : public ExprAST {
    public:
        T mValue;

    LiteralExprAST(T value) : mValue(value) {}

    std::any accept(Visitor& visitor) const override {
        return visitor.visit(this);
    }
};

class UnaryExprAST : public ExprAST {
    public:
        SyntaxToken mOperator;
        std::unique_ptr<ExprAST> mRight;

    UnaryExprAST(SyntaxToken Op, std::unique_ptr<ExprAST> right) 
                : mOperator(Op), mRight(std::move(right)) {}

    std::any accept(Visitor& visitor) const override {
        return visitor.visit(this);
    }
};