#include <iostream>
#include <string>
#include <memory>

#include "AstPrinterVisitor.hpp"

void AstPrinterVisitor::print(ExprAST* expr) {
    expr->accept(*this);
}

void AstPrinterVisitor::visit(const BinaryExprAST* expr) { 
    expr->mLeft->accept(*this);
    expr->mRight->accept(*this);
    std::cout << expr->mOperator.getLexeme() << " ";

}

void AstPrinterVisitor::visit(const LiteralExprAST* expr) { 
    std::cout << *(int*)(expr->mValue) << " ";
}

void AstPrinterVisitor::visit(const UnaryExprAST* expr) { 
    std::cout << expr->mOperator.getLexeme();
    expr->mRight->accept(*this);
}

void AstPrinterVisitor::visit(const GroupingExprAST* expr) { 
    // std::cout << "( ";
    expr->mExpr->accept(*this);
    // std::cout << ") ";
}

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
