#pragma once

#include<iostream>
#include<string>

enum TokenType {

    // literals 
    StringToken,
    NumberToken,
    IdentifierToken,

    PlusToken, MinusToken,
    MultiplyToken, DivideToken,

    LParenToken, RParenToken,
    LBraceToken, RBraceToken,
    CommaToken, DotToken, SemicolonToken, 

    ExclamationToken, ExclamationEqualToken,
    EqualToken, EqualEqualToken,

    LessThanToken, LessThanEqualToken,
    GreaterThanToken, GreaterThanEqualToken,

    IfToken, ElseToken, PrintToken, VarToken,

    TrueToken, FalseToken, NilToken,

    UnknownToken,
    WhitespaceToken,
    EOFToken
};

class SyntaxToken{
    private:
        const TokenType mType;
        const std::string mLexeme;
        const int mPosition;
        const int mLine;
    public:
        SyntaxToken(TokenType type, const std::string& lexeme, int position, int line);
        void display();
        TokenType getType() const;
        std::string getLexeme() const;
        int getLine() const;

        SyntaxToken& operator=(const SyntaxToken& other) {
            return *this;
        }

};

