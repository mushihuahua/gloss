#pragma once

#include <iostream>
#include<string>
#include<vector>
#include<memory>
#include <stdexcept>

#include "../lexer/lexer.hpp"
#include "ASTs/ExprAST.hpp"
#include "ASTs/StmtAST.hpp"


class ParseError : public std::exception {
    private:
    std::string message;

    public:
    ParseError(const std::string& msg = "") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Parser {

    private:  
        SyntaxToken mCurrentToken = SyntaxToken(TokenType::UnknownToken, "", 0, 0);
        std::vector<SyntaxToken> mTokens;
        size_t mPosition = 0;

        void consumeToken(TokenType type, std::string errMsg, size_t offset = 0);
        void checkExpr(const std::unique_ptr<ExprAST>& expr, std::string errMsg);
        std::unique_ptr<ExprAST> parseBinaryExpr(std::unique_ptr<ExprAST> expr, std::function<std::unique_ptr<ExprAST>()> parseFunc);

        bool match(std::vector<TokenType> types);
        void advance();
        SyntaxToken peek(int offset = 0);
        void synchronise();

        /*  
        Current Basic Grammar:

        program        → declaration* EOF ;

        declaration    → varDecl | statement ;
        statement      → exprStmt | printStmt ;

        exprStmt       → expression ";" ;
        printStmt      → "print" "(" expression ")" ";" ;

        expression     → equality ;
        equality       → comparison ( ( "!=" | "==" ) comparison )* ;
        comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        term           → factor ( ( "-" | "+" ) factor )* ;
        factor         → unary ( ( "/" | "*" ) unary )* ;
        unary          → ( "!" | "-" ) unary | primary ;
        primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
        */

        std::unique_ptr<StmtAST> statement();
        std::unique_ptr<StmtAST> exprStmt();
        std::unique_ptr<StmtAST> printStmt();
        std::unique_ptr<StmtAST> declaration();
        std::unique_ptr<StmtAST> varDecl();

        std::unique_ptr<ExprAST> expression();
        std::unique_ptr<ExprAST> equality();
        std::unique_ptr<ExprAST> comparison();
        std::unique_ptr<ExprAST> term();
        std::unique_ptr<ExprAST> factor();
        std::unique_ptr<ExprAST> unary();
        std::unique_ptr<ExprAST> primary();
        
        
    public:
        Parser(std::vector<SyntaxToken>& tokens);
        std::vector<std::unique_ptr<StmtAST>> parse();
};