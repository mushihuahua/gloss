#include <string.h>

#include "parser.hpp"
#include "../alerts.hpp"

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

void Parser::consumeToken(TokenType type, std::string errMsg){

    if(peek().getType() == type && peek().getType() != TokenType::EOFToken){
        advance();
    } else {
        alert(errMsg, peek().getLine());
        throw ParseError();
    }
    
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

std::unique_ptr<ExprAST> Parser::equality(){
    std::unique_ptr<ExprAST> expr = comparison();

    while(match({TokenType::ExclamationEqualToken, TokenType::EqualEqualToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = comparison();
        expr = std::make_unique<BinaryExprAST>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::comparison(){
    std::unique_ptr<ExprAST> expr = term();

    while(match({TokenType::GreaterThanToken, TokenType::GreaterThanEqualToken, 
                 TokenType::LessThanToken, TokenType::LessThanEqualToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = term();
        expr = std::make_unique<BinaryExprAST>(op, std::move(expr), std::move(right));
    }

    return expr;
} 

std::unique_ptr<ExprAST> Parser::term(){
    std::unique_ptr<ExprAST> expr = factor();

    while(match({TokenType::MinusToken, TokenType::PlusToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = factor();
        expr = std::make_unique<BinaryExprAST>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::factor(){
    std::unique_ptr<ExprAST> expr = unary();

    while(match({TokenType::DivideToken, TokenType::MultiplyToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = unary();
        expr = std::make_unique<BinaryExprAST>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::unary(){
    if(match({TokenType::MinusToken, TokenType::ExclamationToken})){
        SyntaxToken op = peek(-1);
        std::unique_ptr<ExprAST> right = unary();
        return std::make_unique<UnaryExprAST>(op, std::move(right));
    }

    return primary();
}

std::unique_ptr<ExprAST> Parser::primary(){
    if(match({TokenType::NumberToken})){
        return std::make_unique<LiteralExprAST<double>>(std::stod(peek(-1).getLexeme()));
    } 
    if(match({TokenType::StringToken})){
        return std::make_unique<LiteralExprAST<std::string>>(peek(-1).getLexeme());
    }

    if(match({TokenType::LParenToken})){
        std::unique_ptr<ExprAST> expr = parse();
        consumeToken(TokenType::RParenToken, "Expected ')'");
        return expr;
    }

    throw ParseError("Unexpected token");
}

std::unique_ptr<ExprAST> Parser::parse(){
    try
    {
        return expression();
    }
    catch(const std::exception& e)
    {
        if(!(e.what()[0] == '\0')) {
            std::cerr << e.what() << '\n';
        }
        return nullptr;
    }
}