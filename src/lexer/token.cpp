#include "token.hpp"

SyntaxToken::SyntaxToken(TokenType type, std::string lexeme, int position)
    : mType(type), mLexeme(lexeme), mPosition(position) {
}

void SyntaxToken::display(){
    std::cout << "SyntaxToken(" << mType << ", '" << mLexeme << "', " << mPosition << ")" << std::endl;
}

TokenType SyntaxToken::getType(){
    return mType;
}

std::string SyntaxToken::getLexeme(){
    return mLexeme;
}