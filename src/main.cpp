#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "lexer/lexer.hpp"
#include "alerts.hpp"

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
}

void run(const std::string& line){

    Lexer lx = Lexer(line);
    std::vector tokens = lx.scanTokens();
 
    for(auto & token : tokens){
        token.display();
    }
}