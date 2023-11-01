#include <sstream>

#include "lexer.hpp"
#include "../alerts.hpp"

Lexer::Lexer(const std::string& source)
    : mSource(source) {}
    
bool Lexer::isEOF() const {
    return (mPosition >= mSource.length());
}

char Lexer::current(){
    if(isEOF()){
        return '\0';
    }

    return mSource[mPosition++];
}

std::vector<SyntaxToken> Lexer::scanTokens(){
    while(!isEOF()){
        scanToken();
    }

    addToken(EOFToken, "EOF");
    return mTokens;
}

bool Lexer::match(char expected) {
    if (isEOF()) return false;

    if(mSource[mPosition] == expected){
        mPosition++;
        return true;
    }

    return false;
}


void Lexer::scanToken(){

    mStart = mPosition;
    char curr = current();

    switch (curr)
    {

        case '(': addToken(LParenToken); break;
        case ')': addToken(RParenToken); break;
        case '{': addToken(LBraceToken); break;
        case '}': addToken(RBraceToken); break;
        case ',': addToken(CommaToken); break;
        case '.': addToken(DotToken); break;
        case '-': addToken(MinusToken); break;
        case '+': addToken(PlusToken); break;
        case ';': addToken(SemicolonToken); break;
        case '*': addToken(MultiplyToken); break; 

        case '!':
            addToken(match('=') ? ExclamationEqualToken : ExclamationToken);
            break;
        case '=':
            addToken(match('=') ? EqualToken : EqualToken);
            break;
        case '<':
            addToken(match('=') ? LessThanEqualToken : LessThanToken);
            break;
        case '>':
            addToken(match('=') ? GreaterThanEqualToken : GreaterThanToken);
            break;


        case ' ':
        case '\r':
        case '\t':
            // addToken(WhitespaceToken);
            break;

        case '\n':
            mLine++;
            break;

        default:
            std::ostringstream err;
            err << "Unexpected character '" << curr << "'";
            alert(err.str(), mLine);
            break;
    }
}

void Lexer::addToken(TokenType type){
    addToken(type, mSource.substr(mStart, (mPosition-mStart)));
}

void Lexer::addToken(TokenType type, const std::string& lexeme){
    mTokens.push_back(SyntaxToken(type, lexeme, mPosition));
}