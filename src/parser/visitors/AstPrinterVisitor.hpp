#pragma once

#include "../ASTs/ExprAST.hpp"

class AstPrinterVisitor : public Visitor{

    public:
        void print(ExprAST* expr);
        void visit(const BinaryExprAST* expr)  override;
        void visit(const GroupingExprAST* expr)  override;
        // template<typename T>
        inline void visit(const LiteralExprAST<double>* expr)  override
        {
            std::cout << (expr->mValue) << " ";
        }
        inline void visit(const LiteralExprAST<std::string>* expr)  override
        {
            std::cout << (expr->mValue) << " ";
        }
        void visit(const UnaryExprAST* expr)  override;

        AstPrinterVisitor() {};
};