#pragma once

#include "../ASTs/ExprAST.hpp"

class AstPrinterVisitor : public ExprVisitor{

    public:
        AstPrinterVisitor() {};
        void print(ExprAST* expr);

    private:
        std::any visit(const BinaryExprAST* expr)  override;
        std::any visit(const GroupingExprAST* expr)  override;
        std::any visit(const UnaryExprAST* expr)  override;

        inline std::any visit(const LiteralExprAST<double>* expr) override;
        inline std::any visit(const LiteralExprAST<std::string>* expr) override;
        inline std::any visit(const LiteralExprAST<bool>* expr) override;
        inline std::any visit(const LiteralExprAST<std::nullptr_t>* expr) override;
};