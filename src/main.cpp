#include <iostream>
#include <fstream>

void runPrompt();
void runFile(const char* path);

void alert(std::string file, std::string message, int line){
    std::cerr << file << ":" << line << " | " << "[error]: " << message << std::endl;
}

void alert(std::string file, std::string message){
    std::cerr << file << " | " << "[error]: " << message << std::endl;
};

int main(int argc, char *argv[]){

    if(argc > 2){
        std::cout << "Too many arguments " << argc << std::endl;
        return 0;
    } else if(argc == 2){
        try{
            runFile(argv[1]);
        } catch (const char* err){
            alert(argv[1], err);
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

        std::cout << input << std::endl;
    }

}

void runFile(const char * path){

    std::ifstream sourceCode(path);

    if(!sourceCode.good()){
        throw "File does not exist";
    }  

    std::string line; 

    while (std::getline(sourceCode, line))
    {
        std::cout << line << std::endl;
    }   
}