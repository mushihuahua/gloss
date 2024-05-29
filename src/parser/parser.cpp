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

SyntaxToken Parser::nextToken(){
    SyntaxToken token = peek();
    advance();

    return token;
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

std::unique_ptr<ExprAST> Parser::parseLogicalExpr(std::unique_ptr<ExprAST> expr, std::function<std::unique_ptr<ExprAST>()> parseFunc){
    SyntaxToken op = peek(-1);
    checkExpr(expr, expectedExpression("before", op.getLexeme().c_str()));
    std::unique_ptr<ExprAST> right = parseFunc();
    checkExpr(right, expectedExpression("after", op.getLexeme().c_str()));
    expr = std::make_unique<LogicalExprAST>(op, std::move(expr), std::move(right));

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
    return assignment();
}

std::unique_ptr<ExprAST> Parser::assignment(){

    if(peek(1).getType() == TokenType::EqualToken){
        if(peek().getType() == TokenType::IdentifierToken){
            SyntaxToken lVal = nextToken();
            SyntaxToken op = nextToken();

            std::unique_ptr<ExprAST> rVal = assignment();

            return std::make_unique<AssignExprAST>(lVal, std::move(rVal));
        }

        alert("Did you mean to do '=='?", peek(1).getLine());
    }

    return logicalOr();
}

std::unique_ptr<ExprAST> Parser::logicalOr(){
    std::unique_ptr<ExprAST> expr = logicalAnd();

    if(peek().getType() == TokenType::ErrorOrToken){
        alert("Did you mean to do '||'?", peek().getLine());
        throw ParseError();
    }

    while(match({TokenType::OrToken})){
        expr = parseLogicalExpr(std::move(expr), [&](){ return logicalAnd(); });
    }

    return expr;
}

std::unique_ptr<ExprAST> Parser::logicalAnd(){
    std::unique_ptr<ExprAST> expr = equality();

    if(peek().getType() == TokenType::ErrorAndToken){
        alert("Did you mean to do '&&'?", peek().getLine());
        throw ParseError();
    }

    while(match({TokenType::AndToken})){
        expr = parseLogicalExpr(std::move(expr), [&](){ return equality(); });
    }

    return expr;
}

std::unique_ptr<StmtAST> Parser::statement(){
    if(match({TokenType::PrintToken})){ 
        std::unique_ptr<StmtAST> stmt = printStmt();
        return stmt; 
    }

    if(match({TokenType::WhileToken})){
        std::unique_ptr<StmtAST> stmt = whileStmt();
        return stmt;
    }

    if(match({TokenType::ForToken})){
        std::unique_ptr<StmtAST> stmt = forStmt();
        return stmt;
    }

    if(match({TokenType::IfToken})){
        std::unique_ptr<StmtAST> stmt = ifStmt();
        return stmt;
    }

    if(match({TokenType::LBraceToken})){ 
        return std::make_unique<BlockStmtAST>(block()); 
    }

    return exprStmt();
}

std::vector<std::unique_ptr<StmtAST>> Parser::block(){
    std::vector<std::unique_ptr<StmtAST>> statements;

    while(peek().getType() != TokenType::RBraceToken && peek().getType() != TokenType::EOFToken){
        statements.push_back(declaration());
    }

    consumeToken(TokenType::RBraceToken, "Expected a '}'");
    return statements;
}

std::unique_ptr<StmtAST> Parser::declaration(){
    // check if the next token is a var token
    if(match({TokenType::VarToken})){
        return varDecl();
    }
    return statement();
}

std::unique_ptr<StmtAST> Parser::varDecl(){
    // check if the next token is an identifier and move to the next token, output an error if it is not
    consumeToken(TokenType::IdentifierToken, "Expected an identifier");
    // get the identifier token
    SyntaxToken token = peek(-1);
    std::unique_ptr<ExprAST> expr = nullptr;

    if(match({TokenType::EqualToken})){
        expr = expression();
    }

    consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    // create a new variable statement ast node and return it
    return std::make_unique<VarStmtAST>(token, std::move(expr));
}

std::unique_ptr<StmtAST> Parser::exprStmt(){
    std::unique_ptr<ExprAST> expr = expression();
    consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    return std::make_unique<ExpressionStmtAST>(std::move(expr));
}

std::unique_ptr<StmtAST> Parser::printStmt(){

    consumeToken(TokenType::LParenToken, "Expected parentheses after 'print'");
    std::unique_ptr<ExprAST> expr = expression();
    consumeToken(TokenType::RParenToken, "Expected ')' after expression");

    consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    return std::make_unique<PrintStmtAST>(std::move(expr));
}

std::unique_ptr<StmtAST> Parser::ifStmt(){
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> thenStmt;
    std::unique_ptr<StmtAST> elseStmt = nullptr;

    consumeToken(TokenType::LParenToken, "Expected parentheses after 'if'");

    condition = expression();

    if(condition == nullptr){
        alert("Condition given is invalid", peek().getLine());
        throw ParseError();
    }
    
    consumeToken(TokenType::RParenToken, "Expected ')' after expression");

    thenStmt = statement();

    if(match({TokenType::ElseToken})){
        elseStmt = statement();
    }

    return std::make_unique<IfStmtAST>(std::move(condition), std::move(thenStmt), std::move(elseStmt));
}

std::unique_ptr<StmtAST> Parser::whileStmt(){
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> body;

    consumeToken(TokenType::LParenToken, "Expected parentheses after 'while'");

    condition = expression();

    if(condition == nullptr){
        alert("Condition given is invalid", peek().getLine());
        throw ParseError();
    }

    consumeToken(TokenType::RParenToken, "Expected ')' after expression");

    body = statement();

    return std::make_unique<WhileStmtAST>(std::move(condition), std::move(body));
}

std::unique_ptr<StmtAST> Parser::forStmt(){
    std::unique_ptr<StmtAST> initialiser = nullptr;
    std::unique_ptr<ExprAST> condition = nullptr;
    std::unique_ptr<ExprAST> incrementor = nullptr;

    std::vector<std::unique_ptr<StmtAST>> stmts;
    std::unique_ptr<StmtAST> body;

    consumeToken(TokenType::LParenToken, "Expected parentheses after 'for'");

    if(peek().getType() != TokenType::SemicolonToken){
        if(match({TokenType::VarToken})){
            initialiser = varDecl();
        } else {
            initialiser = exprStmt();
        }
    } else {
        consumeToken(TokenType::SemicolonToken, "Expected a ';'");
    }

    if(peek().getType() != TokenType::SemicolonToken){
        condition = expression();
    }
    consumeToken(TokenType::SemicolonToken, "Expected a ';'");

    if(peek().getType() != TokenType::RParenToken){
        incrementor = expression();
    }

    consumeToken(TokenType::RParenToken, "Expected ')' after expression");

    body = statement();

    if(initialiser != nullptr){
        stmts.push_back(std::move(initialiser));
    }

    if(condition == nullptr) {
        condition = std::make_unique<LiteralExprAST<bool>>(true);
    }

    if(incrementor != nullptr){
        std::vector<std::unique_ptr<StmtAST>> stmts;
        std::unique_ptr<StmtAST> incrementorStmt = std::make_unique<ExpressionStmtAST>(std::move(incrementor));

        auto* bodyPtr = body.get();

        if(typeid(*bodyPtr) == typeid(BlockStmtAST)){
            stmts = std::move(static_cast<BlockStmtAST&>(*body).mStmts);
            stmts.push_back(std::move(incrementorStmt));
            body = std::make_unique<BlockStmtAST>(std::move(stmts));
        } 
        else {
            stmts.push_back(std::move(body));
            stmts.push_back(std::move(incrementorStmt));
            body = std::make_unique<BlockStmtAST>(std::move(stmts));
        }
    }

   stmts.push_back(
        std::make_unique<WhileStmtAST>(std::move(condition), std::move(body))
    );

    return std::make_unique<BlockStmtAST>(std::move(stmts));
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
        checkExpr(right, "Invalid syntax.");
        return std::make_unique<UnaryExprAST>(op, std::move(right));
    }

    return group();
}

