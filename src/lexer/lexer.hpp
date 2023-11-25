#pragma once

#include <vector>
#include <map>
#include "token.hpp"

class Lexer{
    private:
        std::string mSource;
        std::vector<SyntaxToken> mTokens;

        int mStart = 0;
        int mPosition = 0;
        int mLine = 1;

        char nextChar();
        char current();
        bool isEOF() const;

        void scanToken();

        void addToken(TokenType type);
        void addToken(TokenType type, const std::string& lexeme);
        void addToken(TokenType type, const std::string& lexeme, int position);

        bool match(char expected);
    
    public:
        Lexer(const std::string&);

        std::vector<SyntaxToken> scanTokens();

};