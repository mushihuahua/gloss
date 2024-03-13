#include "../src/parser/visitors/AstPrinterVisitor.hpp"

int main(int argc, char const *argv[])
{
    BinaryExprAST* binaryExpr = new BinaryExprAST(
        SyntaxToken(TokenType::MultiplyToken, "*", 0, 0),
        std::make_unique<GroupingExprAST>(
            std::make_unique<BinaryExprAST>(
                SyntaxToken(TokenType::PlusToken, "+", 0, 0), 
                std::make_unique<LiteralExprAST>(new int(1)), std::make_unique<LiteralExprAST>(new int(2))
            )
        ), 
        std::make_unique<BinaryExprAST>(
            SyntaxToken(TokenType::MinusToken, "-", 0, 0), 
            std::make_unique<UnaryExprAST>(SyntaxToken(TokenType::MinusToken, "-", 0, 0), std::make_unique<LiteralExprAST>(new int(4))), 
            std::make_unique<LiteralExprAST>(new int(3))
        )
    );

    AstPrinterVisitor printer = AstPrinterVisitor();
    printer.print(binaryExpr);
    std::cout << "\n";


    return 0;
}
