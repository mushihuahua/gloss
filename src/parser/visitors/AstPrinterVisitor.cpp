#include <iostream>
#include <string>
#include <memory>

#include "AstPrinterVisitor.hpp"

void AstPrinterVisitor::print(ExprAST* expr) {
    std::any value =  expr->accept(*this);
    std::cout << std::any_cast<std::string>(value) << std::endl;
}

std::any AstPrinterVisitor::visit(const BinaryExprAST* expr) { 
    std::string left = std::any_cast<std::string>(expr->mLeft->accept(*this));
    std::string right = std::any_cast<std::string>(expr->mRight->accept(*this));
    return left + " " + right + " " + expr->mOperator.getLexeme();

}

std::any AstPrinterVisitor::visit(const UnaryExprAST* expr) { 
    return expr->mOperator.getLexeme() + std::any_cast<std::string>(expr->mRight->accept(*this));
}

std::any AstPrinterVisitor::visit(const GroupingExprAST* expr) { 
    // std::cout << "( ";
    return expr->mExpr->accept(*this);
    // std::cout << ") ";
}

inline std::any AstPrinterVisitor::visit(const LiteralExprAST<double>* expr){
    // convert double to string
    std::string str = std::to_string (std::any_cast<double>(expr->mValue));

    // remove trailing zeros and decimal point
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );

    return str;
}

inline std::any AstPrinterVisitor::visit(const LiteralExprAST<std::string>* expr){
    return "\"" + expr->mValue + "\"";
}

inline std::any AstPrinterVisitor::visit(const LiteralExprAST<bool>* expr){
    return (expr->mValue) ? std::string("true") : std::string("false");
}

inline std::any AstPrinterVisitor::visit(const LiteralExprAST<std::nullptr_t>* expr){
    return std::string("nil");
}