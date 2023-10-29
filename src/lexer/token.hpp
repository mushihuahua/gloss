#pragma once

#include<iostream>
#include<string>

enum TokenType {
    StringToken,
    NumberToken,

    PlusToken, MinusToken,
    MultiplyToken, DivideToken,

    LParenToken, RParenToken,
    LBraceToken, RBraceToken,
    CommaToken, DotToken, SemicolonToken,

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
        SyntaxToken(TokenType type, const std::string& lexeme, int position);
        void display();
        TokenType getType() const;
        std::string getLexeme() const;
};