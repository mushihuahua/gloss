#include <iostream>
#include <string>
#include <memory>

#include "AstPrinterVisitor.hpp"

void AstPrinterVisitor::print(ExprAST* expr) {
    expr->accept(*this);
}

void AstPrinterVisitor::visit(const BinaryExprAST* expr) { 
    expr->mLeft->accept(*this);
    expr->mRight->accept(*this);
    std::cout << expr->mOperator.getLexeme() << " ";

}

void AstPrinterVisitor::visit(const LiteralExprAST* expr) { 
    std::cout << *(int*)(expr->mValue) << " ";
}

void AstPrinterVisitor::visit(const UnaryExprAST* expr) { 
    std::cout << expr->mOperator.getLexeme();
    expr->mRight->accept(*this);
}

void AstPrinterVisitor::visit(const GroupingExprAST* expr) { 
    // std::cout << "( ";
    expr->mExpr->accept(*this);
    // std::cout << ") ";
}
