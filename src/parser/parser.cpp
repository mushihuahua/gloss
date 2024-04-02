#include <string.h>
#include <vector>

#include "parser.hpp"
#include "../alerts.hpp"

std::string expectedExpression(const char* position, const char* token) {
    std::string message = "Expected expression ";
    message += position;
    message += " '";
    message += token;
    message += "' token";
    return message;
}

Parser::Parser(std::vector<SyntaxToken>& tokens) : mTokens(tokens) {
    if (!mTokens.empty() && mPosition < mTokens.size()) {
        mCurrentToken = mTokens.at(mPosition);
    }
}

SyntaxToken Parser::peek(int offset){

    int index = mPosition + offset;

    if(index >= mTokens.size()-1){
        return mTokens.at(mTokens.size()-1);
    }
    return mTokens.at(index);
}

void Parser::consumeToken(TokenType type, std::string errMsg, size_t offset){

    if(peek(offset).getType() == type && peek(offset).getType() != TokenType::EOFToken){
        advance();
    } else {
        alert(errMsg, peek(offset).getLine());
        throw ParseError();
    }
    
}

void Parser::checkExpr(const std::unique_ptr<ExprAST>& expr, std::string errMsg){
    if(expr == nullptr){
        alert(errMsg, peek().getLine());
        throw ParseError();
    }
}

std::unique_ptr<ExprAST> Parser::parseBinaryExpr(std::unique_ptr<ExprAST> expr, std::function<std::unique_ptr<ExprAST>()> parseFunc){
    SyntaxToken op = peek(-1);
    checkExpr(expr, expectedExpression("before", op.getLexeme().c_str()));
    std::unique_ptr<ExprAST> right = parseFunc();
    checkExpr(right, expectedExpression("after", op.getLexeme().c_str()));
    expr = std::make_unique<BinaryExprAST>(op, std::move(expr), std::move(right));

    return expr;
}

bool Parser::match(std::vector<TokenType> types){
    for(TokenType type : types){
        if(peek().getType() == type && peek().getType() != TokenType::EOFToken){
            advance();
            return true;
        }
    }
    return false;
}

void Parser::advance(){
    mPosition++;
    mCurrentToken = mTokens.at(mPosition);
}

std::unique_ptr<ExprAST> Parser::expression(){
    return equality();
}

std::unique_ptr<StmtAST> Parser::statement(){
    if(match({TokenType::PrintToken})){ 
        std::unique_ptr<StmtAST> stmt = printStmt();
        return stmt; 
    }
    return exprStmt();
}

std::unique_ptr<StmtAST> Parser::exprStmt(){
    std::unique_ptr<ExprAST> expr = expression();
    consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    return std::make_unique<ExpressionStmtAST>(std::move(expr));
}

std::unique_ptr<StmtAST> Parser::printStmt(){
    if(peek().getType() != TokenType::LParenToken){
        alert("Expected parentheses after 'print'", peek().getLine());
        throw ParseError();
    }
    std::unique_ptr<ExprAST> expr = expression();
    if(peek(-1).getType() != TokenType::RParenToken){
        alert("Expected ')' after expression", peek().getLine());
        throw ParseError();
    }
    consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    return std::make_unique<PrintStmtAST>(std::move(expr));
}

std::unique_ptr<ExprAST> Parser::equality(){
    std::unique_ptr<ExprAST> expr = comparison();

    while(match({TokenType::ExclamationEqualToken, TokenType::EqualEqualToken})){
        expr = parseBinaryExpr(std::move(expr), [&](){ return comparison(); });
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::comparison(){
    std::unique_ptr<ExprAST> expr = term();

    while(match({TokenType::GreaterThanToken, TokenType::GreaterThanEqualToken, 
                 TokenType::LessThanToken, TokenType::LessThanEqualToken})){
        expr = parseBinaryExpr(std::move(expr), [&](){ return term(); });
    }

    return expr;
} 

std::unique_ptr<ExprAST> Parser::term(){
    std::unique_ptr<ExprAST> expr = factor();

    while(match({TokenType::MinusToken, TokenType::PlusToken})){
        expr = parseBinaryExpr(std::move(expr), [&](){ return factor(); });
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::factor(){
    std::unique_ptr<ExprAST> expr = unary();

    while(match({TokenType::DivideToken, TokenType::MultiplyToken})){
        expr = parseBinaryExpr(std::move(expr), [&](){ return unary(); });
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::unary(){
    if(match({TokenType::MinusToken, TokenType::PlusToken, TokenType::ExclamationToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = unary();
        checkExpr(right, "Invalid syntax");
        return std::make_unique<UnaryExprAST>(op, std::move(right));
    }

    return primary();
}

std::unique_ptr<ExprAST> Parser::primary(){

    // Number and String literals
    if(match({TokenType::NumberToken})){ return std::make_unique<LiteralExprAST<double>>(std::stod(peek(-1).getLexeme())); } 
    if(match({TokenType::StringToken})){ return std::make_unique<LiteralExprAST<std::string>>(peek(-1).getLexeme()); }

    // Boolean and Nil literals
    if(match({TokenType::TrueToken})){ return std::make_unique<LiteralExprAST<bool>>(true); }
    if (match({TokenType::FalseToken})){ return std::make_unique<LiteralExprAST<bool>>(false); }
    if(match({TokenType::NilToken})){ return std::make_unique<LiteralExprAST<std::nullptr_t>>(nullptr); }

    // Grouping of expressions
    if(match({TokenType::LParenToken})){
        std::unique_ptr<ExprAST> expr = expression();
        consumeToken(TokenType::RParenToken, "Expected ')'");
        return expr;
    }

    return nullptr;
}

std::vector<std::unique_ptr<StmtAST>> Parser::parse(){

    std::vector<std::unique_ptr<StmtAST>> statements;
    while(peek().getType() != TokenType::EOFToken){
        try{
            statements.push_back(statement());
        } catch(ParseError& e){
            synchronise();
        }
    }

    return statements;
}

void Parser::synchronise(){
    while(peek().getType() != TokenType::EOFToken){
        if(peek().getType() == TokenType::SemicolonToken){
            advance();
            return;
        }
        advance();
    }
}