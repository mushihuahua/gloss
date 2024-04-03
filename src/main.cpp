#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/visitors/AstPrinterVisitor.hpp"
#include "parser/interpreter/interpreter.hpp"
#include "parser/ASTs/ExprAST.hpp"
#include "alerts.hpp"

bool hadRuntimeError = false;

void run(const std::string& line);
void runPrompt();
void runFile(const char* path);

bool endsWith(const std::string& str, const std::string& suffix){
    return suffix.size() < str.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int main(int argc, char *argv[]){

    if(argc > 2){
        std::cout << "Too many arguments " << argc << std::endl;
        return 0;
    } else if(argc == 2){
        try{
            runFile(argv[1]);
        } catch (const char* err){
            alert(err, argv[1]);
        }
    } else {
        runPrompt();
    }

    return 0;
}

void runPrompt(){

    for(;;){

        std::string input;
            
        std::cout << "> " ;
        getline(std::cin, input);

        if(input.empty() || input == "exit") break;

        run(input);
    }

}

void runFile(const char * path){

    if(!endsWith(path, ".gls")){
        throw "File is not a .gls file";
    }

    std::ifstream sourceCode(path);

    if(!sourceCode.good()){
        throw "File does not exist";
    }  

    std::stringstream buffer;
    buffer << sourceCode.rdbuf();

    run(buffer.str()); 

    if(hadRuntimeError){
        exit(70);
    }
}

void run(const std::string& line){

    Lexer lx = Lexer(line);
    std::vector<SyntaxToken> tokens = lx.scanTokens();

    Parser parser = Parser(tokens);
    std::vector<std::unique_ptr<StmtAST>> expressions = parser.parse();

    // if(expression == nullptr){
    //     return;
    // }

    // AstPrinterVisitor printer = AstPrinterVisitor();
    // printer.print(expression.get());

    Interpreter interpreter = Interpreter();
    interpreter.interpret(expressions, hadRuntimeError);
 
    for(auto & token : tokens){
        token.display();
    }
}