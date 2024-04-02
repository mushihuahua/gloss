#pragma once

#include <iostream>
#include <memory>
#include "ExprAST.hpp"
#include <any>

class ExpressionStmtAST;
class PrintStmtAST;

class StmtVisitor {
    public:
        virtual ~StmtVisitor() = default;
        virtual std::any visit(const ExpressionStmtAST* stmt)  = 0;
        virtual std::any visit(const PrintStmtAST* stmt)  = 0;
};

class StmtAST {
    public:
        virtual ~StmtAST() = default;
        virtual std::any accept(StmtVisitor& visitor) = 0;
};

class ExpressionStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> mExpr;

    ExpressionStmtAST(std::unique_ptr<ExprAST> expr) : mExpr(std::move(expr)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};

class PrintStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> mExpr;

    PrintStmtAST(std::unique_ptr<ExprAST> expr) : mExpr(std::move(expr)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};