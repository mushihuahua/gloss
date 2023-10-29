#include "token.hpp"

SyntaxToken::SyntaxToken(TokenType type, const std::string& lexeme, int position)
    : mType(type), mLexeme(lexeme), mPosition(position) {
}

void SyntaxToken::display(){
    std::cout << "SyntaxToken(" << mType << ", '" << mLexeme << "', " << mPosition << ")" << std::endl;
}

TokenType SyntaxToken::getType() const{
    return mType;
}

std::string SyntaxToken::getLexeme() const{
    return mLexeme;
}