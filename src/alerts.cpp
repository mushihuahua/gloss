#include "alerts.hpp"


void alert(std::string message, std::string file, int line){
    std::cerr << file << ":" << line << " | " << "[error]: " << message << std::endl;
}

void alert(std::string message, std::string file){
    alert(message, file, 0);
};

void alert(std::string message, int line){
    std::cerr << "[line " << line << "]" << " | " << "[error]: " << message << std::endl;
}
