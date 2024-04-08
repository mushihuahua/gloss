#pragma once

#include <iostream>
#include <memory>
#include "ExprAST.hpp"
#include <any>

class ExpressionStmtAST;
class PrintStmtAST;
class VarStmtAST;
class BlockStmtAST;
class IfStmtAST;
class WhileStmtAST;

class StmtVisitor {
    public:
        virtual ~StmtVisitor() = default;
        virtual std::any visit(const ExpressionStmtAST* stmt)  = 0;
        virtual std::any visit(const PrintStmtAST* stmt)  = 0;
        virtual std::any visit(const VarStmtAST* stmt)  = 0;
        virtual std::any visit(const BlockStmtAST* stmt)  = 0;
        virtual std::any visit(const IfStmtAST* stmt)  = 0;
        virtual std::any visit(const WhileStmtAST* stmt)  = 0;
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

class VarStmtAST : public StmtAST {
    public:
        SyntaxToken mIdentifier;
        std::unique_ptr<ExprAST> mInitialiser;

    VarStmtAST(SyntaxToken identifier, std::unique_ptr<ExprAST> initialiser) : mIdentifier(identifier), mInitialiser(std::move(initialiser)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};

class BlockStmtAST : public StmtAST {
    public:
        std::vector<std::unique_ptr<StmtAST>> mStmts;

    BlockStmtAST(std::vector<std::unique_ptr<StmtAST>> stmts) : mStmts(std::move(stmts)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};

class IfStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> mCondition;
        std::unique_ptr<StmtAST> mThenStmt;
        std::unique_ptr<StmtAST> mElseStmt;


    IfStmtAST(std::unique_ptr<ExprAST> condition, std::unique_ptr<StmtAST> thenExpr, std::unique_ptr<StmtAST> elseExpr) 
            : mCondition(std::move(condition)), mThenStmt(std::move(thenExpr)), mElseStmt(std::move(elseExpr)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};

class WhileStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> mCondition;
        std::unique_ptr<StmtAST> mBody;

    WhileStmtAST(std::unique_ptr<ExprAST> condition, std::unique_ptr<StmtAST> body) 
            : mCondition(std::move(condition)), mBody(std::move(body)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(this);
    }
};