#include "token.hpp"
#include <magic_enum.hpp>

SyntaxToken::SyntaxToken(TokenType type, const std::string& lexeme, int position, int line)
    : mType(type), mLexeme(lexeme), mPosition(position), mLine(line) {
}

void SyntaxToken::display(){
    std::cout << "SyntaxToken(" << magic_enum::enum_name(mType) << ", '" << mLexeme << "', " << mPosition << ", " << mLine << ")" << std::endl;
}

TokenType SyntaxToken::getType() const{
    return mType;
}

std::string SyntaxToken::getLexeme() const{
    return mLexeme;
}