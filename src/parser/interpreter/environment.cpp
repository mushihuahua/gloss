#include "environment.hpp"

void Environment::define(const SyntaxToken& token, const std::any& value){
    std::map<std::string, std::any>::iterator it = mValues.find(token.getLexeme());

    if(it != mValues.end()){
        runtimeAlert(token, "Variable '" + token.getLexeme() + "' already defined");
        return;
    }

    mValues[token.getLexeme()] = value;
}

std::any Environment::get(const SyntaxToken& token){
    std::map<std::string, std::any>::iterator it = mValues.find(token.getLexeme());

    if(it != mValues.end()){
        if(it->second.type() == typeid(std::nullptr_t)){
            runtimeAlert(token, "Variable '" + token.getLexeme() + "' was initialised but never assigned a value");
            return nullptr;
        }
        return it->second;
    }

    runtimeAlert(token, "Undefined variable '" + token.getLexeme() + "'");
    return nullptr;
}

void Environment::assign(const SyntaxToken& token, const std::any& value){
    std::map<std::string, std::any>::iterator it = mValues.find(token.getLexeme());

    if(it != mValues.end()){
        it->second = value;
        return;
    }

    runtimeAlert(token, "Undefined variable '" + token.getLexeme() + "'");
}