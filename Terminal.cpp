#include "Terminal.h"




Terminal::Terminal() {
    venv = {
        {"AOSPATH", "/bin:/usr/bin"},
        {"AOSCWD", ""}, // update with cd command
        {"AOSHOME", ""} // originally copy from home n the shell that starts it up
    };
    commands =  {
            {"exit", 0},
            {"envset", 1},
            {"envunset", 2},
            {"envprt", 3},
            {"prt", 4},
            {"witch", 5},
            {"pwd", 6},
            {"cd", 7}
        };
    userInput = "";
};


void Terminal::printStatement(const std::vector<std::string>& text) {
    for (const auto& it: text) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
};

void Terminal::recieveInput(std::string textInput) {
    userInput = textInput;
    delete inputArgs;
    inputArgs = this->processStatement(textInput);
    printStatement(*inputArgs);
};

void Terminal::runStatements(std::vector<std::vector<std::string>> statements) {

};

std::vector<std::string>* Terminal::processStatement(std::string statement) {
    std::istringstream ss(statement);
    std::string argStr;
    std::vector<std::string>* statementVector = new std::vector<std::string>();
    while (ss >> std::quoted(argStr)) {
        statementVector->push_back(argStr);
    }
    
    this->replaceEnvironmentVariables(statementVector);
    return statementVector;
};

void Terminal::replaceEnvironmentVariables(std::vector<std::string>* statementVector) {
    std::size_t found;
    
    for (auto& it: *statementVector) {

        found = it.find("$",0,1);

        if (found != std::string::npos) {
            it = venv[it.substr(found+1)];
        }
    }
}
