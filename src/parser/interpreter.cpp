#include "interpreter.hpp"

std::any Interpreter::visit(const BinaryExprAST* expr) {
    std::any left = expr->mLeft->accept(*this);
    std::any right = expr->mRight->accept(*this);

    switch (expr->mOperator.getType())
    {
    case TokenType::MinusToken:
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TokenType::MultiplyToken:
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TokenType::DivideToken:
        return std::any_cast<double>(left) / std::any_cast<double>(right);

    case TokenType::PlusToken:
        if(left.type() == typeid(double) && right.type() == typeid(double)){
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        
         if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }

        throw std::runtime_error("Operands must be of the same type");
        break;
    
    default:
        break;
    }

    return nullptr;
}

std::any Interpreter::visit(const GroupingExprAST* expr) {
    return expr->mExpr->accept(*this);
}

std::any Interpreter::visit(const UnaryExprAST* expr) {
    std::any right = expr->mRight->accept(*this);

    switch (expr->mOperator.getType())
    {
    case TokenType::MinusToken:
        return -std::any_cast<double>(right);

    default:
        break;
    }

    return nullptr;
}

std::any Interpreter::visit(const LiteralExprAST<double>* expr) {
    return expr->mValue;
}

std::any Interpreter::visit(const LiteralExprAST<std::string>* expr) {
    return expr->mValue;
}


void Interpreter::interpret(ExprAST* expr) {
    std::cout << "Interpreting\n";
    std::any value =  expr->accept(*this);

    if(value.type() == typeid(double)){
        std::cout << std::any_cast<double>(value) << std::endl;
    } else if(value.type() == typeid(std::string)){
        std::cout << std::any_cast<std::string>(value) << std::endl;
    }
}