#pragma once

#include <iostream>
#include<string>
#include<vector>
#include<memory>
#include <stdexcept>

#include "../lexer/lexer.hpp"
#include "ASTs/ExprAST.hpp"


class ParseError : public std::exception {
    private:
    std::string message;

    public:
    ParseError(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Parser{

    private:  
        SyntaxToken mCurrentToken = SyntaxToken(TokenType::UnknownToken, "", 0, 0);
        std::vector<SyntaxToken> mTokens;
        size_t mPosition = 0;

        void consumeToken(TokenType type);
        bool match(std::vector<TokenType> types);
        void advance();
        SyntaxToken peek(int offset = 0);

        /*  
        Current Basic Grammar:

        expression     → equality ;
        equality       → comparison ( ( "!=" | "==" ) comparison )* ;
        comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        term           → factor ( ( "-" | "+" ) factor )* ;
        factor         → unary ( ( "/" | "*" ) unary )* ;
        unary          → ( "!" | "-" ) unary | primary ;
        primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
        */
        std::unique_ptr<ExprAST> expression();
        std::unique_ptr<ExprAST> equality();
        std::unique_ptr<ExprAST> comparison();
        std::unique_ptr<ExprAST> term();
        std::unique_ptr<ExprAST> factor();
        std::unique_ptr<ExprAST> unary();
        std::unique_ptr<ExprAST> primary();
        
        
    public:
        Parser(std::vector<SyntaxToken>& tokens);
        std::unique_ptr<ExprAST> parse();
};