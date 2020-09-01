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
            {"set", 1},
            {"envunset", 2},
            {"unset", 2},
            {"envprt", 3},
            {"prt", 4},
            {"witch", 5},
            {"pwd", 6},
            {"cd", 7}
        };
    userInput = "";
};


void Terminal::printStatement(std::vector<std::string>::const_iterator * it, std::vector<std::string>::const_iterator * end) {
    bool endStatement = false;

    while (*it != *end) {
        endStatement = (((*it)+1) == *end);
        std::cout << **it << (endStatement ? "" : " ");
        ++(*it);
    }
    std::cout << std::endl;
};

void Terminal::recieveInput(std::string textInput) {
    userInput = textInput;
    delete inputArgs;
    inputArgs = this->processStatement(textInput);
    this->runStatement(inputArgs);
};

void Terminal::setEnvironmentVariable(std::vector<std::string>::const_iterator * it, std::vector<std::string>::const_iterator * end) {
    std::string key = **it;
    std::string val = "d";
    ++(*it);
    if (*it != *end)
        val = **it;
    if (val.find("$", 0, 1) == std::string::npos)
        venv[key] = val;
}

void Terminal::runStatement(std::vector<std::string> * const statement) {
    std::vector<std::string>::const_iterator it = statement->begin();
    std::vector<std::string>::const_iterator end = statement->end();
    int cmd = commands[*it];
    ++it;
    
    switch(cmd) {
        case 0:
            exit(0);
            break;
        case 1:
            this->setEnvironmentVariable(&it, &end);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            this->printStatement(&it, &end);
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        default:
            break;
    }


    

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
