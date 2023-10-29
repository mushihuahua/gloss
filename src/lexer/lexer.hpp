#pragma once

#include <vector>
#include "token.hpp"

class Lexer{
    private:
        std::string mSource;
        std::vector<SyntaxToken> mTokens;

        int mStart = 0;
        int mPosition = 0;
        int mLine = 1;

        char current();
        bool isEOF() const;

        void scanToken();

        void addToken(TokenType type);
        void addToken(TokenType type, const std::string& lexeme);
    
    public:
        Lexer(const std::string&);

        std::vector<SyntaxToken> scanTokens();

};