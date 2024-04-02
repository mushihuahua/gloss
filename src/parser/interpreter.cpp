#include "interpreter.hpp"

bool isEqual(std::any left, std::any right){
    if(left.type() == typeid(std::nullptr_t) && right.type() == typeid(std::nullptr_t) ){ return true; }

    if(left.type() != right.type()){
        return false;
    }

    if(left.type() == typeid(double)){ return std::any_cast<double>(left) == std::any_cast<double>(right); }
    if(left.type() == typeid(std::string)){ return std::any_cast<std::string>(left) == std::any_cast<std::string>(right); }
    if(left.type() == typeid(bool)){ return std::any_cast<bool>(left) == std::any_cast<bool>(right); }

    return false;
}

std::any Interpreter::visit(const BinaryExprAST* expr) {
    std::any left = expr->mLeft->accept(*this);
    std::any right = expr->mRight->accept(*this);

    switch (expr->mOperator.getType())
    {

    case TokenType::MinusToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TokenType::MultiplyToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TokenType::DivideToken:
        assertNumberOperand(expr->mOperator, left, right);
    if(std::any_cast<double>(right) == 0){
        throw RuntimeError(expr->mOperator, "Division by zero");
    }
        return std::any_cast<double>(left) / std::any_cast<double>(right);

    case TokenType::PlusToken:
        if(left.type() == typeid(double) && right.type() == typeid(double)){
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        
         if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }

        throw RuntimeError(expr->mOperator, "Operands must be of the same type");
        break;
    
    case TokenType::GreaterThanToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
    case TokenType::GreaterThanEqualToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case TokenType::LessThanToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
    case TokenType::LessThanEqualToken:
        assertNumberOperand(expr->mOperator, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);

    case TokenType::EqualEqualToken:
        return isEqual(left, right);
    case TokenType::ExclamationEqualToken:
        return !isEqual(left, right);
    
    default:
        break;
    }

    return nullptr;
}

std::any Interpreter::visit(const GroupingExprAST* expr) {
    return expr->mExpr->accept(*this);
}

bool Interpreter::isTruthy(std::any value){
    if(value.type() == typeid(std::nullptr_t)){ return false; }
    if(value.type() == typeid(bool)){ return std::any_cast<bool>(value); }

    if(value.type() == typeid(double)){ return std::any_cast<double>(value) != 0; }
    if(value.type() == typeid(std::string)){ return std::any_cast<std::string>(value) != ""; }

    return true;
}

std::any Interpreter::visit(const UnaryExprAST* expr) {
    std::any right = expr->mRight->accept(*this);

    switch (expr->mOperator.getType())
    {
    case TokenType::MinusToken:
        return -std::any_cast<double>(right);

    case TokenType::PlusToken:
        return std::any_cast<double>(right);

    case TokenType::ExclamationToken:
        return !isTruthy(right);

    default:
        break;
    }

    return nullptr;
}

std::any Interpreter::visit(const LiteralExprAST<double>* expr) {
    return expr->mValue;
}

std::any Interpreter::visit(const LiteralExprAST<std::string>* expr) {
    return "\"" + expr->mValue + "\"";
}

std::any Interpreter::visit(const LiteralExprAST<bool>* expr) {
    return expr->mValue;
}

std::any Interpreter::visit(const LiteralExprAST<std::nullptr_t>* expr) {
    return nullptr;
}

void Interpreter::assertNumberOperand(const SyntaxToken& op, std::any left, std::any right){
    if(left.type() == typeid(double) && right.type() == typeid(double)){ return; }

    throw RuntimeError(op, "Operands must be numbers");
}


std::string stringify(std::any value){
    if(value.type() == typeid(std::nullptr_t)){ return "nil"; }
    if(value.type() == typeid(std::string)){ return std::any_cast<std::string>(value); }
    if(value.type() == typeid(bool)){ return std::any_cast<bool>(value) ? "true" : "false"; }

    if(value.type() == typeid(double)){ 
        // convert double to string
        std::string str = std::to_string(std::any_cast<double>(value));

        // remove trailing zeros and decimal point
        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
        str.erase ( str.find_last_not_of('.') + 1, std::string::npos );

        return str;
    }

    return "unknown type";
}

std::any Interpreter::visit(const ExpressionStmtAST* stmt) {
    return stmt->mExpr->accept(*this);
}

std::any Interpreter::visit(const PrintStmtAST* stmt) {
    std::any value = stmt->mExpr->accept(*this);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

void Interpreter::interpret(std::vector<std::unique_ptr<StmtAST>> const& stmts, bool& error) {
    std::cout << "Interpreting\n";
    try {

        auto it = stmts.begin();

        while(it != stmts.end()){
            (*it)->accept(*this);
            it++;
        }   

    } catch(const RuntimeError& e){
        // if an error occurs, output the error message and set the error flag to true
        runtimeAlert(e.mToken, e.what());
        error = true;
    }

}