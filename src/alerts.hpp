#include <string>
#include <iostream>
#include "lexer/token.hpp"

void alert(std::string message, std::string file, int line);
void alert(std::string message, std::string file);
void alert(std::string message, int line);

void runtimeAlert(const SyntaxToken& token, const std::string& msg);