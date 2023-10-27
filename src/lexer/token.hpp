#pragma once

#include<iostream>
#include<string>

enum TokenType {
    StringToken,
    NumberToken,

    PlusToken, MinusToken,
    MultiplyToken, DivideToken,

    LPARENToken, RPARENToken,

    UnknownToken,
    WhitespaceToken,
    EOFToken
};

class SyntaxToken{
    private:
        const TokenType mType;
        const std::string mLexeme;
        const int mPosition;
    public:
        SyntaxToken(TokenType type, std::string lexeme, int position);
        void display();
        TokenType SyntaxToken::getType();
        std::string SyntaxToken::getLexeme();
};