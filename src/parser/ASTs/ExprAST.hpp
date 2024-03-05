#pragma once

#include <iostream>
#include <memory>
#include "../../lexer/token.hpp"

class BinaryExprAST;
class GroupingExprAST;
class LiteralExprAST;
class UnaryExprAST;

class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual void visit(const BinaryExprAST* expr)  = 0;
        virtual void visit(const GroupingExprAST* expr)  = 0;
        virtual void visit(const LiteralExprAST* expr)  = 0;
        virtual void visit(const UnaryExprAST* expr)  = 0;
};

class ExprAST {
    public:
        virtual ~ExprAST() = default;
        virtual void accept(Visitor& visitor) const = 0;
};

class BinaryExprAST : public ExprAST {
    public:
        SyntaxToken mOperator; // operator
        std::unique_ptr<ExprAST> mLeft, mRight;

    BinaryExprAST(SyntaxToken Op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right) 
                : mOperator(Op), mLeft(std::move(left)), mRight(std::move(right)) {}

    void accept(Visitor& visitor) const override {
        visitor.visit(this);
    }
};

class GroupingExprAST : public ExprAST {
    public:
        std::unique_ptr<ExprAST> mExpr;

    GroupingExprAST(std::unique_ptr<ExprAST>  Expr) : mExpr(std::move(Expr)) {}

    void accept(Visitor& visitor) const override {
        visitor.visit(this);
    }
};

class LiteralExprAST : public ExprAST {
    public:
        void* mValue;

    LiteralExprAST(void* value) : mValue(value) {}

    void accept(Visitor& visitor) const override {
        visitor.visit(this);
    }
};

class UnaryExprAST : public ExprAST {
    public:
        SyntaxToken mOperator;
        std::unique_ptr<ExprAST> mRight;

    UnaryExprAST(SyntaxToken Op, std::unique_ptr<ExprAST> right) 
                : mOperator(Op), mRight(std::move(right)) {}

    void accept(Visitor& visitor) const override {
        visitor.visit(this);
    }
};