#include "../ASTs/ExprAST.hpp"

class AstPrinterVisitor : public Visitor{

    public:
        void print(ExprAST* expr);
        void visit(const BinaryExprAST* expr)  override;
        void visit(const GroupingExprAST* expr)  override;
        void visit(const LiteralExprAST* expr)  override;
        void visit(const UnaryExprAST* expr)  override;

        AstPrinterVisitor() {};
};