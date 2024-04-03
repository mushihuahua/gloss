#include "environment.hpp"

void Environment::define(const std::string& name, const std::any& value){
    mValues[name] = value;
}

std::any Environment::get(const SyntaxToken& token){
    std::map<std::string, std::any>::iterator it = mValues.find(token.getLexeme());

    if(it != mValues.end()){
        return it->second;
    }

    runtimeAlert(token, "Undefined variable '" + token.getLexeme() + "'");
    return nullptr;
}