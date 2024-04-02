#include <sstream>

#include "lexer.hpp"
#include "../alerts.hpp"

std::map<std::string, TokenType> keywords
{
    {"if", IfToken},
    {"else", ElseToken},
    {"true", TrueToken},
    {"false", FalseToken},
    {"nil", NilToken}
};

Lexer::Lexer(const std::string& source)
    : mSource(source) {}
    
bool Lexer::isEOF() const {
    return (mPosition >= mSource.length());
}

char Lexer::nextChar(){

    if((mPosition+1 >= mSource.length())){
        return '\0';
    }

    return mSource[(mPosition++)+1];
}

char Lexer::current(){
    if(isEOF()){
        return '\0';
    }

    return mSource[mPosition];
}

std::vector<SyntaxToken> Lexer::scanTokens(){
    while(!isEOF()){
        scanToken();
    }

    addToken(EOFToken, "EOF", mSource.length());
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
    mPosition++;

    if(isdigit(curr)){
        while(isdigit(current())){mPosition++;}

        if(current() == '.' && isdigit(nextChar())){
            while(isdigit(current())){mPosition++;}
        }

        addToken(NumberToken);
        return;
    }

    if(isalpha(curr)){
        while(isalpha(current())){mPosition++;}

        std::string identifier = mSource.substr(mStart, (mPosition-mStart));
        std::map<std::string, TokenType>::iterator it = keywords.find(identifier);

        if(it == keywords.end()) { 
            std::ostringstream err;
            err << "Undefined identifier '" << identifier << "'";
            alert(err.str(), mLine);
            return;
        }

        addToken(it->second);
        return;

    }

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
            addToken(match('=') ? EqualEqualToken : EqualToken);
            break;
        case '<':
            addToken(match('=') ? LessThanEqualToken : LessThanToken);
            break;
        case '>':
            addToken(match('=') ? GreaterThanEqualToken : GreaterThanToken);
            break;

        case '/':
            if(match('/')){
                while(current() != '\n' && !isEOF()){ mPosition++; }
            } 

            else if(match('*')){
                while(!isEOF()){
                    if(current() == '*'){
                        mPosition++;
                        if(match('/')){
                            break;
                        }
                    }

                    if(current() == '\n'){ mLine++; }
                    mPosition++;
                }

                if(isEOF()){
                    alert("Unterminated comment", mLine);
                    break;
                }

            } else {
                addToken(DivideToken);
            }

            break;


        case '"':
            while(current() != '"' && !isEOF()){
                if(current() == '\n'){ mLine++; }
                mPosition++;
            }

            if(isEOF()){
                alert("Unterminated string literal", mLine);
                break;
            }
            
            mPosition++;

            addToken(StringToken, mSource.substr(mStart+1, (mPosition-mStart)-2));

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
    mTokens.push_back(SyntaxToken(type, lexeme, mStart, mLine));
}

void Lexer::addToken(TokenType type, const std::string& lexeme, int position){
    mTokens.push_back(SyntaxToken(type, lexeme, position, mLine));
}