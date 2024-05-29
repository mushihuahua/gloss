#include <gtest/gtest.h>
#include <iostream>

#include<vector>
#include<string>
#include<memory>

#include "../../src/lexer/lexer.hpp"
#include "../../src/lexer/token.hpp"

void expectTokens(const std::vector<SyntaxToken>& expectedTokens, const std::vector<SyntaxToken>& tokens){
    for(int i = 0; i < expectedTokens.size(); i++){
        EXPECT_EQ(expectedTokens[i].getType(), tokens[i].getType());
        EXPECT_EQ(expectedTokens[i].getLexeme(), tokens[i].getLexeme());
        EXPECT_EQ(expectedTokens[i].getPosition(), tokens[i].getPosition());
        EXPECT_EQ(expectedTokens[i].getLine(), tokens[i].getLine());
    }
}

TEST(GeneralTest, LexerTokenTests) {
    Lexer lex("for(var i = 0; i < 90; i = i+1){ print(i); }");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::ForToken, "for", 0, 1),
        SyntaxToken(TokenType::LParenToken, "(", 3, 1),
        SyntaxToken(TokenType::VarToken, "var", 4, 1),
        SyntaxToken(TokenType::IdentifierToken, "i", 8, 1),
        SyntaxToken(TokenType::EqualToken, "=", 10, 1),
        SyntaxToken(TokenType::NumberToken, "0", 12, 1),
        SyntaxToken(TokenType::SemicolonToken, ";", 13, 1),
        SyntaxToken(TokenType::IdentifierToken, "i", 15, 1),
        SyntaxToken(TokenType::LessThanToken, "<", 17, 1),
        SyntaxToken(TokenType::NumberToken, "90", 19, 1),
        SyntaxToken(TokenType::SemicolonToken, ";", 21, 1),
        SyntaxToken(TokenType::IdentifierToken, "i", 23, 1),
        SyntaxToken(TokenType::EqualToken, "=", 25, 1),
        SyntaxToken(TokenType::IdentifierToken, "i", 27, 1),
        SyntaxToken(TokenType::PlusToken, "+", 28, 1),
        SyntaxToken(TokenType::NumberToken, "1", 29, 1),
        SyntaxToken(TokenType::RParenToken, ")", 30, 1),
        SyntaxToken(TokenType::LBraceToken, "{", 31, 1),
        SyntaxToken(TokenType::PrintToken, "print", 33, 1),
        SyntaxToken(TokenType::LParenToken, "(", 38, 1),
        SyntaxToken(TokenType::IdentifierToken, "i", 39, 1),
        SyntaxToken(TokenType::RParenToken, ")", 40, 1),
        SyntaxToken(TokenType::SemicolonToken, ";", 41, 1),
        SyntaxToken(TokenType::RBraceToken, "}", 43, 1),
        SyntaxToken(TokenType::EOFToken, "EOF", 44, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(MultipleLineCommentTest, LexerTokenTests) {
    Lexer lex("/* while for if var 5334 <= >= +@ */");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::EOFToken, "EOF", 36, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(InlineCommentTest, LexerTokenTests) {
    Lexer lex("// while for if var 5334 <= >= +@");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::EOFToken, "EOF", 33, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(FloatTest, LexerTokenTests) {
    Lexer lex("3.3252");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::NumberToken, "3.3252", 0, 1),
        SyntaxToken(TokenType::EOFToken, "EOF", 6, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(FloatTest2, LexerTokenTests) {
    Lexer lex("3.23234.4234");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::NumberToken, "3.23234", 0, 1),
        SyntaxToken(TokenType::DotToken, ".", 7, 1),
        SyntaxToken(TokenType::NumberToken, "4234", 8, 1),
        SyntaxToken(TokenType::EOFToken, "EOF", 12, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(IdentifierTest, LexerTokenTests) {
    Lexer lex("varname192");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::IdentifierToken, "varname192", 0, 1),
        SyntaxToken(TokenType::EOFToken, "EOF", 7, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

TEST(IdentifierTest2, LexerTokenTests) {
    Lexer lex("var_name");

    std::vector<SyntaxToken> expectedTokens = {
        SyntaxToken(TokenType::IdentifierToken, "var_name", 0, 1),
        SyntaxToken(TokenType::EOFToken, "EOF", 8, 1)
    };

    std::vector<SyntaxToken> tokens = lex.scanTokens();

    expectTokens(expectedTokens, tokens);
}