std::unique_ptr<ExprAST> Parser::group(){
    if(match({TokenType::LParenToken})){
        std::unique_ptr<ExprAST> expr = expression();
        consumeToken(TokenType::RParenToken, "Expected ')'");
        return expr;
    }

    return primary();
}

std::unique_ptr<ExprAST> Parser::primary(){

    // Number and String literals
    if(match({TokenType::NumberToken})){ return std::make_unique<LiteralExprAST<double>>(std::stod(peek(-1).getLexeme())); } 
    if(match({TokenType::StringToken})){ return std::make_unique<LiteralExprAST<std::string>>(peek(-1).getLexeme()); }

    // Identifier
    if(match({TokenType::IdentifierToken})){ return std::make_unique<VariableExprAST>(peek(-1)); }

    // Boolean and Nil literals
    if(match({TokenType::TrueToken})){ return std::make_unique<LiteralExprAST<bool>>(true); }
    if (match({TokenType::FalseToken})){ return std::make_unique<LiteralExprAST<bool>>(false); }
    if(match({TokenType::NilToken})){ return std::make_unique<LiteralExprAST<std::nullptr_t>>(nullptr); }

    if(peek().getType() == TokenType::EOFToken){
        alert("Unexpected end of file", peek().getLine());
        throw ParseError();
    }

    if(peek().getType() == TokenType::UnknownToken){
        alert("Invalid syntax.", peek().getLine());
        throw ParseError();
    }

    return nullptr;
}

std::vector<std::unique_ptr<StmtAST>> Parser::parse(){

    std::vector<std::unique_ptr<StmtAST>> statements;
    while(peek().getType() != TokenType::EOFToken){
        try{
            statements.push_back(declaration());
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
